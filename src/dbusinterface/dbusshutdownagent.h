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

#ifndef DBUSSHUTDOWNAGENT_H
#define DBUSSHUTDOWNAGENT_H

#include <QObject>

class SessionBaseModel;
class SessionShellController;
class DBusShutdownAgent : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Adapter", "com.deepin.dde.shutdownFront")

public:
    explicit DBusShutdownAgent(SessionBaseModel *const model, SessionShellController *const controller);

public Q_SLOTS:
    void Shutdown();
    void Restart();
    void Show();
    bool ShutdownVisible();

private:
    SessionBaseModel *m_model;
    SessionShellController *m_controller;
};

#endif // DBUSSHUTDOWNAGENT_H
