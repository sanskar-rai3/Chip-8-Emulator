#include "platform.hpp"

#include <iostream>
#include <cstdlib>
#include <cmath>

void Platform::AudioCallback(void *userdata, Uint8 *stream, int len) {
    auto *audio   = (Platform::AudioData *)userdata;
    float *buffer = (float *)stream;
    int samples   = len / sizeof(float);

    for (int i = 0; i < samples; ++i) {
        if (audio->playing) {
            buffer[i] = std::sin(audio->phase) * 0.2f;

            audio->phase += 2.0 * M_PI * audio->freq / SAMPLE_RATE;
            if (audio->phase > 2.0 * M_PI) {
                audio->phase -= 2.0 * M_PI;
            }
        }
        else {
            buffer[i] = 0;
        }
    }
}

Platform::Platform(const char *title, int windowWidth, int windowHeight, int textureWidth, int textureHeight) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        std::cerr << "error: Failed to initialize SDL\n";
        std::exit(EXIT_FAILURE);
    }

    window = SDL_CreateWindow(
        title,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        windowWidth,
        windowHeight,
        SDL_WINDOW_SHOWN
    );
    if (!window) {
        std::cerr << "error: Failed to create window\n";
        std::exit(EXIT_FAILURE);
    }

    renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED
    );
    if (!renderer) {
        std::cerr << "error: Failed to create renderer\n";
        std::exit(EXIT_FAILURE);
    }

    texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING,
        textureWidth,
        textureHeight
    ); 
    if (!texture) {
        std::cerr << "error: Failed to create texture\n";
        std::exit(EXIT_FAILURE);
    }

    SDL_AudioSpec spec;
    SDL_zero(spec);

    spec.freq     = SAMPLE_RATE;
    spec.format   = AUDIO_F32SYS;
    spec.channels = 1;
    spec.samples  = 1024;
    spec.callback = AudioCallback;
    spec.userdata = &audio;

    device = SDL_OpenAudioDevice(nullptr, 0, &spec, nullptr, 0);
    SDL_PauseAudioDevice(device, 0);
}

Platform::~Platform() {
    SDL_CloseAudioDevice(device);

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
}

void Platform::Update(const void *buffer, int pitch) {
    SDL_UpdateTexture(texture, nullptr, buffer, pitch);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

bool Platform::ProcessInput(uint8_t *keys) {
    bool quit = false;

    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT: quit = true; break;
            
            case SDL_KEYDOWN: {
                switch (event.key.keysym.sym) {
						case SDLK_ESCAPE: quit = true; break;

						case SDLK_x: keys[0]   = 1; break;
						case SDLK_1: keys[1]   = 1; break;
						case SDLK_2: keys[2]   = 1; break;
						case SDLK_3: keys[3]   = 1; break;
						case SDLK_q: keys[4]   = 1; break;
						case SDLK_w: keys[5]   = 1; break;
						case SDLK_e: keys[6]   = 1; break;
						case SDLK_a: keys[7]   = 1; break;
						case SDLK_s: keys[8]   = 1; break;
						case SDLK_d: keys[9]   = 1; break;
						case SDLK_z: keys[0xA] = 1; break;
						case SDLK_c: keys[0xB] = 1; break;
						case SDLK_4: keys[0xC] = 1; break;
						case SDLK_r: keys[0xD] = 1; break;
						case SDLK_f: keys[0xE] = 1; break;
						case SDLK_v: keys[0xF] = 1; break;
                }

                break;
            }

            case SDL_KEYUP: {
                switch (event.key.keysym.sym) {
						case SDLK_x: keys[0]   = 0; break;
						case SDLK_1: keys[1]   = 0; break;
						case SDLK_2: keys[2]   = 0; break;
						case SDLK_3: keys[3]   = 0; break;
						case SDLK_q: keys[4]   = 0; break;
						case SDLK_w: keys[5]   = 0; break;
						case SDLK_e: keys[6]   = 0; break;
						case SDLK_a: keys[7]   = 0; break;
						case SDLK_s: keys[8]   = 0; break;
						case SDLK_d: keys[9]   = 0; break;
						case SDLK_z: keys[0xA] = 0; break;
						case SDLK_c: keys[0xB] = 0; break;
						case SDLK_4: keys[0xC] = 0; break;
						case SDLK_r: keys[0xD] = 0; break;
						case SDLK_f: keys[0xE] = 0; break;
						case SDLK_v: keys[0xF] = 0; break;
                }

                break;
            }
        }

    }

    return quit;
}

void Platform::SetSound(bool on) {
    audio.playing = on;
}