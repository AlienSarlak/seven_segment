# Copyright (c) 2024 Ali Sarlak
# SPDX-License-Identifier: Apache-2.0

zephyr_library()
zephyr_library_sources_ifdef(CONFIG_SEVENSEGMENT seven_segment.c)
zephyr_include_directories("${CMAKE_SOURCE_DIR}/../../include")
