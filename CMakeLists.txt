#
#    gchat_linux An un-official Google Chat client for linux
#
#    Copyright (C) 2018  Samith Sandanayake <samithdisal@gmail.com>
#
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <https://www.gnu.org/licenses/>.
#

cmake_minimum_required(VERSION 3.10)
project(gchat_linux C)

set(MAJOR_VERSION 1)
set(MINOR_VERSION 2)
set(PATCH_VERSION 0)

# Use the package PkgConfig to detect GTK+ headers/library files
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0 webkit2gtk-4.0)

# Setup CMake to use GTK+, tell the compiler where to look for headers
# and to the linker where to look for libraries
include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})

# Add other flags to the compiler
add_definitions(${GTK3_CFLAGS_OTHER})

set(CMAKE_C_STANDARD 99)

add_executable(gchat_linux main.c external.c notification.c preferences.c preferences.h)

# Link the target to the GTK+ libraries
target_link_libraries(gchat_linux ${GTK3_LIBRARIES})

install(TARGETS gchat_linux DESTINATION bin)
install(FILES gchat_linux.desktop DESTINATION share/applications)
install(FILES gchat_linux.desktop DESTINATION /etc/xdg/autostart)
install(FILES gchat_linux.xpm DESTINATION share/pixmaps)

# Package
set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_VERSION_MAJOR ${MAJOR_VERSION})
set(CPACK_PACKAGE_VERSION_MINOR ${MINOR_VERSION})
set(CPACK_PACKAGE_VERSION_PATCH ${PATCH_VERSION})
set(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}_${MAJOR_VERSION}.${MINOR_VERSION}.${CPACK_PACKAGE_VERSION_PATCH}")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libwebkit2gtk-4.0-37,gir1.2-webkit2-4.0")
set(CPACK_DEBIAN_PACKAGE_DESCRIPTION "A Google Chat client for linux")
set(CPACK_PACKAGE_CONTACT "Samith Sandanayake samithdisal@gmail.com")
set(CPACK_DEBIAN_PACKAGE_SECTION "network")
set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://github.com/samithdisal/gchat_linux")
#set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${CMAKE_CURRENT_SOURCE_DIR}/Debian/postinst")

include(CPack)