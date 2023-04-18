cmake_minimum_required(VERSION 3.7)
project(due-shell LANGUAGES CXX)

SET(CMAKE_CXX_STANDARD 14)
SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTOUIC ON)
SET(CMAKE_AUTORCC ON)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -g -Wall -pthread -Wl,--as-need -fPIE -Wl,-E")
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -O0 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O3")

# --------------------- DBus Adaptor ----------------------
function(generation_dbus_xml class_dir class_file xml_name)
    execute_process(COMMAND qdbuscpp2xml ${class_file} -A -o ${xml_name}
                    WORKING_DIRECTORY ${class_dir})
endfunction(generation_dbus_xml)

function(generation_dbus_adapt xml class_name class_file option)
    execute_process(COMMAND qdbusxml2cpp ${option} -a ${class_file} -c ${class_name} ${xml}
                    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
endfunction(generation_dbus_adapt)

# 第一次运行时可借助该方法生成XML
generation_dbus_xml(
    ${CMAKE_SOURCE_DIR}/src/dbusinterface
    dbuslockagent.h
    ${CMAKE_SOURCE_DIR}/misc/com.deepin.dde.lockFront.xml
)

generation_dbus_xml(
    ${CMAKE_SOURCE_DIR}/src/dbusinterface
    dbusshutdownagent.h
    ${CMAKE_SOURCE_DIR}/misc/com.deepin.dde.shutdownFront.xml
)

# 第一次运行时可借助该方法生成adaptor
generation_dbus_adapt(
    ${CMAKE_SOURCE_DIR}/misc/com.deepin.dde.lockFront.xml
    DBusLockAdaptor
    ${CMAKE_SOURCE_DIR}/src/dbusinterface/lockadaptor
    -N
)

generation_dbus_adapt(
    ${CMAKE_SOURCE_DIR}/misc/com.deepin.dde.shutdownFront.xml
    DBusShutdownAdaptor
    ${CMAKE_SOURCE_DIR}/src/dbusinterface/shutdownadaptor
    -N
)
# ------------------------------------------------------------

FIND_PACKAGE(Qt5 COMPONENTS Core Gui Sql Network Quick DBus DBus X11Extras REQUIRED)
FIND_PACKAGE(PkgConfig REQUIRED)
FIND_PACKAGE(DtkWidget REQUIRED)
FIND_PACKAGE(DtkCMake REQUIRED)
PKG_CHECK_MODULES(DFrameworkDBus REQUIRED dframeworkdbus)
PKG_CHECK_MODULES(QGSettings REQUIRED gsettings-qt)
FIND_LIBRARY(PAM_LIBRARIES libpam.so)

SET(Qt_LIBS
    Qt5::Core
    Qt5::Gui
    Qt5::DBus
    Qt5::Quick
)

FILE(GLOB_RECURSE shell_files src/*.cpp src/*.h ./main.cpp ./due-shell.qrc)

ADD_EXECUTABLE(${PROJECT_NAME} main.cpp ${shell_files})
TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PUBLIC
    ${DFrameworkDBus_INCLUDE_DIRS}
    ${QGSettings_INCLUDE_DIRS}
    ${DtkWidget_INCLUDE_DIRS}
    src
    src/common
    src/libAuth
    src/dbusinterface
)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${Qt_LIBS} ${DFrameworkDBus_LIBRARIES} ${QGSettings_LIBRARIES} ${DtkWidget_LIBRARIES} ${PAM_LIBRARIES})
