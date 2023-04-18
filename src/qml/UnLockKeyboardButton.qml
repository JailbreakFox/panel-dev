/*
   * Copyright (C) 2020 ~ 2021 Uniontech Software Technology Co.,Ltd.
   *
   * Author:     xuyanghe <xuyanghe@uniontech.com>
   *
   * Maintainer: xuyanghe <xuyanghe@uniontech.com>
   *
   * This program is free software: you can redistribute it and/or modify
   * it under the terms of the GNU General Public License as published by
   * the Free Software Foundation, either version 3 of the License, or
   * any later version.
   *
   * This program is distributed in the hope that it will be useful,
   * but WITHOUT ANY WARRANTY; without even the implied warranty of
   * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   * GNU General Public License for more details.
   *
   * You should have received a copy of the GNU General Public License
   * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

import QtQuick 2.11
import QtQuick.Controls 2.4
import com.deepin.dtk 1.0

Item {
    width: 104
    height: 104

    RoundButton {
        id: unLockKeyboardButton

        width: 84
        height: 84
        anchors {
            horizontalCenter: parent.horizontalCenter
            verticalCenter: parent.verticalCenter
        }
        radius: width / 2
        opacity: modelData.opacityValue

        onPressed: {
            unLockKeyboardButton.opacity = 0.4
        }

        onReleased: {
            if (modelData.buttonStyle === 0) {
                unLockKeyboardButton.opacity = 0.15
                SessionShellController.requestPasswordUnitAdd(modelData.textContent)
            } else if (modelData.buttonStyle === 1) {
                unLockKeyboardButton.opacity = 0
                SessionShellController.requestBackToLock()
            } else {
                unLockKeyboardButton.opacity = 0
                SessionShellController.requestPasswordUnitDelete()
            }
        }
    }

    Text {
        text: modelData.textContent
        color: "white"
        anchors.horizontalCenter: unLockKeyboardButton.horizontalCenter
        anchors.top: unLockKeyboardButton.top
        anchors.topMargin: modelData.buttonStyle === 0 ? 12 : 30
        font.pixelSize: modelData.buttonStyle === 0 ? 30 : 14
    }

    Text {
        text: modelData.textChildContent
        color: "white"
        anchors {
            horizontalCenter: unLockKeyboardButton.horizontalCenter
            top: unLockKeyboardButton.top
            topMargin: 52
        }
        font {
            pixelSize: 11
            weight: Font.Thin
        }
        opacity: 0.7
    }
}
