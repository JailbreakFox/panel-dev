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

#ifndef USERINFO_H
#define USERINFO_H

#include "public_func.h"
#include "constant.h"

#include <QObject>

#include <com_deepin_daemon_accounts_user.h>

#include <memory>

using UserInter = com::deepin::daemon::accounts::User;

class User : public QObject
{
    Q_OBJECT

public:
    User(const QString &path, QObject *parent);

    struct lockData {
        std::vector<uint> waitTime; //可设置的等待时间限制数组
        uint limitTryNum;           //可设置的锁定次数
        uint lockTime;              //当前的锁定时间
        uint tryNum;                //尝试的次数记录
    };

public:
    QString desktopBackgroundPath();
    bool isUserIsvalid();

    UserInter *getUserInter() { return m_userInter; }
    const QString name() const { return m_userName; }
    const QString locale() const { return m_locale; }
    void setLocale(const QString &locale);
    uid_t uid() const { return m_uid; }

    // 用户锁定相关
    uint lockTime() const { return m_lockData.lockTime; }
    bool isLock() const { return m_isLock; }
    bool isLockForNum();
    void startLock();
    void resetLock();

private:
    void initLockData();
    void initUserInfo(const QString &path);
    QString userPwdName(__uid_t uid);
    void configAccountInfo(const QString& account_config);
    void onLockTimeOut();

signals:
    void localeChanged(const QString &locale) const;
    void lockChanged(bool lock);

private:
    uid_t m_uid = INT_MAX;
    QString m_userName;
    QString m_locale;
    QString m_desktopBackground;
    UserInter *m_userInter;
    // 用户锁定相关
    bool m_isLock;
    std::shared_ptr<QTimer> m_lockTimer;
    time_t m_startTime;  // 记录账号锁定的时间搓
    lockData m_lockData;
};

#endif // USERINFO_H
