/*
   * Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
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

import QtQuick 2.0

Component {
    Canvas {
        id: passwordCanvas

        property int roundRadius: 6

        width: 42
        height: 42

        onPaint: {
            var ctx = getContext("2d")
            ctx.lineWidth = 2

            ctx.fillStyle = "white"
            ctx.strokeStyle = "white"
            ctx.lineWidth = 1.2

            // 每次重新绘制前清除画布
            ctx.clearRect(0, 0, passwordCanvas.width, passwordCanvas.height)

            ctx.beginPath()
            ctx.arc(21, 21, roundRadius, 0, -2 * Math.PI * roundRadius, true)
            index < SessionBaseModel.passwordLength ? ctx.fill() : ctx.stroke()
        }

        Connections {
            target: SessionBaseModel
            onPasswordLengthChanged: {
                // 当密码长度改变时,请求重新绘制圆点
                passwordCanvas.requestPaint()
            }
        }
    }
}