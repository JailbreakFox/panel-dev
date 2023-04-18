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

#include "userinfo.h"

#include <pwd.h>
#include <grp.h>
#include <unistd.h>

static const std::vector<uint> DEFAULT_WAIT_TIME = {3, 5, 15, 60, 1440};

User::User(const QString &path, QObject *parent)
    : QObject(parent)
    , m_locale(getenv("LANG"))
    , m_userInter(new UserInter(DDESESSIONCC::ACCOUNT_DBUS_NAME, path, QDBusConnection::systemBus(), this))
    , m_isLock(false)
    , m_lockTimer(new QTimer)
{
    initLockData();
    initUserInfo(path);
}

QString User::desktopBackgroundPath()
{
    m_userInter->desktopBackgrounds();
    return m_desktopBackground;
}

bool User::isUserIsvalid()
{
    //无效用户的时候m_userInter是有效的
    return m_userInter->isValid();
}

void User::setLocale(const QString &locale)
{
    if (m_locale == locale) return;

    m_locale = locale;

    emit localeChanged(locale);
}

bool User::isLockForNum()
{
    m_lockData.tryNum++;
    return m_isLock || (m_lockData.tryNum >= m_lockData.limitTryNum);
}

void User::startLock()
{
    m_startTime = time(nullptr);
    if (m_lockTimer->isActive())
        return;

    m_isLock = true;

    if ((m_lockData.tryNum - m_lockData.limitTryNum + 1) > m_lockData.waitTime.size())
        m_lockData.lockTime = m_lockData.waitTime.back();
    else
        m_lockData.lockTime = m_lockData.waitTime[m_lockData.tryNum - m_lockData.limitTryNum];

    onLockTimeOut();
}

void User::resetLock()
{
    m_lockData.tryNum = 0;
    m_startTime = 0;
}

void User::initLockData()
{
    m_lockData.lockTime = 0;
    m_lockData.tryNum = 0;
    auto waitTimeAvail = [ = ](std::vector<uint> *waitTime) {
        if (waitTime->empty())
            return false;

        for (uint i = 0; i < waitTime->size(); i++) {
            if (i < waitTime->size() - 1) {
                // 需求规定，等待的最大时间不得超过2880分钟
                if (waitTime->at(i) > 2880)
                    waitTime->at(i) = 2880;
                // 需求规定，等待时间前一次必须小于等于后一次
                if (waitTime->at(i) > waitTime->at(i+1))
                    waitTime->at(i+1) = waitTime->at(i);
            }
        }
        return true;
    };

    // 可配置认证密码失败,且超过失败次数后的间隔限制时间
    QString lockWaitTime = findValueByQSettings<QString>("LockTime", "lockWaitTime", "Default");
    if(lockWaitTime == "Default")
        m_lockData.waitTime = DEFAULT_WAIT_TIME;
    else {
        std::vector<uint> waitTime;
        for (QString i : lockWaitTime.split(",")) {
            waitTime.push_back(i.toUInt());
        }

        m_lockData.waitTime = waitTimeAvail(&waitTime) ? waitTime : DEFAULT_WAIT_TIME;
    }

    // 可配置进入等待前的失败次数
    uint lockLimitTryNum = findValueByQSettings<uint>("LockTime", "lockLimitTryNum", 0);
    if(lockLimitTryNum == 0 || lockLimitTryNum > 10)
        m_lockData.limitTryNum = 5;
    else
        m_lockData.limitTryNum = lockLimitTryNum;

    m_lockTimer->setInterval(1000 * 60);
    m_lockTimer->setSingleShot(false);
    connect(m_lockTimer.get(), &QTimer::timeout, this, &User::onLockTimeOut);
}

void User::initUserInfo(const QString &path)
{
    m_userInter->setSync(false);

    connect(m_userInter, &UserInter::UidChanged, this, [ = ](const QString & uid) {
        m_uid = uid.toUInt();
    });
    connect(m_userInter, &UserInter::LocaleChanged, this, &User::setLocale);

    m_userInter->locale();
    m_userInter->passwordStatus();

    // intercept account dbus path uid
    m_uid = path.mid(QString(DDESESSIONCC::ACCOUNTS_DBUS_PREFIX).size()).toUInt();
    m_userName = userPwdName(m_uid);

    configAccountInfo(DDESESSIONCC::CONFIG_FILE + m_userName);
}

QString User::userPwdName(__uid_t uid)
{
    if (uid < 1000) return QString();

    struct passwd *pw = nullptr;
    /* Fetch passwd structure (contains first group ID for user) */
    pw = getpwuid(uid);

    return QString().fromLocal8Bit(pw->pw_name);
}

void User::configAccountInfo(const QString &account_config)
{
    QSettings settings(account_config, QSettings::IniFormat);
    settings.beginGroup("User");

    m_locale = settings.value("Locale").toString();

    QStringList desktopBackgrounds = settings.value("DesktopBackgrounds").toString().split(";");

    // TODO: 平板项目一期使用固定壁纸
    m_desktopBackground = DDESESSIONCC::DESKTOP_BACKGROUND_PICTURE;
}

void User::onLockTimeOut()
{
    time_t stopTime = time(nullptr);
    auto min = static_cast<uint>((stopTime - m_startTime) / 60);
    uint minLimit;
    if ((m_lockData.tryNum - m_lockData.limitTryNum + 1) > m_lockData.waitTime.size())
        minLimit = m_lockData.waitTime.back();
    else
        minLimit = m_lockData.waitTime[m_lockData.tryNum - m_lockData.limitTryNum];

    if (min >= minLimit) {
        // 到达等待时间
        m_isLock = false;
        m_lockTimer->stop();
    } else {
        // 未到达等待时间
        m_lockData.lockTime = minLimit - min;
        m_lockTimer->start();
    }

    emit lockChanged(m_isLock);
}
