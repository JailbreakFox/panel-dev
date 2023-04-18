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
import QtQuick.Window 2.2
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

Window {
    visible: SessionBaseModel.isShow
    width: Screen.width
    height: Screen.height
    color: "transparent"
    title: "due-shell"
    flags: Qt.FramelessWindowHint

    StackLayout {
        id: mainLayout

        width: parent.width
        height: parent.height
        currentIndex: SessionBaseModel.currentModeState

        Rectangle {
            SwipeView {
                id: lockAndUnlockView

                orientation: Qt.Vertical
                anchors.fill: parent
                currentIndex: 0
                interactive: SessionBaseModel.isScreenBlackLock  // 只有控制中心点击"唤醒显示器时需要密码"才能切换到解锁界面

                // 锁屏界面
                LockFrame {}

                // 解锁界面
                UnLockFrame {}
            }
        }

        // 关机界面
        ShutdownFrame {}

        Connections {
            target: SessionBaseModel

            // 用户切换"唤醒显示器时需要密码"状态时,切换到锁定界面
            onIsScreenBlackLockChanged: {
                mainLayout.y = 0
                lockAndUnlockView.interactive = false
                lockAndUnlockView.currentIndex = 0
                lockAndUnlockView.interactive = true
            }

            // 在锁屏界面被调用显示的情况下,开启认证
            onVisibleChanged: {
                // TODO: C++枚举类
                if (visible && SessionBaseModel.isScreenBlackLock && SessionBaseModel.currentModeState === 0) SessionShellController.requestStartAuth()
            }

            // 锁定状态切换时，切换回锁定界面
            onIsLockChanged: {
                mainLayout.y = 0
                if (isLock) {
                    lockAndUnlockView.interactive = false
                    lockAndUnlockView.currentIndex = 0
                } else {
                    lockAndUnlockView.interactive = true
                }
            }
        }
        Connections {
            target: SessionShellController

            // 初始化绑定
            onRequestInitial: {
                mainLayout.y = 0
                lockAndUnlockView.interactive = false
                lockAndUnlockView.setCurrentIndex(0)
                lockAndUnlockView.interactive = true
                swipeAni.init()
            }

            // 用户主动切换到锁定界面，或者在其他界面停留时间过长，需要返回锁定界面
            onRequestBackToLock: {
                SessionBaseModel.setCurrentModeState(0)
                SessionBaseModel.setIsShow(true)
                mainLayout.y = 0
                lockAndUnlockView.interactive = false
                lockAndUnlockView.currentIndex = 0
                lockAndUnlockView.interactive = true
            }
        }
    }

    // 滑屏动画
    SwipeAni {
        id: swipeAni
        layoutName: mainLayout
    }
}
