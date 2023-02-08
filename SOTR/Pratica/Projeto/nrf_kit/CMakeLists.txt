# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(hello_world)

set(IMGPROC_FILES src/imageProcessing/imageProcessing.h src/imageProcessing/imageProcessing.c )
set(CAB_FILES src/cab/cab.h src/cab/cab.c )

target_sources(app PRIVATE src/main.c ${IMGPROC_FILES} ${CAB_FILES} )
