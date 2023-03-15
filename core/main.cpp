#include <stdio.h>
#include "player.h"
#include <emscripten.h>

extern "C" {
    #include <libavutil/version.h>
}

static Player *player = nullptr;

EMSCRIPTEN_KEEPALIVE
extern "C" void prepare(char *url, int cb)
{
    if (player != nullptr)
    {
        free(player);
        player = nullptr;
    }
    player = new Player();
    player->prepare(url, cb);
}

EMSCRIPTEN_KEEPALIVE
extern "C" void play()
{
    if (player)
        player->play();
}

EMSCRIPTEN_KEEPALIVE
extern "C" void stop()
{
    if (player)
        player->stop();
}

EMSCRIPTEN_KEEPALIVE
extern "C" void destroy()
{
    if (player)
    {
        free(player);
        player = nullptr;
    }
}

int main()
{
    printf("ffmpeg version:%u\n", avcodec_version());
}