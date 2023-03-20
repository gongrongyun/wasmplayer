cmake_minimum_required(VERSION 3.9.0)
project(ffprobe_wasm)

# 准备环境变量
execute_process(
        COMMAND
        bash -c
        "source ${PROJECT_SOURCE_DIR}/env.sh"
        OUTPUT_VARIABLE
        ENVIRONMENT
)
string(REPLACE "\n" ";" ENVIRONMENT_LIST ${ENVIRONMENT})
foreach(ENV_VAR ${ENVIRONMENT_LIST})
    string(REPLACE "=" ";" ENV_VAR_LIST ${ENV_VAR})
    list(GET ENV_VAR_LIST 0 ENV_VAR_NAME)
    list(GET ENV_VAR_LIST 1 ENV_VAR_VALUE)
    set(${ENV_VAR_NAME} ${ENV_VAR_VALUE})
endforeach()

# 引入 emscripten cmake
include(cmake/Emscripten.cmake)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY build)
set(CMAKE_MODULE_PATH build)


# open this while using fleet IDE
#set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(ffprobe_wasm player.cpp src/ffprobe.cpp src/ffprobe.h)

include_directories(
  ./third_party/ffmpeg/include
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