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

#include "dbusshutdownagent.h"
#include "sessionbasemodel.h"
#include "sessionshellcontroller.h"

DBusShutdownAgent::DBusShutdownAgent(SessionBaseModel *const model, SessionShellController *const controller)
    : m_model(model)
    , m_controller(controller)
{

}

void DBusShutdownAgent::Shutdown()
{
    m_controller->requestShutdown();
}

void DBusShutdownAgent::Restart()
{
    m_controller->requestReboot();
}

void DBusShutdownAgent::Show()
{
    m_controller->requestShow(EnumType::ModeStatus::PowerMode);
}

bool DBusShutdownAgent::ShutdownVisible()
{
    return (m_model->currentModeState() == EnumType::ModeStatus::PowerMode) && m_model->isShow();
}
