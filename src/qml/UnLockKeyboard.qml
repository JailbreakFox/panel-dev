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
import QtQuick.Layouts 1.11

Item {
    width: 312
    height: 416
    anchors { horizontalCenter: parent.horizontalCenter
        top: unLockCirclesItem.bottom
        topMargin: 65
    }

    GridLayout {
        rows: 4
        columns: 3
        rowSpacing: 0
        columnSpacing: 0
        anchors.fill: parent
        flow: GridLayout.LeftToRight

        Repeater {
            /*! buttonStyle表示按钮类型 0:按键0~9 1:返回键 2:清除键*/
            model: [
                { buttonStyle: 0, opacityValue: 0.15, textContent: "1", textChildContent: ""},
                { buttonStyle: 0, opacityValue: 0.15, textContent: "2", textChildContent: "ABC"},
                { buttonStyle: 0, opacityValue: 0.15, textContent: "3", textChildContent: "DEF"},
                { buttonStyle: 0, opacityValue: 0.15, textContent: "4", textChildContent: "GHI"},
                { buttonStyle: 0, opacityValue: 0.15, textContent: "5", textChildContent: "JKL"},
                { buttonStyle: 0, opacityValue: 0.15, textContent: "6", textChildContent: "MNO"},
                { buttonStyle: 0, opacityValue: 0.15, textContent: "7", textChildContent: "PQRS"},
                { buttonStyle: 0, opacityValue: 0.15, textContent: "8", textChildContent: "TUV"},
                { buttonStyle: 0, opacityValue: 0.15, textContent: "9", textChildContent: "WVWZ"},
                { buttonStyle: 1, opacityValue: 0, textContent: qsTr("Back"), textChildContent: ""},
                { buttonStyle: 0, opacityValue: 0.15, textContent: "0", textChildContent: "WVWZ"},
                { buttonStyle: 2, opacityValue: 0, textContent: qsTr("Clean"), textChildContent: ""}
            ]

            UnLockKeyboardButton {}
        }
    }
}
