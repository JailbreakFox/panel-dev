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

#include "sessionbasemodel.h"
#include "userinfo.h"

#include <DSysInfo>

#include <QDebug>

#include <com_deepin_sessionmanager.h>

SessionBaseModel::SessionBaseModel(QObject *parent)
    : QObject(parent)
    , m_currentUser(nullptr)
    , m_isShow(false)
    , m_currentModeState(ModeStatus::NoStatus)
    , m_isFirstTimeLogin(true)
{
}

QString SessionBaseModel::desktopBackground()
{
    return m_currentUser->desktopBackgroundPath();
}

uint SessionBaseModel::lockTime()
{
    return m_currentUser->lockTime();
}

void SessionBaseModel::setCurrentUser(std::shared_ptr<User> user)
{
    if (m_currentUser == user)
        return;

    m_currentUser = user;

    emit currentUserChanged(user);
}

void SessionBaseModel::setIsScreenBlackLock(bool isScreenBlackLock)
{
    if(m_isScreenBlackLock == isScreenBlackLock)
        return;

    m_isScreenBlackLock = isScreenBlackLock;

    Q_EMIT isScreenBlackLockChanged(isScreenBlackLock);
}

void SessionBaseModel::setIsFirstTimeLogin(bool isFirstTimeLogin)
{
    if(!m_isFirstTimeLogin || isFirstTimeLogin)
        return;

    m_isFirstTimeLogin = isFirstTimeLogin;

    Q_EMIT isFirstTimeLoginChanged(isFirstTimeLogin);
}

void SessionBaseModel::setTime(const QString &time)
{
    if(m_time == time)
        return;

    m_time = time;

    Q_EMIT timeChanged(time);
}

void SessionBaseModel::setDate(const QString &date)
{
    if(m_date == date)
        return;

    m_date = date;

    Q_EMIT dateChanged(date);
}

void SessionBaseModel::setPasswordLength(const int &passwordLength)
{
    if(m_passwordLength == passwordLength)
        return;

    m_passwordLength = passwordLength;

    Q_EMIT passwordLengthChanged(passwordLength);
}

void SessionBaseModel::setLock(const bool &isLock)
{
    if(m_isLock == isLock)
        return;

    m_isLock = isLock;

    Q_EMIT isLockChanged(isLock);
}

void SessionBaseModel::setIsShow(bool isShow)
{
    if (m_isShow == isShow)
        return;

    m_isShow = isShow;

    emit visibleChanged(m_isShow);
}

void SessionBaseModel::setCurrentModeState(const int &currentModeState)
{
    if (m_currentModeState == currentModeState)
        return;

    m_currentModeState = currentModeState;

    emit onStatusChanged(currentModeState);
}
