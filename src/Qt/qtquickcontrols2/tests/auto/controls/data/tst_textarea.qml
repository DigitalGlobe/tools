/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the test suite of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 2.2
import QtTest 1.0
import QtQuick.Controls 2.0

TestCase {
    id: testCase
    width: 400
    height: 400
    visible: true
    when: windowShown
    name: "TextArea"

    Component {
        id: textArea
        TextArea { background: Item { } }
    }

    Component {
        id: flickable
        Flickable {
            width: 200
            height: 200
            TextArea.flickable: TextArea { }
        }
    }

    Component {
        id: signalSpy
        SignalSpy { }
    }

    function test_creation() {
        var control = textArea.createObject(testCase)
        verify(control)
        control.destroy()
    }

    function test_implicitSize() {
        var control = textArea.createObject(testCase)

        var implicitWidthSpy = signalSpy.createObject(control, { target: control, signalName: "implicitWidthChanged"} )
        var implicitHeightSpy = signalSpy.createObject(control, { target: control, signalName: "implicitHeightChanged"} )
        control.background.implicitWidth = 400
        control.background.implicitHeight = 200
        compare(control.implicitWidth, 400)
        compare(control.implicitHeight, 200)
        compare(implicitWidthSpy.count, 1)
        compare(implicitHeightSpy.count, 1)

        control.destroy()
    }

    function test_alignment() {
        var control = textArea.createObject(testCase)

        control.horizontalAlignment = TextArea.AlignRight
        compare(control.horizontalAlignment, TextArea.AlignRight)
        for (var i = 0; i < control.children.length; ++i) {
            if (control.children[i].hasOwnProperty("horizontalAlignment"))
                compare(control.children[i].horizontalAlignment, Text.AlignRight) // placeholder
        }

        control.verticalAlignment = TextArea.AlignBottom
        compare(control.verticalAlignment, TextArea.AlignBottom)
        for (var j = 0; j < control.children.length; ++j) {
            if (control.children[j].hasOwnProperty("verticalAlignment"))
                compare(control.children[j].verticalAlignment, Text.AlignBottom) // placeholder
        }

        control.destroy()
    }

    function test_font_explicit_attributes_data() {
        return [
            {tag: "bold", value: true},
            {tag: "capitalization", value: Font.Capitalize},
            {tag: "family", value: "Courier"},
            {tag: "italic", value: true},
            {tag: "strikeout", value: true},
            {tag: "underline", value: true},
            {tag: "weight", value: Font.Black},
            {tag: "wordSpacing", value: 55}
        ]
    }

    function test_font_explicit_attributes(data) {
        var control = textArea.createObject(testCase)
        verify(control)

        var child = textArea.createObject(control)
        verify(child)

        var controlSpy = signalSpy.createObject(control, {target: control, signalName: "fontChanged"})
        verify(controlSpy.valid)

        var childSpy = signalSpy.createObject(child, {target: child, signalName: "fontChanged"})
        verify(childSpy.valid)

        var defaultValue = control.font[data.tag]
        child.font[data.tag] = defaultValue

        compare(child.font[data.tag], defaultValue)
        compare(childSpy.count, 0)

        control.font[data.tag] = data.value

        compare(control.font[data.tag], data.value)
        compare(controlSpy.count, 1)

        compare(child.font[data.tag], defaultValue)
        compare(childSpy.count, 0)

        control.destroy()
    }

    function test_flickable() {
        var control = flickable.createObject(testCase, {text:"line0"})
        verify(control)

        var textArea = control.TextArea.flickable
        verify(textArea)

        for (var i = 1; i <= 100; ++i)
            textArea.text += "line\n" + i

        verify(textArea.contentWidth > 0)
        verify(textArea.contentHeight > 200)

        compare(control.contentWidth, textArea.contentWidth + textArea.leftPadding + textArea.rightPadding)
        compare(control.contentHeight, textArea.contentHeight + textArea.topPadding + textArea.bottomPadding)

        control.destroy()
    }

    function test_warning() {
        ignoreWarning(Qt.resolvedUrl("tst_textarea.qml") + ":45:1: QML TestCase: TextArea must be attached to a Flickable")
        testCase.TextArea.flickable = null
    }
}
