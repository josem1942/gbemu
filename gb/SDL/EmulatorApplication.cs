using gbemu.GameBoy;
using System.Runtime.InteropServices;
using static SDL2.SDL;

namespace gbemu.SDL;

public unsafe class EmulatorApplication : IDisposable
{
    GBInstance gb;

    nint window;
    nint renderer;
    nint texture;
    uint audio;

    bool run;

    public EmulatorApplication()
    {
        gb = new GBInstance();

        Init();
    }

    private void Init()
    {
        SDL_Init(SDL_INIT_EVERYTHING);

        window = SDL_CreateWindow("DMG0", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 160 * 4, 144 * 4, SDL_WindowFlags.SDL_WINDOW_SHOWN | SDL_WindowFlags.SDL_WINDOW_RESIZABLE);
        renderer = SDL_CreateRenderer(window, -1, SDL_RendererFlags.SDL_RENDERER_ACCELERATED | SDL_RendererFlags.SDL_RENDERER_PRESENTVSYNC);
        texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24, (int)SDL_TextureAccess.SDL_TEXTUREACCESS_STREAMING, 160, 144);

        SDL_AudioSpec desired = new SDL_AudioSpec();
        desired.freq = 44100;
        desired.channels = 2;
        desired.samples = 0;

        SDL_AudioSpec got = new SDL_AudioSpec();

        string dev = SDL_GetAudioDeviceName(1, 0);
        audio = SDL_OpenAudioDevice(dev, 0, ref desired, out got, 0);
        SDL_PauseAudioDevice(audio, 0);

        run = false;

        string boot = "E:\\bios\\gb\\dmg_boot.bin";
        gb.LoadBootROM(File.ReadAllBytes(boot));
    }

    public void Run()
    {
        bool exit = false;

        IntPtr audioBuffer = Marshal.AllocHGlobal(gb.APU.AudioBuffer.Length);
        SDL_Event e;

        while (!exit)
        {
            while (SDL_PollEvent(out e) != 0)
            {
                switch (e.type)
                {
                    case SDL_EventType.SDL_QUIT:
                        exit = true;
                        break;
                    case SDL_EventType.SDL_KEYDOWN:
                        switch (e.key.keysym.sym)
                        {
                            case SDL_Keycode.SDLK_DOWN:
                                gb.Joypad.ButtonDown(GameBoy.IO.JoypadInput.Down);
                                break;
                            case SDL_Keycode.SDLK_UP:
                                gb.Joypad.ButtonDown(GameBoy.IO.JoypadInput.Up);
                                break;
                            case SDL_Keycode.SDLK_LEFT:
                                gb.Joypad.ButtonDown(GameBoy.IO.JoypadInput.Left);
                                break;
                            case SDL_Keycode.SDLK_RIGHT:
                                gb.Joypad.ButtonDown(GameBoy.IO.JoypadInput.Right);
                                break;
                            case SDL_Keycode.SDLK_x:
                                gb.Joypad.ButtonDown(GameBoy.IO.JoypadInput.A);
                                break;
                            case SDL_Keycode.SDLK_z:
                                gb.Joypad.ButtonDown(GameBoy.IO.JoypadInput.B);
                                break;
                            case SDL_Keycode.SDLK_RETURN:
                                gb.Joypad.ButtonDown(GameBoy.IO.JoypadInput.Start);
                                break;
                            case SDL_Keycode.SDLK_BACKSPACE:
                                gb.Joypad.ButtonDown(GameBoy.IO.JoypadInput.Select);
                                break;
                        }
                        break;
                    case SDL_EventType.SDL_KEYUP:
                        switch (e.key.keysym.sym)
                        {
                            case SDL_Keycode.SDLK_DOWN:
                                gb.Joypad.ButtonUp(GameBoy.IO.JoypadInput.Down);
                                break;
                            case SDL_Keycode.SDLK_UP:
                                gb.Joypad.ButtonUp(GameBoy.IO.JoypadInput.Up);
                                break;
                            case SDL_Keycode.SDLK_LEFT:
                                gb.Joypad.ButtonUp(GameBoy.IO.JoypadInput.Left);
                                break;
                            case SDL_Keycode.SDLK_RIGHT:
                                gb.Joypad.ButtonUp(GameBoy.IO.JoypadInput.Right);
                                break;
                            case SDL_Keycode.SDLK_x:
                                gb.Joypad.ButtonUp(GameBoy.IO.JoypadInput.A);
                                break;
                            case SDL_Keycode.SDLK_z:
                                gb.Joypad.ButtonUp(GameBoy.IO.JoypadInput.B);
                                break;
                            case SDL_Keycode.SDLK_RETURN:
                                gb.Joypad.ButtonUp(GameBoy.IO.JoypadInput.Start);
                                break;
                            case SDL_Keycode.SDLK_BACKSPACE:
                                gb.Joypad.ButtonUp(GameBoy.IO.JoypadInput.Select);
                                break;
                        }
                        break;
                    case SDL_EventType.SDL_DROPFILE:
                        gb.LoadROM(File.ReadAllBytes(Marshal.PtrToStringUTF8(e.drop.file)));
                        run = true;
                        break;
                }
            }

            if (run)
                gb.ExecuteFrame();

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
            SDL_RenderClear(renderer);

            IntPtr pixels = IntPtr.Zero;
            SDL_LockTexture(texture, IntPtr.Zero, out pixels, out _);
            Marshal.Copy(gb.PPU.OutputBuffer, 0, pixels, gb.PPU.OutputBuffer.Length);
            SDL_UnlockTexture(texture);

            SDL_RenderCopy(renderer, texture, IntPtr.Zero, IntPtr.Zero);
            SDL_RenderPresent(renderer);


            Marshal.Copy(gb.APU.AudioBuffer, 0, audioBuffer, gb.APU.AudioBuffer.Length);

            if (gb.APU.AudioBuffer.Length > 0)
            {
                // Copy the audio buffer to SDL audio device
                // This is a simplified example; ensure proper audio handling and buffer synchronization
                SDL_QueueAudio(audio, audioBuffer, (uint)gb.APU.AudioBuffer.Length);
            }
        }
    }

    public void Dispose()
    {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyRenderer(window);
    }

    ~EmulatorApplication()
    {
        Dispose();
    }
}
