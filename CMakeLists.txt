cmake_minimum_required(VERSION 3.16)

project(test_embarque VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(APP_FILES
    main.cpp
)

set(UI_FILES
    ui/mainwindow.cpp
    ui/mainwindow.h
    ui/mainwindow.ui
)

set(CORE_FILES
    core/ISensor.h
    core/SensorBase.h
    core/SensorBase.cpp
    core/TemperatureSensor.cpp
    core/TemperatureSensor.h
    core/HumiditySensor.cpp
    core/HumiditySensor.h
    core/SensorManager.cpp
    core/SensorManager.h
    core/Measurement.h
)

set(PROJECT_SOURCES
    ${APP_FILES}
    ${UI_FILES}
    ${CORE_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(test_embarque
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        core/ISensor.h
        core/SensorBase.h core/SensorBase.cpp
        core/TemperatureSensor.h core/TemperatureSensor.cpp
        core/HumiditySensor.h core/HumiditySensor.cpp
        core/Measurement.h
        core/SensorManager.h core/SensorManager.cpp
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET test_embarque APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(test_embarque SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(test_embarque
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_include_directories(test_embarque PRIVATE core ui)

target_link_libraries(test_embarque PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.test_embarque)
endif()
set_target_properties(test_embarque PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS test_embarque
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(test_embarque)
endif()
