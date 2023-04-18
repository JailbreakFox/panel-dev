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

#include "sessionshellcontroller.h"

SessionShellController::SessionShellController(SessionBaseModel *const model, QObject *parent)
    : QObject(parent)
    , m_model(model)
    , lockTimer(new QTimer(this))
    , shutdownTimer(new QTimer(this))
{
    lockTimer->setInterval(5000);
    lockTimer->setSingleShot(true);
    shutdownTimer->setInterval(5000);
    shutdownTimer->setSingleShot(true);

    connect(lockTimer, &QTimer::timeout, this, [ = ] {
        lockTimer->stop();
        shutdownTimer->stop();
        Q_EMIT requestBackToLock();
    });
    connect(shutdownTimer, &QTimer::timeout, this, [ = ] {
        lockTimer->stop();
        shutdownTimer->stop();
        Q_EMIT requestBackToLock();
    });
}

/**
 * @brief SessionShellController::setBackToLockTimer 开启或关闭切换回锁屏界面的定时器
 * @param modeStatus 代表当前的界面状态
 * @param isStart 代表当前需要关闭/开启定时器
 */
void SessionShellController::setBackToLockTimer(int modeStatus, bool isStart)
{
    if (modeStatus == EnumType::ModeStatus::NoStatus) {
        if (isStart)
            lockTimer->start();
        else
            lockTimer->stop();
    } else if(modeStatus == EnumType::ModeStatus::PowerMode) {
        if (isStart)
            shutdownTimer->start();
        else
            shutdownTimer->stop();
    }
}
