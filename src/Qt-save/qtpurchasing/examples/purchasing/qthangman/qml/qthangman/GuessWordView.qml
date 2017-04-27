/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the Purchasing module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL3-COMM$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPLv3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 2.2
import QtQuick.Controls 1.1

Item {
    id: dialog
    PageHeader {
        id: header
        title: "What is the word?"
        onClicked: {
            Qt.inputMethod.hide();
        }
    }

    Word {
        id: word
        text: applicationData.word
        anchors.top: header.bottom
        anchors.bottomMargin: parent.height * 0.05
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width * 0.8
        height: parent.height * 0.1
    }

    function guess() {
        applicationData.guessWord(input.text)
        input.text = ""
        Qt.inputMethod.hide();
        pageStack.pop();
        topLevel.forceActiveFocus()
    }

    TextField {
        id: input
        font.capitalization: Font.AllUppercase
        inputMethodHints: Qt.ImhLatinOnly | Qt.ImhUppercaseOnly | Qt.ImhNoPredictiveText
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        focus: true
        maximumLength: applicationData.word.length
        anchors.top: word.bottom
        anchors.right: word.right
        anchors.left: word.left
        height: word.height
        anchors.topMargin: topLevel.globalMargin * 2
        font.pixelSize: height * 0.75
        onAccepted: {
            dialog.guess();
        }
    }

    Component.onCompleted: {
        Qt.inputMethod.show();
        input.forceActiveFocus();
    }
}
