#! /bin/bash

# go to ffmpeg dir
cd $(dirname $0)

# verify Emscripten version
emcc -v

# configure FFMpeg with Emscripten
# CFLAGS="-s USE_PTHREADS"
# LDFLAGS="$CFLAGS -s INITIAL_MEMORY=33554432" # 33554432 bytes = 32 MB
CONFIG_ARGS=(
  --target-os=none        # use none to prevent any os specific configurations
  --arch=x86_32           # use x86_32 to achieve minimal architectural optimization
  --enable-cross-compile  # enable cross compile
  --disable-x86asm        # disable x86 asm
  --disable-inline-asm    # disable inline asm
  --disable-stripping     # disable stripping
  --disable-programs      # disable programs build (incl. ffplay, ffprobe & ffmpeg)
  --disable-doc           # disable doc
  # --extra-cflags="$CFLAGS"
  # --extra-cxxflags="$CFLAGS"
  # --extra-ldflags="$LDFLAGS"
  --nm="llvm-nm"
  --ar=emar
  --ranlib=emranlib
  --cc=emcc
  --cxx=em++
  --objcc=emcc
  --dep-cc=emcc

  --prefix=/root/code/ffprobe-wasm/core/third_party/ffmpeg
)
emconfigure ./configure "${CONFIG_ARGS[@]}"

make -j2

make install

echo "====== Finish ======"