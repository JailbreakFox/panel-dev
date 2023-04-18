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

#ifndef SESSIONSHELLCONTROLLER_H
#define SESSIONSHELLCONTROLLER_H

#include "enumtype.h"

#include <QObject>
#include <QTimer>

class SessionBaseModel;
class SessionShellController : public QObject
{
    Q_OBJECT
public:
    SessionShellController(SessionBaseModel *const model, QObject *parent = nullptr);
    Q_INVOKABLE void setBackToLockTimer(int modeStatus, bool isStart);

Q_SIGNALS:
    Q_INVOKABLE void requestShow(EnumType::ModeStatus modeStatus);
    Q_INVOKABLE void requestReboot();
    Q_INVOKABLE void requestShutdown();
    Q_INVOKABLE void requestStartAuth();
    Q_INVOKABLE void requestInitial();
    /**
    * @brief SessionShellController::requestBackToLock 返回锁屏界面
    */
    Q_INVOKABLE void requestBackToLock();
    Q_INVOKABLE void requestPasswordUnitAdd(QString str);
    Q_INVOKABLE void requestPasswordUnitDelete();

    void authTipsMessage(const QString &message);
    void authFaildMessage(const QString &message);
    void authFinished(bool success);

private:
    SessionBaseModel *m_model;
    QTimer *lockTimer;
    QTimer *shutdownTimer;
};

#endif // SESSIONSHELLCONTROLLER_H
