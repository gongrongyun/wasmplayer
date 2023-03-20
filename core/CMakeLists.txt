cmake_minimum_required(VERSION 3.9.0)
project(ffprobe_wasm)

execute_process(
        COMMAND
        ./${PROJECT_SOURCE_DIR}/env.sh
)

set(EMSCRIPTEN_ROOT_PATH /Users/rongyun.gong/third_party/emsdk/upstream/emscripten)
message("env:" $ENV{EMSCRIPTEN_ROOT_PATH})

include(cmake/Emscripten.cmake)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY build)
set(CMAKE_MODULE_PATH build)


# open this while using fleet IDE
#set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#set(EMSCRIPTEN_ROOT_PATH /root/third_party/emsdk/upstream/emscripten)

add_library(ffprobe_wasm player.cpp src/ffprobe.cpp src/ffprobe.h)

include_directories(
        ./third_party/ffmpeg/include
        # ./third_patry/emsdk/include
        ${EMSCRIPTEN_SYSROOT}/include
        ${PROJECT_SOURCE_DIR}
)

link_libraries(
        ./third_party/ffmpeg/lib/*
)

#
#target_link_libraries(
#  ffprobe_wasm
#  PRIVATE
#  ./third_party/ffmpeg/lib/libavformat.a
#  ./third_party/ffmpeg/lib/libavcodec.a
#  ./third_party/ffmpeg/lib/libavutil.a
#  ./third_party/ffmpeg/lib/libswscale.a
#)