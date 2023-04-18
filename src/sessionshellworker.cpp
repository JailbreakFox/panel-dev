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

#include "sessionshellworker.h"
#include "sessionbasemodel.h"
#include "sessionshellcontroller.h"
#include "deepinauthframework.h"
#include "deepinauthinterface.h"
#include "constant.h"
#include "userinfo.h"

SessionShellWorker::SessionShellWorker(SessionBaseModel *const model, SessionShellController *const controller, QObject *parent)
    : DeepinAuthInterface(model, controller, parent)
    , m_isThumbAuth(false)
    , m_authenticating(false)
    , m_authFramework(new DeepinAuthFramework(this, this))
    , m_powerInter(new PowerInter(DDESESSIONCC::POWER_DBUS_NAME, DDESESSIONCC::POWER_DBUS_PATH, QDBusConnection::sessionBus(), this))
    , m_timedateInter(new Timedate(DDESESSIONCC::TIMEDATE_DBUS_NAME, DDESESSIONCC::TIMEDATE_DBUS_PATH, QDBusConnection::sessionBus(), this))
    , m_sessionInter(new SessionManager(DDESESSIONCC::SESSIONMANAGER_DBUS_NAME, DDESESSIONCC::SESSIONMANAGER_DBUS_PATH, QDBusConnection::sessionBus(), this))
{
    m_currentUserUid = getuid();

    initUser(DDESESSIONCC::ACCOUNTS_DBUS_PREFIX + QString::number(m_currentUserUid));
    initTimer();
}

void SessionShellWorker::onDisplayErrorMsg(const QString &msg)
{
    // TODO: 弹窗认证消息
    Q_UNUSED(msg);
}

void SessionShellWorker::onDisplayTextInfo(const QString &msg)
{
    emit m_controller->authFaildMessage(msg);
}

void SessionShellWorker::onPasswordResult(const QString &msg)
{
    onUnlockFinished(!msg.isEmpty());

    if(msg.isEmpty()) {
        m_authFramework->Authenticate(m_model->currentUser());
        m_authenticating = false;
    }
}

void SessionShellWorker::authUser(const QString &password)
{
    if (m_authenticating)
        return;

    m_authenticating = true;

    // auth interface
    std::shared_ptr<User> user = m_model->currentUser();

    qInfo() << "start authentication of user: " << user->name();

    if(!m_authFramework->isAuthenticate())
        m_authFramework->Authenticate(user);

    m_authFramework->Responsed(password);
}

void SessionShellWorker::passwordUnitAdd(const QString passwordUnit)
{
    // 如果正在认证过程中或者用户已经被锁定,则不执行
    if (m_authenticating || m_model->currentUser()->isLock())
        return;

    m_password.append(passwordUnit);
    m_model->setPasswordLength(m_password.size());

    if (m_password.size() >= 6) {
        QString password = m_password.mid(0, 6);
        m_password.clear();
        authUser(password);
    }
}

void SessionShellWorker::passwordUnitDelete()
{
    // 如果正在认证过程中或者用户已经被锁定,则不执行
    if (m_authenticating || m_model->currentUser()->isLock())
        return;

    if (!m_password.isEmpty())
        m_password.chop(1);

    m_model->setPasswordLength(m_password.size());
}

bool SessionShellWorker::isAuthenticate()
{
    if (!m_authFramework)
        return false;

    return m_authFramework->isAuthenticate();
}

void SessionShellWorker::initTimer()
{
    UserInter *userInter = m_model->currentUser()->getUserInter();

    if (!userInter->isValid()) {
        qWarning() << "userInter initial failed!";
        return;
    }

    m_refreshTimer = new QTimer(this);
    m_refreshTimer->setInterval(1000);
    m_refreshTimer->start();

    connect(m_refreshTimer, &QTimer::timeout, this, &SessionShellWorker::refreshTimeAndDate, Qt::UniqueConnection);
    connect(userInter, &UserInter::LongDateFormatChanged, this, &SessionShellWorker::setLongDateFormat);
    connect(userInter, &UserInter::ShortTimeFormatChanged, this, &SessionShellWorker::setShortTimeFormat);
    connect(userInter, &UserInter::Use24HourFormatChanged, this, &SessionShellWorker::setUse24HourFormat);

    m_shortTimeIndex = m_timedateInter->shortTimeFormat();
    m_longDateIndex = m_timedateInter->longDateFormat();
    m_use24HourFormat = m_timedateInter->use24HourFormat();

    refreshTimeAndDate();
}

void SessionShellWorker::refreshTimeAndDate()
{
    QString time;
    QString date;

    if (m_use24HourFormat) {
        time = QLocale(m_model->currentUser()->locale()).toString(QDateTime::currentDateTime(), DDESESSIONCC::shortTimeFormat.at(m_shortTimeIndex));
    } else {
        time = QLocale(m_model->currentUser()->locale()).toString(QDateTime::currentDateTime(), DDESESSIONCC::shortTimeFormat.at(m_shortTimeIndex) + " AP");
    }

    date = QLocale(m_model->currentUser()->locale()).toString(QDateTime::currentDateTime(), DDESESSIONCC::longDateFormat.at(m_longDateIndex));

    Q_EMIT m_model->setTime(time);
    Q_EMIT m_model->setDate(date);
}

