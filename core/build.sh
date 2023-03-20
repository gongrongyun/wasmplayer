#! /bin/bash
export TOTAL_MEMORY=52428800 #50MB

PWD=$(pwd)

FFMPEG_INCLUDE_DIR=${PWD}/third_party/ffmpeg/include
FFMPEG_LIB_DIR=${PWD}/third_party/ffmpeg/lib
PLAYER_INCLUDE_DIR=.
PLAYER_LIB_DIR=./build

emcc ./main.cpp \
 -I ${PLAYER_INCLUDE_DIR} ${PLAYER_LIB_DIR}/libffprobe_wasm.a \
 -I ${FFMPEG_INCLUDE_DIR} ${FFMPEG_LIB_DIR}/libavformat.a ${FFMPEG_LIB_DIR}/libavcodec.a ${FFMPEG_LIB_DIR}/libavutil.a ${FFMPEG_LIB_DIR}/libswscale.a ${FFMPEG_LIB_DIR}/libswresample.a ${FFMPEG_LIB_DIR}/libavutil.a \
 -O2 \
 -lwebsocket.js \
 -s ALLOW_TABLE_GROWTH=1 \
 -s ALLOW_MEMORY_GROWTH=1 \
 -s MODULARIZE=1 \
 -s EXPORT_NAME="WASMPlayer" \
 -s EXPORTED_RUNTIME_METHODS='["ccall", "cwrap", "dynCall", "allocate", "addFunction", "UTF8ToString" , "intArrayFromString", "ALLOC_NORMAL"]' \
 -s TOTAL_MEMORY=${TOTAL_MEMORY} \
 -o glue.js
