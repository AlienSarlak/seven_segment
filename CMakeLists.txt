# Copyright (c) 2024
# SPDX-License-Identifier: Apache-2.0
cmake_minimum_required(VERSION 3.20.0)
set(ZEPHYR_EXTRA_MODULES "${CMAKE_SOURCE_DIR}/modules")
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(test VERSION 1.0)

target_sources(app PRIVATE src/main.c)
zephyr_syscall_include_directories("${CMAKE_SOURCE_DIR}/modules/sevenseg/include")
zephyr_include_directories("${CMAKE_SOURCE_DIR}/modules/sevenseg/include")
