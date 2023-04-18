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

Item {
    width: 42
    height: 42

    Rectangle {
        id: circle

        width: 12
        height: 12
        anchors.centerIn: parent
        radius: width / 2
        color: "transparent"
        border.color: "white"
    }

    Connections {
        target: SessionBaseModel

        onPasswordLengthChanged: {
            // 当密码长度改变时,请求重新绘制圆点
            if (index < SessionBaseModel.passwordLength)
                circle.color = "white"
            else
                circle.color = "transparent"
        }
    }
}
