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

#ifndef DEEPINAUTHFRAMEWORK_H
#define DEEPINAUTHFRAMEWORK_H

#include <QObject>
#include <QPointer>

#include <memory>

class DeepinAuthInterface;
class QThread;
class User;
class AuthAgent;
class DeepinAuthFramework : public QObject
{
    Q_OBJECT
public:
    explicit DeepinAuthFramework(DeepinAuthInterface *inter, QObject *parent = nullptr);
    ~DeepinAuthFramework();

    friend class AuthAgent;
    bool isAuthenticate() const;
    int GetAuthType();

public slots:
    void Authenticate(std::shared_ptr<User> user);
    void EndAuthenticate();
    void Responsed(const QString &password);

private:
    static void *pamAuthWorker(void *arg);
    const QString RequestEchoOff(const QString &msg);
    const QString RequestEchoOn(const QString &msg);
    void DisplayErrorMsg(const QString &msg);
    void DisplayTextInfo(const QString &msg);
    void RespondResult(const QString &msg);

private:
    DeepinAuthInterface *m_interface;
    QPointer<AuthAgent> m_authagent;
    std::shared_ptr<User> m_currentUser = nullptr;
    QString m_password;
    pthread_t m_pamAuth = 0;
};

#endif // DEEPINAUTHFRAMEWORK_H
