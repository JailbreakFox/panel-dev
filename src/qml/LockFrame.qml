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
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import com.deepin.dtk 1.0

Rectangle {
    DesktopBackground {}

    Item {
        width: 204
        height: 159
        anchors {
            top: parent.top
            topMargin: 60
            horizontalCenter: parent.horizontalCenter
        }

        Column {
            anchors.fill: parent

            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                text: SessionBaseModel.time
                // 最大的时间，字体不跟随系统
                font {
                    family: "Times"
                    bold: false
                    pixelSize: 88
                }
                color: "white"
            }

            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                text: SessionBaseModel.date
                font.pixelSize: 20
                color: "white"
            }
        }
    }

    Rectangle {
        id: messagesRect

        width: 480
        height: 700
        anchors {
            top: parent.top
            topMargin: 250
            horizontalCenter: parent.horizontalCenter
        }
        color: "grey"
        opacity: 0.2
    }

    LockStatus {
        anchors {
            top: messagesRect.bottom
            topMargin: 20
            horizontalCenter: parent.horizontalCenter
        }
        currentFrame: 0
    }
}
