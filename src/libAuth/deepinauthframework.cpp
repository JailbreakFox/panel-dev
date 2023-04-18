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

#include "deepinauthframework.h"
#include "deepinauthinterface.h"
#include "userinfo.h"
#include "authagent.h"

#include <QThread>
#include <QTimer>
#include <QVariant>

#include <pwd.h>
#include <grp.h>
#include <signal.h>
#include <unistd.h>

DeepinAuthFramework::DeepinAuthFramework(DeepinAuthInterface *inter, QObject *parent)
    : QObject(parent)
    , m_interface(inter)
{
    m_authagent = new AuthAgent(this);
}

DeepinAuthFramework::~DeepinAuthFramework()
{
    EndAuthenticate();
    delete m_authagent;
    m_authagent = nullptr;
}

bool DeepinAuthFramework::isAuthenticate() const
{
    return m_pamAuth != 0;
}

int DeepinAuthFramework::GetAuthType()
{
    return m_authagent->GetAuthType();
}

void *DeepinAuthFramework::pamAuthWorker(void *arg)
{
    DeepinAuthFramework *deepin_auth = static_cast<DeepinAuthFramework*>(arg);
    if(deepin_auth != nullptr && deepin_auth->m_currentUser != nullptr) {
        //开始验证,并重置变量等待输入密码
        deepin_auth->m_authagent->setCancelAuth(false);
        deepin_auth->m_authagent->Authenticate(deepin_auth->m_currentUser->name());
    } else {
        qWarning() << "pam auth worker deepin framework is nullptr";
    }

    return nullptr;
}

void DeepinAuthFramework::Authenticate(std::shared_ptr<User> user)
{
    if (user->isLock() || user->name().isEmpty())
        return;

    m_password.clear();

    if (m_pamAuth != 0) {
        qDebug() << "pam auth cancel" << m_authagent->thread()->loopLevel();
        //先取消上次验证请求
        m_authagent->setCancelAuth(true);
        //发送退出线程消息
        pthread_cancel(m_pamAuth);
        //等待线程退出
        pthread_join(m_pamAuth, nullptr);
        m_pamAuth = 0;
    }

    m_currentUser = user;

    //创建验证线程,等待输入密码
    int rc = pthread_create(&m_pamAuth, nullptr, &pamAuthWorker, this);
    if (rc != 0) {
        qWarning() << "failed to create the authentication thread: %s" << strerror(errno);
    } else {
        qInfo() << "[+]Start Authentication Thread!";
    }
}

void DeepinAuthFramework::EndAuthenticate()
{
    if (m_authagent.isNull())
        return;

    if (m_pamAuth != 0) {
        //先取消上次验证请求
        m_authagent->setCancelAuth(true);
        pthread_cancel(m_pamAuth);
        pthread_join(m_pamAuth, nullptr);
        m_pamAuth = 0;
    }
}

void DeepinAuthFramework::Responsed(const QString &password)
{
    if (m_authagent.isNull() || m_pamAuth == 0) {
        qWarning() << "pam auth agent is not start";
        return;
    }

    m_password = password;
    if (!m_password.isEmpty()) {
        qInfo() << "pam responsed password";

        m_authagent->Responsed(password);
    }
}

const QString DeepinAuthFramework::RequestEchoOff(const QString &msg)
{
    Q_UNUSED(msg);

    return m_password;
}

const QString DeepinAuthFramework::RequestEchoOn(const QString &msg)
{
    return msg;
}

void DeepinAuthFramework::DisplayErrorMsg(const QString &msg)
{
    m_interface->onDisplayErrorMsg(msg);
}

void DeepinAuthFramework::DisplayTextInfo(const QString &msg)
{
    m_interface->onDisplayTextInfo(msg);
}

void DeepinAuthFramework::RespondResult(const QString &msg)
{
    m_interface->onPasswordResult(msg);
}
