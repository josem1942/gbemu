#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <SDL3/SDL.h>

#include "gb/gb.h"
#include "debug/disassembler.h"
#include "debug/debug.h"

#ifdef _WIN32
const char JSON_OPCODES[] = "..\\..\\..\\..\\Downloads\\ops.json";
const char ROM[] = "..\\..\\..\\..\\Downloads\\cpu_instrs.gb";
const char BIOS[] = "E:\\bios\\gb\\dmg_boot.bin";
#else
const char JSON_OPCODES[] = "../../../Downloads/ops.json";
const char ROM[] = "../../../Downloads/cpu_instrs.gb";
const char BIOS[] = "../../../Downloads/dmg_boot.bin";
#endif

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    srand(time(0));

    gb_gameboy *gb = gb_gameboy_init();
    disasm_opcodes *ops = disasm_init(JSON_OPCODES);
    SDL_Window *window = SDL_CreateWindow("GameBoy", 160 * 5, 144 * 5, SDL_WINDOW_RESIZABLE);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, nullptr);
    SDL_Texture *screen_tx = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 160, 144);

    struct {
        uint8_t a;
        uint8_t b;
        uint8_t g;
        uint8_t r;
    } buffer[160 * 144];

    SDL_Event event;
    bool quit = false;

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_QUIT:
                    quit = true;
                    break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        for (int i = 0; i < sizeof(buffer); i++) {
            buffer[i].r = rand() % 255;
            buffer[i].g = rand() % 255;
            buffer[i].b = rand() % 255;
            buffer[i].a = 255;
        }

        int pitch;
        uint32_t *locked;
        SDL_LockTexture(screen_tx, nullptr, (void**)&locked, &pitch);
        SDL_memcpy(locked, buffer, 160 * 144 * 4);
        SDL_UnlockTexture(screen_tx);

        SDL_RenderTexture(renderer, screen_tx, nullptr, nullptr);
        SDL_RenderPresent(renderer);
    }

    SDL_Quit();
    disasm_destroy(ops);
    gb_gameboy_destroy(gb);

    return 0;
}

/*
gb_gameboy *gb = gb_gameboy_init();
disasm_opcodes *ops = disasm_init(JSON_OPCODES);

gb_gameboy_load_rom(gb, ROM);
gb_gameboy_load_bootrom(gb, BIOS);

char assembly[50] = {};

while (1) {
gb_gameboy_clock_cpu(gb);
}

disasm_destroy(ops);
gb_gameboy_destroy(gb);
*/
