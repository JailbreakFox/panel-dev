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

#ifndef SESSIONSHELLWORKER_H
#define SESSIONSHELLWORKER_H

#include "deepinauthinterface.h"
#include "enumtype.h"

#include <com_deepin_daemon_power.h>
#include <com_deepin_sessionmanager.h>
#include <com_deepin_daemon_timedate.h>

#include <libintl.h>
#include <unistd.h>

using PowerInter = com::deepin::daemon::Power;
using SessionManager = com::deepin::SessionManager;
using Timedate = com::deepin::daemon::Timedate;

class DeepinAuthFramework;
class SessionShellWorker : public DeepinAuthInterface
{
    Q_OBJECT
public:
    explicit SessionShellWorker(SessionBaseModel *const model, SessionShellController *const controller, QObject *parent = nullptr);

    void onDisplayErrorMsg(const QString &msg) override;
    void onDisplayTextInfo(const QString &msg) override;
    void onPasswordResult(const QString &msg) override;
    void authUser(const QString &password) override;

    Q_INVOKABLE void passwordUnitAdd(const QString passwordUnit);
    Q_INVOKABLE void passwordUnitDelete();
    bool isAuthenticate();

private:
    void initUser(const QString &user);
    /**
    * @brief SessionShellWorker::initTimer 时间定时器初始化
    */
    void initTimer();
    /**
    * @brief SessionShellWorker::refreshTimeAndDate 更新时间和日期
    */
    void refreshTimeAndDate();
    void setLongDateFormat(int type);
    void setShortTimeFormat(int type);
    void setUse24HourFormat(bool type);

private Q_SLOTS:
    void onShow(EnumType::ModeStatus modeStatus);
    void onUnlockFinished(bool unlocked);
    void onReboot();
    void onShutdown();

private:
    // 认证相关
    bool m_isThumbAuth;
    bool m_authenticating;
    DeepinAuthFramework *m_authFramework;
    PowerInter *m_powerInter;
    QString m_password;

    // 时间相关
    QTimer *m_refreshTimer;
    int m_longDateIndex;
    int m_shortTimeIndex;
    bool m_use24HourFormat;
    Timedate *m_timedateInter;

    // 重启与关机
    SessionManager *m_sessionInter;
};

#endif // SESSIONSHELLWORKER_H
