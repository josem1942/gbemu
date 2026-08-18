#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <SDL3/SDL.h>

#include "gb/gb.h"
#include "debug/disassembler.h"
#include "debug/debug.h"

#ifdef _WIN32
const char JSON_OPCODES[] = "..\\..\\..\\..\\Downloads\\ops.json";
const char BIOS[] = "E:\\bios\\gb\\dmg_boot.bin";
#else
const char JSON_OPCODES[] = "../../../Downloads/ops.json";
const char BIOS[] = "../../../Downloads/dmg_boot.bin";
#endif

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    srand(time(0));

    gb_gameboy *gb = gb_gameboy_init();
    disasm_opcodes *ops = disasm_init(JSON_OPCODES);
    char assembly[50] = {};

    gb_gameboy_load_bootrom(gb, BIOS);
    gb_cpu_skip_bootrom(gb->cpu);

    SDL_Window *window = SDL_CreateWindow("GameBoy", 160 * 5, 144 * 5, SDL_WINDOW_RESIZABLE);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, nullptr);
    SDL_Texture *screen_tx = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 160, 144);
    SDL_SetTextureScaleMode(screen_tx, SDL_SCALEMODE_NEAREST);

    uint32_t buffer[160 * 144];

    for (int i = 0; i < 160 * 144; i++) {
        buffer[i] = 0;
    }

    SDL_Event event;
    bool quit = false;
    bool loop = false;

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_QUIT:
                    quit = true;
                    break;
                case SDL_EVENT_DROP_FILE:
                    gb_gameboy_load_rom(gb, event.drop.data);
                    break;
                case SDL_EVENT_KEY_DOWN:
                    if (event.key.key == SDLK_D) {
                        printf("\x1b[1;0H");
                        debug_print_registers(gb);
                        if (gb->cpu->instruction_cycles == 0) {
                            printf("\x1b[6;0H");
                            debug_print_assembly(gb, ops, assembly);
                            printf("%s                        ", assembly);
                        }
                        gb_gameboy_clock_cpu(gb);
                    }
                    if (event.key.key == SDLK_S) {
                        do {
                            gb_gameboy_clock_cpu(gb);
                        }
                        while (gb->cpu->instruction_cycles != 0);
                    }
                    else if (event.key.key == SDLK_Q) {
                        quit = true;
                    }
                    else if (event.key.key == SDLK_L) {
                        loop = !loop;
                    }
                    else if (event.key.key == SDLK_T) {
                        for (int sy = 0; sy < 18; sy++) {
                            for (int sx = 0; sx < 20; sx++) {
                                uint8_t tile[16];
                                for (int i = 0; i < 16; i++) {
                                    tile[i] = gb_bus_read(gb->bus, ((0x8000 + (sy * 0x140))  + (sx * 16)) + i);
                                }

                                for (int y = 0; y < 8; y++) {
                                    uint8_t lo = tile[y * 2];
                                    uint8_t hi = tile[y * 2 + 1];
                                    for (int x = 7; x >= 0; x--) {
                                        uint8_t lsb = (lo >> x) & 0x1;
                                        uint8_t msb = (hi >> x) & 0x1;
                                        uint8_t col = (msb << 1) | lsb;
                                        int index = (abs(7-x) + (sx*8)) + 160 * (y + (sy * 8));
                                        switch (col) {
                                            case 0:
                                                buffer[index] = 0x000000FF;
                                                break;
                                            case 1:
                                                buffer[index] = 0x404040FF;
                                                break;
                                            case 2:
                                                buffer[index] = 0xA0A0A0FF;
                                                break;
                                            default:
                                                buffer[index] = 0xFFFFFFFF;
                                                break;
                                        }
                                    }
                                }
                            }
                        }
                    }
                    break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        if (loop) {
            gb_gameboy_clock_frame(gb);
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
