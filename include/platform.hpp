#pragma once

#include <SDL2/SDL.h>

class Platform {
private:
    SDL_Window   *window {};
    SDL_Renderer *renderer {};
    SDL_Texture  *texture {};

    SDL_AudioDeviceID device {};
    struct AudioData {
        float phase  = 0.0f;
        float freq   = 440.0f;
        bool playing = false;
    } audio;

    static const int SAMPLE_RATE = 44100;
    static void AudioCallback(void *userdata, Uint8 *stream, int len);

public:
    Platform(const char *title, int windowWidth, int windowHeight, int textureWidth, int textureHeight);
    ~Platform();

    void Update(const void *buffer, int pitch);
    bool ProcessInput(uint8_t *keys);
    void SetSound(bool on);
};
