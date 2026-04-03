cmake_minimum_required(VERSION 3.16)

project(AndroidCanController VERSION 0.1 LANGUAGES CXX)

# Export compile commands for clangd/IDE so headers like QObject are resolvable
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Quick Bluetooth )
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Quick Bluetooth )

# Build common library from subdir (common/CMakeLists.txt)
add_subdirectory(common)

# Group source files for clarity
set(SRC_MAIN
    main.cpp
    qml.qrc
    resources.qrc
)

# QML-related sources
set(SRC_QML
    qml/controller/QMLGamepad.h
    qml/controller/QMLGamepad.cpp
)

set(PROJECT_SOURCES
    ${SRC_MAIN}
    ${SRC_QML}
)

# Add platform-specific sources
if(ANDROID)
  find_package(AndroidExtras)
    list(APPEND PROJECT_SOURCES
        android/qt/AndroidGamepadBridge.h
        android/qt/AndroidGamepadBridge.cpp
        android/qt/AndroidPermission.h
        android/qt/AndroidPermission.cpp
    )

elseif(UNIX AND NOT APPLE)  # Linux only (NOT Android)
    list(APPEND PROJECT_SOURCES
        sdl/SDLGamepadBridge.h
        sdl/SDLGamepadBridge.cpp
    )
endif()

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(AndroidCanController
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )

    # Android package source directory for Qt 6 (also set the target property)
    if(ANDROID)
        set_property(TARGET AndroidCanController PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
                     "${ANDROID_PACKAGE_SOURCE_DIR}")

        # Ensure the android/qt folder is on the include path so headers like AndroidPermissions.h
        # can be found by the compiler and visible in IDE project views.
        target_include_directories(AndroidCanController PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/android/qt")

        # Add the header to the target sources so IDEs show it in the project tree
        target_sources(AndroidCanController PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/android/qt/AndroidPermissions.h")
    endif()
else()
    if(ANDROID)
        add_library(AndroidCanController SHARED
            ${PROJECT_SOURCES}
        )
        set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(AndroidCanController
          ${PROJECT_SOURCES}
        )
    endif()
endif()

# Move target_compile_definitions AFTER target creation
if(ANDROID)
    target_compile_definitions(AndroidCanController PRIVATE ANDROID_PLATFORM)
elseif(UNIX AND NOT APPLE)  # Linux only
    target_compile_definitions(AndroidCanController PRIVATE LINUX_PLATFORM)
endif()

target_link_libraries(AndroidCanController
  PRIVATE
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Quick
  Qt${QT_VERSION_MAJOR}::Bluetooth
  common_lib
  ble_lib
)


if(ANDROID)
  target_link_libraries(AndroidCanController
    PRIVATE
    Qt${QT_VERSION_MAJOR}::AndroidExtras
  )
endif()

# ONLY link SDL2 for actual Linux desktop builds (NOT Android)
if(UNIX AND NOT APPLE AND NOT ANDROID)  # Added NOT ANDROID condition
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(SDL2 REQUIRED sdl2)
    if(SDL2_FOUND)
        target_link_libraries(AndroidCanController PRIVATE ${SDL2_LIBRARIES})
        target_include_directories(AndroidCanController PRIVATE ${SDL2_INCLUDE_DIRS})
    endif()
endif()

if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.AndroidCanController)
endif()
set_target_properties(AndroidCanController PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS AndroidCanController
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_import_qml_plugins(AndroidCanController)
    qt_finalize_executable(AndroidCanController)
endif()
