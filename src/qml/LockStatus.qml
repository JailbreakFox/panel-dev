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

Item {
    /*! 传输被实例化的时候，是在什么界面 0: 锁定界面 1: 解锁界面 */
    property int currentFrame

    width: 150
    height: 80

    Column {
        anchors.fill: parent

        AnimatedImage {
            id: lockButtonImage;

            width: 32
            height: 32
            rotation: 0
            anchors.horizontalCenter: parent.horizontalCenter
            source: "/res/lock/lock.apng"
            currentFrame: 0
            speed: 3.0
            playing: false

            // 认证失败的动画
            Loader {
                id: lockAuthFailAni

                onLoaded: {
                    item.targetName = lockButtonImage
                    item.targetProperty = "rotation"
                    item.aniMaxVal = 30
                    item.durVal = 50
                    item.loopVal = 4
                }
            }

            Connections {
                target: SessionShellController

                // 初始化绑定
                onRequestInitial: {
                    lockButtonImage.playing = false
                    lockButtonImage.currentFrame = 0
                }

                onAuthFinished: {
                    if (!success && currentFrame === 0) {
                        // 播放认证失败动画
                        lockAuthFailAni.source = "AuthFailAni.qml"
                        lockAuthFailAni.item.start()
                    } else if (success)
                        // 播放认证成功动画
                        lockButtonImage.playing = true
                }
            }

            // TODO: 由于认证成功后，动画会循环播放两次，这里临时处理一下
            Connections {
                target: lockButtonImage

                onCurrentFrameChanged: {
                    if (lockButtonImage.currentFrame === (lockButtonImage.frameCount - 1)) {
                        lockButtonImage.playing = false
                    }
                }
            }
        }

        Text {
            id: lockText

            anchors.horizontalCenter: parent.horizontalCenter
            color: "white"
            text: SessionBaseModel.isScreenBlackLock ? "" : qsTr("Swipe up to unlock")
            font.pixelSize: 14
        }

        Connections {
            target: SessionShellController

            onAuthFaildMessage: {
                lockText.text = message
            }

            // 初始化绑定
            onRequestInitial: {
                lockText.text = SessionBaseModel.isScreenBlackLock ? "" : qsTr("Swipe up to unlock")
            }
        }
    }

    // 用户被锁定时,切换到锁定界面
    Connections {
        target: SessionBaseModel

        onIsLockChanged: {
            if (isLock) {
                lockText.text = qsTr("Please try again %1 minute(s) later").arg(SessionBaseModel.lockTime)
            }
        }
    }
}
