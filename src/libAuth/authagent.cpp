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

#include "authagent.h"
#include "deepinauthframework.h"
#include "public_func.h"
#include "constant.h"

#include <QDebug>

#include <security/pam_appl.h>
#include <unistd.h>

#ifdef PAM_SUN_CODEBASE
#define PAM_MSG_MEMBER(msg, n, member) ((*(msg))[(n)].member)
#else
#define PAM_MSG_MEMBER(msg, n, member) ((msg)[(n)]->member)
#endif

AuthAgent::AuthAgent(DeepinAuthFramework *deepin)
    : m_deepinauth(deepin)
    , m_isCondition(true)
    , m_isCancel(false)
{
    connect(this, &AuthAgent::displayErrorMsg, deepin, &DeepinAuthFramework::DisplayErrorMsg, Qt::QueuedConnection);
    connect(this, &AuthAgent::displayTextInfo, deepin, &DeepinAuthFramework::DisplayTextInfo, Qt::QueuedConnection);
    connect(this, &AuthAgent::respondResult, deepin, &DeepinAuthFramework::RespondResult, Qt::QueuedConnection);
}

AuthAgent::~AuthAgent()
{
}

void AuthAgent::Responsed(const QString &password)
{
    m_password = password;
    m_isCondition = false;
}

void AuthAgent::Authenticate(const QString& username)
{
    pam_handle_t *m_pamHandle = nullptr;
    pam_conv conv = { pamConversation, static_cast<void*>(this) };
    const char *serviceName = DDESESSIONCC::PAM_SERVICE_DEEPIN_NAME.toLatin1();
    int ret = pam_start(serviceName, username.toLocal8Bit().data(), &conv, &m_pamHandle);

    if (ret != PAM_SUCCESS) {
        qWarning() << "pam_start() failed: " << pam_strerror(m_pamHandle, ret);
    }

    int rc = pam_authenticate(m_pamHandle, 0);
    if (rc != PAM_SUCCESS) {
        qDebug() << "pam_authenticate() failed: " << pam_strerror(m_pamHandle, rc);
    }

    int re = pam_end(m_pamHandle, rc);
    if (re != PAM_SUCCESS) {
        qWarning() << "pam_end() failed: " << pam_strerror(m_pamHandle, re);
    }

    bool is_success = (rc == PAM_SUCCESS) && (re == PAM_SUCCESS);

    m_isCondition = true;
    emit respondResult(is_success ? "success" : QString());
}

int AuthAgent::GetAuthType()
{
    return m_authType;
}

int AuthAgent::pamConversation(int num_msg, const struct pam_message **msg,
                               struct pam_response **resp, void *app_data)
{
    AuthAgent *app_ptr = static_cast<AuthAgent *>(app_data);
    struct pam_response *aresp = nullptr;
    int idx = 0;
    AuthFlag auth_type = AuthFlag::Password;

    QPointer<AuthAgent> isThreadAlive(app_ptr);
    if (!isThreadAlive) {
        qWarning() << "pam: application is null";
        return PAM_CONV_ERR;
    }

    if (num_msg <= 0 || num_msg > PAM_MAX_NUM_MSG)
        return PAM_CONV_ERR;

    if ((aresp = static_cast<struct pam_response*>(calloc(static_cast<size_t>(num_msg), sizeof(*aresp)))) == nullptr)
        return PAM_BUF_ERR;

    for (idx = 0; idx < num_msg; ++idx) {
        switch(PAM_MSG_MEMBER(msg, idx, msg_style)) {
        case PAM_PROMPT_ECHO_OFF: {
            while(app_ptr->m_isCondition) {
                //取消验证时返回一般错误,退出等待输入密码的循环,然后退出验证线程
                if (app_ptr->m_isCancel) {
                    return PAM_ABORT;
                }
                sleep(1);
            }

            app_ptr->m_isCondition = true;

            if (!QPointer<DeepinAuthFramework>(app_ptr->deepinAuth())) {
                qWarning() << "pam: deepin auth framework is null";
                return PAM_CONV_ERR;
            }

            QString password = app_ptr->deepinAuth()->RequestEchoOff(PAM_MSG_MEMBER(msg, idx, msg));
            aresp[idx].resp = strdup(password.toLocal8Bit().data());

            if (aresp[idx].resp == nullptr)
              goto fail;

            auth_type = AuthFlag::Password;
            aresp[idx].resp_retcode = PAM_SUCCESS;
            break;
        }

        case PAM_PROMPT_ECHO_ON:
        case PAM_ERROR_MSG: {
            qWarning() << "pam authagent error: " << PAM_MSG_MEMBER(msg, idx, msg);
            app_ptr->displayErrorMsg(QString::fromLocal8Bit(PAM_MSG_MEMBER(msg, idx, msg)));
            auth_type = AuthFlag::Fingerprint;
            aresp[idx].resp_retcode = PAM_SUCCESS;
            break;
        }

        case PAM_TEXT_INFO: {
            qWarning() << "pam authagent info: " << PAM_MSG_MEMBER(msg, idx, msg);
            app_ptr->displayTextInfo(QString::fromLocal8Bit(PAM_MSG_MEMBER(msg, idx, msg)));
            aresp[idx].resp_retcode = PAM_SUCCESS;
            break;
         }

        default:
            goto fail;
        }
    }

    *resp = aresp;
    if (auth_type == AuthFlag::Password) {
        app_ptr->m_authType = AuthFlag::Password;
    } else if (auth_type == AuthFlag::Fingerprint) {
        app_ptr->m_authType = AuthFlag::Fingerprint;
    }
    return PAM_SUCCESS;

fail:
    for (idx = 0; idx < num_msg; idx++) {
        free(aresp[idx].resp);
    }
    free(aresp);
    return PAM_CONV_ERR;
}
