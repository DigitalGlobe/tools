# Copyright (C) 2013 Adobe Systems Incorporated. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# 1. Redistributions of source code must retain the above
#    copyright notice, this list of conditions and the following
#    disclaimer.
# 2. Redistributions in binary form must reproduce the above
#    copyright notice, this list of conditions and the following
#    disclaimer in the documentation and/or other materials
#    provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER "AS IS" AND ANY
# EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
# PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
# OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
# PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
# TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
# THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
# SUCH DAMAGE.

import logging
import re

from webkitpy.common.host import Host
from webkitpy.common.webkit_finder import WebKitFinder
from HTMLParser import HTMLParser


_log = logging.getLogger(__name__)


def convert_for_webkit(new_path, filename, reference_support_info, host=Host()):
    """Converts a file's contents so the Blink layout test runner can run it.

    Returns:
      A pair: the list of modified properties, and the modified text if the file was modified, None otherwise.
    """
    contents = host.filesystem.read_binary_file(filename)
    converter = _W3CTestConverter(new_path, filename, reference_support_info, host)
    if filename.endswith('.css'):
        return converter.add_webkit_prefix_to_unprefixed_properties(contents.decode('utf-8'))
    else:
        try:
            converter.feed(contents.decode('utf-8'))
        except UnicodeDecodeError:
            converter.feed(contents.decode('utf-16'))
        converter.close()
        return converter.output()


class _W3CTestConverter(HTMLParser):
    """A HTMLParser subclass which converts a HTML file as it is parsed.

    After the feed() method is called, the converted document will be stored
    in converted_data, and can be retrieved with the output() method.
    """

    def __init__(self, new_path, filename, reference_support_info, host=Host()):
        HTMLParser.__init__(self)

        self._host = host
        self._filesystem = self._host.filesystem
        self._webkit_root = WebKitFinder(self._filesystem).webkit_base()

        self.converted_data = []
        self.converted_properties = []
        self.in_style_tag = False
        self.style_data = []
        self.filename = filename
        self.reference_support_info = reference_support_info
        resources_path = self.path_from_webkit_root('LayoutTests', 'resources')
        resources_relpath = self._filesystem.relpath(resources_path, new_path)
        self.resources_relpath = resources_relpath

        # These settings might vary between WebKit and Blink.
        self._css_property_file = self.path_from_webkit_root('Source', 'core', 'css', 'CSSProperties.in')
        self.prefixed_properties = self.read_webkit_prefixed_css_property_list()
        prop_regex = r'([\s{]|^)(' + '|'.join(
            prop.replace('-webkit-', '') for prop in self.prefixed_properties) + r')(\s+:|:)'
        self.prop_re = re.compile(prop_regex)

    def output(self):
        return (self.converted_properties, ''.join(self.converted_data))

    def path_from_webkit_root(self, *comps):
        return self._filesystem.abspath(self._filesystem.join(self._webkit_root, *comps))

    def read_webkit_prefixed_css_property_list(self):
        prefixed_properties = []
        unprefixed_properties = set()

        contents = self._filesystem.read_text_file(self._css_property_file)
        for line in contents.splitlines():
            if re.match(r'^(#|//|$)', line):
                # skip comments and preprocessor directives.
                continue
            prop = line.split()[0]
            # Find properties starting with the -webkit- prefix.
            match = re.match(r'-webkit-([\w|-]*)', prop)
            if match:
                prefixed_properties.append(match.group(1))
            else:
                unprefixed_properties.add(prop.strip())

        # Ignore any prefixed properties for which an unprefixed version is supported.
        return [prop for prop in prefixed_properties if prop not in unprefixed_properties]

    def add_webkit_prefix_to_unprefixed_properties(self, text):
        """Searches |text| for instances of properties requiring the -webkit- prefix and adds the prefix to them.

        Returns the list of converted properties and the modified text.
        """
        converted_properties = set()
        text_chunks = []
        cur_pos = 0
        for m in self.prop_re.finditer(text):
            text_chunks.extend([text[cur_pos:m.start()], m.group(1), '-webkit-', m.group(2), m.group(3)])
            converted_properties.add(m.group(2))
            cur_pos = m.end()
        text_chunks.append(text[cur_pos:])

        for prop in converted_properties:
            _log.info('  converting %s', prop)

        # FIXME: Handle the JS versions of these properties and GetComputedStyle, too.
        return (converted_properties, ''.join(text_chunks))

    def convert_reference_relpaths(self, text):
        """Converts reference file paths found in the given text.

        Searches |text| for instances of files in |self.reference_support_info| and
        updates the relative path to be correct for the new ref file location.
        """
        converted = text
        for path in self.reference_support_info['files']:
            if path in text:
                # FIXME: This doesn't handle an edge case where simply removing the relative path doesn't work.
                # See crbug.com/421584 for details.
                new_path = re.sub(self.reference_support_info['reference_relpath'], '', path, 1)
                converted = re.sub(path, new_path, text)

        return converted

    def convert_style_data(self, data):
        converted = self.add_webkit_prefix_to_unprefixed_properties(data)
        if converted[0]:
            self.converted_properties.extend(list(converted[0]))

        if self.reference_support_info is None or self.reference_support_info == {}:
            return converted[1]

        return self.convert_reference_relpaths(converted[1])

    def convert_attributes_if_needed(self, tag, attrs):
        """Converts attributes in a start tag in HTML.

        The converted tag text is appended to |self.converted_data|.
        """
        converted = self.get_starttag_text()
        for attr_name, attr_value in attrs:
            if attr_name == 'style':
                new_style = self.convert_style_data(attr_value)
                converted = re.sub(re.escape(attr_value), new_style, converted)

        src_tags = ('script', 'img', 'style', 'frame', 'iframe', 'input', 'layer', 'textarea', 'video', 'audio')
        if tag in src_tags and self.reference_support_info is not None and self.reference_support_info != {}:
            for attr_name, attr_value in attrs:
                if attr_name == 'src':
                    new_path = self.convert_reference_relpaths(attr_value)
                    converted = re.sub(re.escape(attr_value), new_path, converted)

        self.converted_data.append(converted)

    def parse_endtag(self, i):
        # parse_endtag is being overridden here instead of handle_endtag
        # so we can get the original end tag text with the original
        # capitalization
        endpos = HTMLParser.parse_endtag(self, i)
        self.converted_data.extend([self.rawdata[i:endpos]])
        return endpos

    def handle_starttag(self, tag, attrs):
        if tag == 'style':
            self.in_style_tag = True
        self.convert_attributes_if_needed(tag, attrs)

    def handle_endtag(self, tag):
        if tag == 'style':
            self.converted_data.append(self.convert_style_data(''.join(self.style_data)))
            self.in_style_tag = False
            self.style_data = []

    def handle_startendtag(self, tag, attrs):
        self.convert_attributes_if_needed(tag, attrs)

    def handle_data(self, data):
        if self.in_style_tag:
            self.style_data.append(data)
        else:
            self.converted_data.append(data)

    def handle_entityref(self, name):
        self.converted_data.extend(['&', name, ';'])

    def handle_charref(self, name):
        self.converted_data.extend(['&#', name, ';'])

    def handle_comment(self, data):
        self.converted_data.extend(['<!--', data, '-->'])

    def handle_decl(self, decl):
        self.converted_data.extend(['<!', decl, '>'])

    def handle_pi(self, data):
        self.converted_data.extend(['<?', data, '>'])
