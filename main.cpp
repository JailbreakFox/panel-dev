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

#include "constant.h"
#include "sessionbasemodel.h"
#include "sessionshellworker.h"
#include "sessionshellcontroller.h"
#include "dbuslockagent.h"
#include "dbusshutdownagent.h"
#include "lockadaptor.h"
#include "shutdownadaptor.h"

#include <DDBusSender>

#include <QGuiApplication>
#include <QQmlContext>
#include <QtQuick>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    // crash catch
    init_sig_crash();

    QCommandLineParser cmdParser;
    cmdParser.addHelpOption();
    cmdParser.addVersionOption();

    QCommandLineOption lockFrame(QStringList() << "l" << "lockFrame", "show lockFrame");
    QCommandLineOption shutdownFrame(QStringList() << "s" << "shutdownFrame", "show shutdownFrame");
    cmdParser.addOption(lockFrame);
    cmdParser.addOption(shutdownFrame);
    cmdParser.process(app);

    QQmlApplicationEngine engine;
    SessionBaseModel sessionBaseModel;
    SessionShellController sessionShellController(&sessionBaseModel);
    SessionShellWorker *sessionShellWorker = new SessionShellWorker(&sessionBaseModel, &sessionShellController, &engine);

    // 直接将C++中实例化的对象传给qml使用
    engine.rootContext()->setContextProperty("SessionBaseModel", &sessionBaseModel);
    engine.rootContext()->setContextProperty("SessionShellController", &sessionShellController);
    engine.load(QUrl(QStringLiteral("qrc:///src/qml/main.qml")));
    if (engine.rootObjects().isEmpty()) return -1;

    QDBusConnection session = QDBusConnection::sessionBus();
    if (!session.registerService(DDESESSIONCC::LOCK_DBUS_NAME) || !session.registerService(DDESESSIONCC::SHUTDOWN_DBUS_NAME)) {
        qWarning() << "due-shell is running...";

        if (cmdParser.isSet(lockFrame)) {
            DDBusSender()
            .service(DDESESSIONCC::LOCK_DBUS_NAME)
            .interface(DDESESSIONCC::LOCK_DBUS_NAME)
            .path(DDESESSIONCC::LOCK_DBUS_PATH)
            .method("Show")
            .call();
            return 0;
        }

        if (cmdParser.isSet(shutdownFrame)) {
            DDBusSender()
            .service(DDESESSIONCC::SHUTDOWN_DBUS_NAME)
            .interface(DDESESSIONCC::SHUTDOWN_DBUS_NAME)
            .path(DDESESSIONCC::SHUTDOWN_DBUS_PATH)
            .method("Show")
            .call();
            return 0;
        }
    } else {
        qDebug() << "dbus registration success.";

        DBusLockAgent dbusLockAgent(&sessionBaseModel, &sessionShellController, sessionShellWorker);
        DBusLockAdaptor lockAdaptor(&dbusLockAgent);
        QDBusConnection::sessionBus().registerObject(DDESESSIONCC::LOCK_DBUS_PATH, &dbusLockAgent);

        DBusShutdownAgent dbusShutdownAgent(&sessionBaseModel, &sessionShellController);
        DBusShutdownAdaptor shutdownAdaptor(&dbusShutdownAgent);
        QDBusConnection::sessionBus().registerObject(DDESESSIONCC::SHUTDOWN_DBUS_PATH, &dbusShutdownAgent);

        // 实现需要传输给DBus服务的连接
        sessionBaseModel.setIsShow(true);
        QObject::connect(&sessionBaseModel, &SessionBaseModel::visibleChanged, &lockAdaptor, &DBusLockAdaptor::Visible);
        QObject::connect(&sessionShellController, &SessionShellController::authFinished, &lockAdaptor, &DBusLockAdaptor::Authenticate);

        return app.exec();
    }
}
