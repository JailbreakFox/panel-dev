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
import QtQuick.Layouts 1.3

// 滑屏动画
Item {
    anchors.fill: parent

    property StackLayout layoutName
    property int pressY: 0
    property int pressChangedY: 0
    property int lastTimeY: 0
    property bool alreadyBack: false

    function init() {
        pressY = 0
        pressChangedY = 0
        lastTimeY = 0
        alreadyBack = false
    }

    // 用于在滑屏动画结束时隐藏进程
    Timer {
        id: quitTimer

        interval: 10 //定时周期
        repeat: false

        onTriggered: {
            SessionBaseModel.setIsShow(false)
            SessionShellController.requestInitial()
        }
    }

    // 手指释放动画
    YAnimator {
        id: aniPullingBack

        target: layoutName
        from: layoutName.y
        to: pressChangedY
        easing.type: Easing.InOutCubic
        duration: 250
        running: false
    }

    MouseArea {
        anchors.fill: parent
        enabled: !SessionBaseModel.isScreenBlackLock  // 只有控制中心取消"唤醒显示器时需要密码"才能实现滑动动画

        property int lastTimeChangedY: 0

        onPressed: {
            if (alreadyBack)
                return
            lastTimeY = mouseY
            pressY = mouseY
        }

        onMouseYChanged: {
            if (alreadyBack)
                return
            lastTimeChangedY = mouseY - lastTimeY
            lastTimeY = mouseY
            pressChangedY = mouseY - pressY
            layoutName.y = pressChangedY > 0 ? 0 : pressChangedY
        }

        onReleased: {
            if (alreadyBack === true)
                return

            if (lastTimeChangedY > 15) {
                alreadyBack = false
                pressChangedY = 0
            } else if (lastTimeChangedY < -15) {
                alreadyBack = false
                pressChangedY = -layoutName.height
            } else {
                pressChangedY = layoutName.y > - layoutName.height/2 ? 0 : -layoutName.height
            }

            aniPullingBack.start()
        }
    }

    Connections {
        target: aniPullingBack

        onStopped: {
            if (layoutName.y === -layoutName.height)
                quitTimer.start()
            else
                alreadyBack = false
        }
    }
}
