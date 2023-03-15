cmake_minimum_required(VERSION 3.0.0)
project(ffprobe_wasm VERSION 0.1.0)

set(EMSCRIPTEN_ROOT_PATH /root/third_party/emsdk/upstream/emscripten)
include(cmake/Emscripten.cmake)

add_library(ffprobe_wasm player.cpp)

include_directories(
  ./third_party/ffmpeg/include
  # /root/third_party/emsdk/upstream/emscripten/system/include

  ${PROJECT_SOURCE_DIR}
)

target_include_directories(
  ffprobe_wasm
  PUBLIC
  ./third_party/ffmpeg/include
  # /root/third_party/emsdk/upstream/emscripten/system/include

  ${PROJECT_SOURCE_DIR}
)

target_link_libraries(
  ffprobe_wasm
  PRIVATE
  ./third_party/ffmpeg/lib/libavformat.a
  ./third_party/ffmpeg/lib/libavcodec.a
  ./third_party/ffmpeg/lib/libavutil.a
  ./third_party/ffmpeg/lib/libswscale.a
)