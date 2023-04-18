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
import QtQuick.Layouts 1.3

Rectangle {
    DesktopBackground {
        needBlur: true
        radiusNum: 48
    }

    RowLayout {
        width: 240
        height: 120
        anchors.centerIn: parent
        spacing: 140

        Repeater {
            model: [
                { buttonType: 0, imageSource: "/res/bottom_actions/reboot.svg", textContent: qsTr("Reboot") },
                { buttonType: 1, imageSource: "/res/bottom_actions/shutdown.svg", textContent: qsTr("Shutdown") }
            ]

            ShutdownAndRebootButton {}
        }
    }
}