/**
 * @brief SessionShellWorker::setLongDateFormat 根据类型来设置长日期显示格式
 * @param type 自定义格式
 */
void SessionShellWorker::setLongDateFormat(int type)
{
    if (type >= DDESESSIONCC::longDateFormat.size() || type < 0)
        return;

    m_longDateIndex = type;
    refreshTimeAndDate();
}

/**
 * @brief SessionShellWorker::setShortTimeFormat 根据类型来设置短时间显示格式
 * @param type
 */
void SessionShellWorker::setShortTimeFormat(int type)
{
    if(type >= DDESESSIONCC::shortTimeFormat.size() || type < 0)
        return;

    m_shortTimeIndex = type;
    refreshTimeAndDate();
}

/**
 * @brief SessionShellWorker::setUse24HourFormat 根据类型来设置24H时间格式
 * @param type
 */
void SessionShellWorker::setUse24HourFormat(bool type)
{
    m_use24HourFormat = type;
    refreshTimeAndDate();
}

void SessionShellWorker::initUser(const QString &user)
{
    std::shared_ptr<User> user_ptr = nullptr;
    uid_t uid = user.mid(QString(DDESESSIONCC::ACCOUNTS_DBUS_PREFIX).size()).toUInt();
    if(uid < DDESESSIONCC::DOMAIN_BASE_UID)
        user_ptr = std::make_shared<User>(user, this);

    if (!user_ptr->isUserIsvalid())
        return;

    if (user_ptr->uid() == getuid()) {
        m_model->setCurrentUser(user_ptr);

        m_model->setIsScreenBlackLock(m_powerInter->screenBlackLock());

        bool isLock = user_ptr->isLock();
        m_model->setLock(isLock);
        if (isLock && m_model->isScreenBlackLock()) {
            qInfo() << "Continue lock";
            Q_EMIT m_model->lockTimeChanged();
        }

        connect(m_powerInter, &PowerInter::ScreenBlackLockChanged, this, [ = ](bool isScreenBlackLock) {
            m_model->setIsScreenBlackLock(isScreenBlackLock);
        });
        connect(user_ptr.get(), &User::lockChanged, this, [ = ](bool isLock) {
            m_model->setLock(isLock);
            Q_EMIT m_model->lockTimeChanged();

            // 在锁定状态解除后需要开启认证
            if (!isLock)
                m_authFramework->Authenticate(user_ptr);
        });
        connect(m_controller, &SessionShellController::requestPasswordUnitAdd, this, &SessionShellWorker::passwordUnitAdd);
        connect(m_controller, &SessionShellController::requestPasswordUnitDelete, this, &SessionShellWorker::passwordUnitDelete);
        connect(m_controller, &SessionShellController::requestShow, this, &SessionShellWorker::onShow);
        connect(m_controller, &SessionShellController::requestReboot, this, &SessionShellWorker::onReboot);
        connect(m_controller, &SessionShellController::requestShutdown, this, &SessionShellWorker::onShutdown);
        connect(m_controller, &SessionShellController::requestStartAuth, this, [ = ] {
            m_authFramework->Authenticate(user_ptr);
        });
    }
}

/**
 * @brief SessionShellWorker::onUnlockFinished 根据返回结果来做认证成功/失败的对应逻辑
 * @param unlocked 为真时代表认证成功
 */
void SessionShellWorker::onUnlockFinished(bool unlocked)
{
    qInfo() << "Password verified result: " << unlocked;
    emit m_controller->authFinished(unlocked);

    // 在获取到认证结束的信号后,清除密码,并告知视图密码长度清零
    m_password.clear();
    m_model->setPasswordLength(0);

    if (unlocked) {
        // 认证成功的情况下,隐藏界面,触发初始化界面的信号
        QTimer::singleShot(500, this, [ = ] {
            m_model->setIsFirstTimeLogin(false);
            m_model->currentUser()->resetLock();
            m_model->setIsShow(false);
            m_controller->requestInitial();
            m_authenticating = false;
            m_authFramework->EndAuthenticate();
        });
    } else if (m_authFramework->GetAuthType() & EnumType::AuthFlag::Password) {
        // 密码认证失败后的操作
        if (m_model->currentUser()->isLockForNum()) {
            m_model->currentUser()->startLock();
        }
        m_authenticating = false;
    }
}

void SessionShellWorker::onShow(EnumType::ModeStatus modeStatus) {
    if (!m_sessionInter->isValid())
        return;

    m_model->setCurrentModeState(modeStatus);
    if (modeStatus == EnumType::ModeStatus::PowerMode)
        m_controller->setBackToLockTimer(EnumType::ModeStatus::PowerMode, true);
    m_model->setIsShow(true);
}

void SessionShellWorker::onReboot() {
    if (!m_sessionInter->isValid())
        return;

    m_sessionInter->RequestReboot();
}

void SessionShellWorker::onShutdown() {
    if (!m_sessionInter->isValid())
        return;

    m_sessionInter->RequestShutdown();
}
