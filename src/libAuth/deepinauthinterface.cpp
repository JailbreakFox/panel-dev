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

#include "deepinauthinterface.h"
#include "sessionbasemodel.h"
#include "sessionshellcontroller.h"
#include "userinfo.h"
#include "public_func.h"

DeepinAuthInterface::DeepinAuthInterface(SessionBaseModel *const model, SessionShellController *const controller, QObject *parent)
    : QObject(parent)
    , m_model(model)
    , m_controller(controller)
{
    if (QGSettings::isSchemaInstalled(DDESESSIONCC::GSETTING_NAME)) {
        m_gsettings = new QGSettings(DDESESSIONCC::GSETTING_NAME, DDESESSIONCC::GSETTING_PATH, this);
    }
}

QVariant DeepinAuthInterface::getGSettings(const QString& node, const QString& key)
{
    QVariant value = findValueByQSettings<QVariant>(node, key, true);
    if (m_gsettings && m_gsettings->keys().contains(key))
        value = m_gsettings->get(key);

    return value;
}

bool DeepinAuthInterface::isDeepin()
{
    // 这是临时的选项，只在Deepin下启用同步认证功能，其他发行版下禁用。
#ifdef QT_DEBUG
    return true;
#else
    return getGSettings("", "useDeepinAuth").toBool();
#endif
}
