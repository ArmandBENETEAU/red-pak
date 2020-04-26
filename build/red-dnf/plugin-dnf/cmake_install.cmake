# Install script for directory: /home/fulup/Workspace/DNF/red-pak/red-dnf/plugin-dnf

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/opt")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "0")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/opt/lib64/python3.7/site-packages/dnf-plugins/red-install.py;/opt/lib64/python3.7/site-packages/dnf-plugins/red-list.py;/opt/lib64/python3.7/site-packages/dnf-plugins/red-manager.py;/opt/lib64/python3.7/site-packages/dnf-plugins/red-remove.py;/opt/lib64/python3.7/site-packages/dnf-plugins/red-repolist.py;/opt/lib64/python3.7/site-packages/dnf-plugins/red-search.py")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/opt/lib64/python3.7/site-packages/dnf-plugins" TYPE FILE FILES
    "/home/fulup/Workspace/DNF/red-pak/red-dnf/plugin-dnf/red-install.py"
    "/home/fulup/Workspace/DNF/red-pak/red-dnf/plugin-dnf/red-list.py"
    "/home/fulup/Workspace/DNF/red-pak/red-dnf/plugin-dnf/red-manager.py"
    "/home/fulup/Workspace/DNF/red-pak/red-dnf/plugin-dnf/red-remove.py"
    "/home/fulup/Workspace/DNF/red-pak/red-dnf/plugin-dnf/red-repolist.py"
    "/home/fulup/Workspace/DNF/red-pak/red-dnf/plugin-dnf/red-search.py"
    )
endif()

