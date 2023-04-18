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

#ifndef DEEPINAUTHINTERFACE_H
#define DEEPINAUTHINTERFACE_H

#include "public_func.h"
#include "constant.h"

#include <QGSettings>

class User;
class SessionBaseModel;
class SessionShellController;
class DeepinAuthInterface : public QObject {
    Q_OBJECT
public:
    explicit DeepinAuthInterface(SessionBaseModel *const model, SessionShellController *const controller, QObject *parent = nullptr);

    virtual void authUser(const QString &password) = 0;
    virtual void onDisplayErrorMsg(const QString &msg) = 0;
    virtual void onDisplayTextInfo(const QString &msg) = 0;
    virtual void onPasswordResult(const QString &msg) = 0;

protected:
    bool isDeepin();
    QVariant getGSettings(const QString& node, const QString& key);

protected:
    SessionBaseModel *m_model;
    SessionShellController *m_controller;
    QGSettings       *m_gsettings;
    uint              m_currentUserUid;
};

#endif // DEEPINAUTHINTERFACE_H
