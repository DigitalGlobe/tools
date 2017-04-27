"""
/******************************************************************************
 * $Id$
 *
 * Project:  libLAS - http://liblas.org - A BSD library for LAS format data.
 * Purpose:  Python LASFile implementation
 * Author:   Howard Butler, hobu.inc@gmail.com
 *
 ******************************************************************************
 * Copyright (c) 2009, Howard Butler
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following
 * conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in
 *       the documentation and/or other materials provided
 *       with the distribution.
 *     * Neither the name of the Martin Isenburg or Iowa Department
 *       of Natural Resources nor the names of its contributors may be
 *       used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 ****************************************************************************/
 """

from . import core
from . import header as lasheader
from . import point

import os
import types

files = {'append': [], 'write': [], 'read': {}}
import sys


class File(object):
    def __init__(self, filename,
                       header=None,
                       mode='r',
                       in_srs=None,
                       out_srs=None):
        """Instantiate a file object to represent an LAS file.

        :arg filename: The filename to open
        :keyword header: A header open the file with
        :type header: an :obj:`liblas.header.header.Header` instance
        :keyword mode: "r" for read, "w" for write, and "w+" for append
        :type mode: string
        :keyword in_srs: Input SRS to override the existing file's SRS with
        :type in_srs: an :obj:`liblas.srs.SRS` instance
        :keyword out_srs: Output SRS to reproject points on-the-fly to as \
        they are read/written.
        :type out_srs: an :obj:`liblas.srs.SRS` instance

        .. note::
            To open a file in write mode, you must provide a
            liblas.header.Header instance which will be immediately written to
            the file. If you provide a header instance in read mode, the
            values of that header will be used in place of those in the actual
            file.

        .. note::
            If a file is open for write, it cannot be opened for read and vice
            versa.

        >>> from liblas import file
        >>> f = file.File('file.las', mode='r')
        >>> for p in f:
        ...     print 'X,Y,Z: ', p.x, p.y, p.z

        >>> h = f.header
        >>> f2 = file.File('file2.las', header=h)
        >>> for p in f:
        ...     f2.write(p)
        >>> f2.close()
        """
        if sys.version_info.major == 3:
            self.filename = bytes(os.path.abspath(filename), "ascii")
        else:
            self.filename = filename
        self._header = None
        self.ownheader = True

        # import pdb;pdb.set_trace()
        if header != None:

            self.ownheader = False
            self._header = header.handle

        self.handle = None
        self._mode = mode.lower()
        self.in_srs = in_srs
        self.out_srs = out_srs


        #check in the registry if we already have the file open
        if mode == 'r':
            for f in files['write'] + files['append']:
                if f == self.filename:
                    raise core.LASException("File %s is already open for "
                                            "write.  Close the file or delete "
                                            "the reference to it" % filename)
        else:
            # we're in some kind of write mode, and if we already have the
            # file open, complain to the user.
            for f in list(files['read'].keys()) + files['append'] + files['write']:
                if f == self.filename:
                    raise core.LASException("File %s is already open. "
                                            "Close the file or delete the "
                                            "reference to it" % filename)
        self.open()

    def open(self):
        """Open the file for processing, called by __init__
        """
        
        if self._mode == 'r' or self._mode == 'rb':
            
            if not os.path.exists(self.filename):
                raise OSError("No such file or directory: '%s'" % self.filename)

            if self._header == None:
                self.handle = core.las.LASReader_Create(self.filename)
                self._header = core.las.LASReader_GetHeader(self.handle)
            else:
                self.handle = \
                core.las.LASReader_CreateWithHeader(self.filename,
                                                        self._header)
            core.las.LASHeader_Destroy(self._header)
            self._header = core.las.LASReader_GetHeader(self.handle)
            self.mode = 0
            try:
                files['read'][self.filename] += 1
            except KeyError:
                files['read'][self.filename] = 1

            if self.in_srs:
                core.las.las.LASReader_SetInputSRS(self.handle,
                                                   self.in_srs.handle)
            if self.out_srs:
                core.las.LASReader_SetOutputSRS(self.handle,
                                                self.out_srs.handle)

        if self._mode == 'w' and '+' not in self._mode:

            if self._header == None:
                self._header = core.las.LASHeader_Create()

            self.handle = core.las.LASWriter_Create(self.filename,
                                                    self._header,
                                                    1)
            core.las.LASHeader_Destroy(self._header)
            self._header = core.las.LASWriter_GetHeader(self.handle)
            self.mode = 1
            files['write'].append(self.filename)

            if self.in_srs:
                core.las.LASWriter_SetInputSRS(self.handle, self.in_srs.handle)
            if self.out_srs:
                core.las.LASWriter_SetOutputSRS(self.handle,
                                                self.out_srs.handle)

        if '+' in self._mode and 'r' not in self._mode:
            if self._header == None:
                reader = core.las.LASReader_Create(self.filename)
                self._header = core.las.LASReader_GetHeader(reader)
                core.las.LASReader_Destroy(reader)
            self.handle = core.las.LASWriter_Create(self.filename,
                                                    self._header,
                                                    2)
            core.las.LASHeader_Destroy(self._header)
            self._header = core.las.LASWriter_GetHeader(self.handle)
            self.mode = 2
            files['append'].append(self.filename)

            if self.in_srs:
                core.las.LASWriter_SetInputSRS(self.handle, self.in_srs.handle)
            if self.out_srs:
                core.las.LASWriter_SetOutputSRS(self.handle,
                                                self.out_srs.handle)

    def __del__(self):
        try:
            self.handle
        except AttributeError:
            return
        if not self.handle or not core:
            return
        self.close()

    def close(self):
        """Closes the LAS file
        """
        if self.mode == 0:
            try: 
                files['read'][self.filename] -= 1
                if files['read'][self.filename] == 0:
                    files['read'].pop(self.filename)
            except KeyError:
                raise core.LASException("File %s was not found in accounting dictionary!" % self.filename)

            core.las.LASReader_Destroy(self.handle)
        else:
            try:
                files['append'].remove(self.filename)
            except:
                files['write'].remove(self.filename)
            core.las.LASWriter_Destroy(self.handle)
        
        if (self._header):
            core.las.LASHeader_Destroy(self._header)
            
        self._header = None
        self.handle = None

    def set_srs(self, value):

        return self.set_output_srs(value)

    def set_output_srs(self, value):
        if self.mode == 0:
            return core.las.LASReader_SetOutputSRS(self.handle, value.handle)
        else:
            return core.las.LASWriter_SetOutputSRS(self.handle, value.handle)

    def get_output_srs(self):
        return self.out_srs

    doc = """The output :obj:`liblas.srs.SRS` for the file.  Data will be
    reprojected to this SRS according to either the :obj:`input_srs` if it
    was set or default to the :obj:`liblas.header.Header.SRS` if it was
    not set.  The header's SRS must be valid and exist for reprojection
    to occur. GDAL support must also be enabled for the library for
    reprojection to happen."""
    output_srs = property(get_output_srs, set_output_srs, None, doc)

    def set_input_srs(self, value):
        if self.mode == 0:
            return core.las.LASReader_SetInputSRS(self.handle, value.handle)
        else:
            return core.las.LASWriter_SetInputSRS(self.handle, value.handle)

    def get_input_srs(self):
        return self.in_srs
    doc = """The input :obj:`liblas.srs.SRS` for the file.  This overrides the
    :obj:`liblas.header.Header.SRS`.  It is useful in cases where the header's
    SRS is not valid or does not exist."""
    input_srs = property(get_input_srs, set_input_srs, None, doc)

    def get_header(self):
        """Returns the liblas.header.Header for the file"""
        if not self.handle:
            return None
        
        if self.mode == 0:
            return lasheader.Header(handle=core.las.LASReader_GetHeader(self.handle), owned=True)
        else:
            return lasheader.Header(handle=core.las.LASWriter_GetHeader(self.handle), owned=True)

        return None

    def set_header(self, header):
        """Sets the liblas.header.Header for the file.  If the file is in \
        append mode, the header will be overwritten in the file."""
        # append mode
        if mode == 2:
            core.las.LASWriter_Destroy(self.handle)
            self.handle = core.las.LASWriter_Create(self.handle, header, 2)
            self._header = core.las.LASHeader_Copy(header.handle)
            return True
        raise core.LASException("The header can only be set "
                                "after file creation for files in append mode")
    doc = """The file's :obj:`liblas.header.Header`

    .. note::
        If the file is in append mode, the header will be overwritten in the
        file. Setting the header for the file when it is in read mode has no
        effect. If you wish to override existing header information with your
        own at read time, you must instantiate a new :obj:`liblas.file.File`
        instance.

    """
    header = property(get_header, set_header, None, doc)

    def read(self, index):
        """Reads the point at the given index"""
        if self.mode == 0:
            p = point.Point(
            handle=core.las.LASReader_GetPointAt(self.handle, index),
            copy=True)
            p.set_header(lasheader.Header(handle=self._header, copy=False))
            return p
            
    def seek(self, index):
        """Seeks to the point at the given index.  Subsequent calls \
	   to :meth:`next` will then start at that point."""
        if self.mode == 0:
            return core.las.LASReader_Seek(self.handle, index)

    def __iter__(self):
        """Iterator support (read mode only)

          >>> points = []
          >>> for i in f:
          ...   points.append(i)
          ...   print i # doctest: +ELLIPSIS
          <liblas.point.Point object at ...>
        """
        if self.mode == 0:
            self.at_end = False
            p = core.las.LASReader_GetNextPoint(self.handle)
            while p and not self.at_end:
                p2 = point.Point(handle=p, copy=True)
                p2.set_header(lasheader.Header(handle=self._header, copy=False))
                yield p2
                p = core.las.LASReader_GetNextPoint(self.handle)
                if not p:
                    self.at_end = True
            else:
                self.close()
                self.open()

    def __getitem__(self, index):
        """Index and slicing support

          >>> out = f[0:3]
          [<liblas.point.Point object at ...>,
          <liblas.point.Point object at ...>,
          <liblas.point.Point object at ...>]
        """
        try:
            index.stop
        except AttributeError:
            return self.read(index)

        output = []
        if index.step:
            step = index.step
        else:
            step = 1
        for i in range(index.start, index.stop, step):
            output.append(self.read(i))

        return output

    def __len__(self):
        """Returns the number of points in the file according to the header"""
        return self.header.point_records_count

    def write(self, pt):
        """Writes the point to the file if it is append or write mode. LAS
        files are written sequentially starting from the first point (in pure
        write mode) or from the last point that exists (in append mode).

        :param pt: The point to write.
        :type pt: :obj:`liblas.point.Point` instance to write

        .. note::
            At this time, it is not possible to duck-type point objects and
            have them be written into the LAS file (from say numpy or
            something). You have to take care of this adaptation yourself.

        """
        if not isinstance(pt, point.Point):
            raise core.LASException('cannot write %s, it must '
                                    'be of type liblas.point.Point' % pt)
        if self.mode == 1 or self.mode == 2:
            core.las.LASWriter_WritePoint(self.handle, pt.handle)
            
    def get_xmlsummary(self):
        """Returns an XML string summarizing all of the points in the reader
        
        .. note::
            This method will reset the reader's read position to the 0th 
            point to summarize the entire file, and it will again reset the 
            read position to the 0th point upon completion."""
        if self.mode != 0:
            raise core.LASException("file must be in read mode, not append or write mode to provide xml summary")
        return  core.las.LASReader_GetSummaryXML(self.handle)
        
    summary = property(get_xmlsummary, None, None, None)