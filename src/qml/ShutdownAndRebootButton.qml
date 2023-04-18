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
    /*! 用户按下的按钮类型 0: 重启 1: 关机 */
    property int rebootButtonType: 0
    property int shutdownButtonType: 1

    width: 84
    height: 84

    RoundButton {
        id: button

        width: 84
        height: 84
        anchors.centerIn: parent.Center
        radius: width / 2
        opacity: 0.1

        onPressed: {
            button.opacity = 0.5
        }

        onReleased: {
            button.opacity = 0.1
            if (modelData.buttonType === rebootButtonType) {
                SessionShellController.requestReboot()
            } else if (modelData.buttonType === shutdownButtonType) {
                SessionShellController.requestShutdown()
            }
        }
    }

    Image {
        anchors {
            horizontalCenter: parent.horizontalCenter
            verticalCenter: parent.verticalCenter
        }
        width: parent.width - 50
        height: parent.height - 50
        source: modelData.imageSource
    }

    Text {
        text: modelData.textContent
        color: "white"
        anchors {
            horizontalCenter: parent.horizontalCenter
            top: button.bottom
            topMargin: 21
        }
        font.pixelSize: 17
        opacity: 0.8
    }
}
