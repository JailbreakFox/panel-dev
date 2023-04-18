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
    visible: SessionBaseModel.isScreenBlackLock

    DesktopBackground {
        needBlur: true
        radiusNum: 64
    }

    Item {
       id: unLockTimeAndDataItem

       width: 150
       height: 80
       anchors {
           top: parent.top
           topMargin: 30
           horizontalCenter: parent.horizontalCenter
       }

       Column {
           anchors.fill: parent

           Label {
               anchors.horizontalCenter: parent.horizontalCenter
               text: SessionBaseModel.time
               font.pixelSize: 40
               color: "white"
           }

           Label {
               anchors.horizontalCenter: parent.horizontalCenter
               text: SessionBaseModel.date
               font.pixelSize: 14
               color: "white"
           }
       }
    }

    LockStatus {
       id: lockStatusItem

       anchors {
           top: unLockTimeAndDataItem.bottom
           topMargin: 91
           horizontalCenter: parent.horizontalCenter
       }
        currentFrame: 1
    }

    Item {
        id: unLockCirclesItem

        width: 252
        height: 42
        anchors {
            horizontalCenter: parent.horizontalCenter
            top: lockStatusItem.bottom
        }

        ListView {
            anchors.fill: parent
            orientation:ListView.Horizontal
            delegate: UnLockCircles { }

            model: ListModel {
                id: unLockCanvasModel
                ListElement{ index: 0 }
                ListElement{ index: 1 }
                ListElement{ index: 2 }
                ListElement{ index: 3 }
                ListElement{ index: 4 }
                ListElement{ index: 5 }
            }
        }

        // 认证失败的动画
        Loader {
            id: authFailAni

            onLoaded: {
                item.targetName = unLockCirclesItem
                item.targetProperty = "anchors.horizontalCenterOffset"
                item.aniMaxVal = 10
                item.durVal = 50
                item.loopVal = 4
            }
        }

        Connections {
            target: SessionShellController

            onAuthFinished: {
                if (!success) {
                    // 播放认证失败动画
                    authFailAni.source = "AuthFailAni.qml"
                    authFailAni.item.start()
                }
            }
        }
    }

    UnLockKeyboard {}
}
