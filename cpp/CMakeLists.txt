cmake_minimum_required(VERSION 3.5)

project(gomoku VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_FLAGS "-Werror -Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# set(DCMAKE_CXX_FLAGS "-pg")
# set(DCMAKE_EXE_LINKER_FLAGS "-pg")
# set(DCMAKE_SHARED_LINKER_FLAGS "-pg")

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check https://doc.qt.io/qt/deployment-android.html for more information.
# They need to be set before the find_package( ...) calls below.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

set(PROJECT_SOURCES
        src/main.cpp
        src/mainwindow.cpp
        src/mainwindow.ui
        src/Startup.cpp
        include/Startup.hpp
        src/Startup.ui
        src/Scene.cpp
        src/Token.cpp
        include/Token.hpp
        include/Scene.hpp
        include/mainwindow.hpp

        src/board.cpp
        src/game.cpp
        include/game.hpp
        include/board.hpp
        src/Patterns.cpp
        include/Patterns.hpp
        include/Board2_0.h
  )

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
  qt_add_executable(${PROJECT_NAME}
          MANUAL_FINALIZATION
          ${PROJECT_SOURCES}
          )
else()
  if(ANDROID)
    add_library(${PROJECT_NAME} SHARED
            ${PROJECT_SOURCES}
            )
  else()
    add_executable(${PROJECT_NAME}
            ${PROJECT_SOURCES}
            )
  endif()
endif()

include_directories(include)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        )

add_executable(test_extract
        tests/test_extract.cpp
        )

ADD_CUSTOM_TARGET(copy_runtime_dep ALL)
# we don't want to copy if we're building in the source dir
if (NOT CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
  # list of files for which we add a copy rule
  file(GLOB_RECURSE RES_CL images/*)
  foreach(item IN LISTS RES_CL)
    get_filename_component(barename ${item} NAME)
    message(STATUS "${item} -> ${CMAKE_CURRENT_BINARY_DIR}/images/${barename}")
    add_custom_command(TARGET copy_runtime_dep POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy "${item}" "${CMAKE_CURRENT_BINARY_DIR}/images/${barename}"
            DEPENDS "${item}"
            )
  endforeach()
endif()
ADD_DEPENDENCIES(${PROJECT_NAME} copy_runtime_dep)


if(QT_VERSION_MAJOR EQUAL 6)
  qt_finalize_executable(${PROJECT_NAME})
endif()
