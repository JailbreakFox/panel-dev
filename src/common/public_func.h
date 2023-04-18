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

#ifndef PUBLIC_FUNC_H
#define PUBLIC_FUNC_H

#include "constant.h"

#include <QImageReader>
#include <QSettings>

template <typename T>
T findValueByQSettings(const QString &group,
                       const QString &key,
                       const QVariant &failback)
{
    for (const QString &path : DDESESSIONCC::session_ui_configs) {
        QSettings settings(path, QSettings::IniFormat);
        if (!group.isEmpty()) {
            settings.beginGroup(group);
        }

        const QVariant& v = settings.value(key);
        if (v.isValid()) {
            T t = v.value<T>();
            return t;
        }
    }

    return failback.value<T>();
}

/**
 * @brief 捕获程序异常退出
 *
 * @param sig 异常退出信号
 */
[[noreturn]] void sig_crash(int sig);

/**
 * @brief 初始化需要捕获的异常信号
 */

void init_sig_crash();

#endif // PUBLIC_FUNC_H
