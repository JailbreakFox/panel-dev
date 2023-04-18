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

#ifndef DBUSLOCKAGENT_H
#define DBUSLOCKAGENT_H

#include <QObject>

class SessionBaseModel;
class SessionShellController;
class SessionShellWorker;
class DBusLockAgent : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Adapter", "com.deepin.dde.lockFront")

public:
    explicit DBusLockAgent(SessionBaseModel *const model, SessionShellController *const controller, SessionShellWorker *const worker);

public Q_SLOTS:
    void Show();
    void Hide();
    bool LockVisible();
    bool LockAuthenticating();

Q_SIGNALS:
    void Visible(bool visible);
    void Authenticate(bool result);

private:
    SessionBaseModel *m_model;
    SessionShellController *m_controller;
    SessionShellWorker *m_worker;
};

#endif // DBUSLOCKAGENT_H
