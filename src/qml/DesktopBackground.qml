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
import QtGraphicalEffects 1.0

Item {
    anchors.fill: parent

    property int radiusNum: 0
    property bool needBlur: false

    Image {
        id: desktopBackgroundPic

        anchors.fill: parent
        source: SessionBaseModel.desktopBackground
    }

    Component {
        id: blurComponent

        FastBlur {
           source: desktopBackgroundPic
           radius: radiusNum
        }
    }

    Loader {
        anchors.fill: parent
        sourceComponent: needBlur ? blurComponent : undefined
    }
}
