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

#include "dbuslockagent.h"
#include "sessionbasemodel.h"
#include "sessionshellcontroller.h"
#include "sessionshellworker.h"

DBusLockAgent::DBusLockAgent(SessionBaseModel *const model, SessionShellController *const controller, SessionShellWorker *const worker)
    : m_model(model)
    , m_controller(controller)
    , m_worker(worker)
{

}

void DBusLockAgent::Show()
{
    m_controller->requestShow(EnumType::ModeStatus::NoStatus);
}

void DBusLockAgent::Hide()
{
    // 只有锁屏未处于认证状态时，才能响应外界的隐藏锁屏请求
    if (!m_worker->isAuthenticate())
        m_model->setIsShow(false);
}

bool DBusLockAgent::LockVisible()
{
    return (m_model->currentModeState() == EnumType::ModeStatus::NoStatus) && m_model->isShow();
}

bool DBusLockAgent::LockAuthenticating()
{
    return m_worker->isAuthenticate();
}
