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

#ifndef SESSIONBASEMODEL_H
#define SESSIONBASEMODEL_H

#include <QObject>

#include <memory>

class User;
class SessionBaseModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool isScreenBlackLock READ isScreenBlackLock WRITE setIsScreenBlackLock NOTIFY isScreenBlackLockChanged)
    Q_PROPERTY(int isFirstTimeLogin READ isFirstTimeLogin WRITE setIsFirstTimeLogin NOTIFY isFirstTimeLoginChanged)
    Q_PROPERTY(QString desktopBackground READ desktopBackground NOTIFY desktopBackgroundChanged)
    Q_PROPERTY(QString time READ time NOTIFY timeChanged)
    Q_PROPERTY(QString date READ date NOTIFY dateChanged)
    Q_PROPERTY(int passwordLength READ passwordLength NOTIFY passwordLengthChanged)
    Q_PROPERTY(int isLock READ isLock NOTIFY isLockChanged)
    Q_PROPERTY(uint lockTime READ lockTime NOTIFY lockTimeChanged)
    Q_PROPERTY(bool isShow READ isShow WRITE setIsShow NOTIFY visibleChanged)
    Q_PROPERTY(int currentModeState READ currentModeState WRITE setCurrentModeState NOTIFY onStatusChanged)

// 平板项目新增
public:
    enum ModeStatus {
        NoStatus,
        PowerMode
    };

    explicit SessionBaseModel(QObject *parent = nullptr);

    inline std::shared_ptr<User> currentUser() const { return m_currentUser; }
    inline bool isScreenBlackLock() const { return m_isScreenBlackLock; }
    inline bool isFirstTimeLogin() const { return m_isFirstTimeLogin; }
    QString desktopBackground();
    inline QString time() const { return m_time; }
    inline QString date() const { return m_date; }
    inline int passwordLength() const { return m_passwordLength; }
    inline bool isLock() const { return m_isLock; }
    uint lockTime();
    inline bool isShow() const { return m_isShow; }
    inline int currentModeState() const { return m_currentModeState; }

    void setCurrentUser(std::shared_ptr<User> user);
    void setIsScreenBlackLock(bool isScreenBlackLock);
    void setIsFirstTimeLogin(bool isFirstTimeLogin);
    void setTime(const QString &time);
    void setDate(const QString &date);
    void setPasswordLength(const int &passwordLength);
    void setLock(const bool &isLock);
    Q_INVOKABLE void setIsShow(bool isShow);
    Q_INVOKABLE void setCurrentModeState(const int &currentModeState);

Q_SIGNALS:
    void isScreenBlackLockChanged(bool isScreenBlackLock);
    void isFirstTimeLoginChanged(bool isFirstTimeLogin);
    void desktopBackgroundChanged(QString desktopBackground);
    void currentUserChanged(std::shared_ptr<User> user);
    void timeChanged(QString desktopBackground);
    void dateChanged(QString desktopBackground);
    void passwordLengthChanged(int passwordLength);
    void isLockChanged(bool isLock);
    void lockTimeChanged();
    void visibleChanged(bool visible);
    void onStatusChanged(int status);

private:
    std::shared_ptr<User> m_currentUser;
    bool m_isScreenBlackLock;
    bool m_isShow = false;
    QString m_desktopBackground;
    int m_currentModeState;
    // 时间相关
    QString m_time;
    QString m_date;
    // 认证相关
    bool m_isLock = false;
    bool m_isFirstTimeLogin;
    int m_passwordLength = 0;
    uint m_lockTime;
};

#endif // SESSIONBASEMODEL_H
