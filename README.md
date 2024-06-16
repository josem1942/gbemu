# gbemu
A GameBoy emulator written in C#, definitely not cycle accurate.

## Building
This is just a typical .NET project, so open the solution in Visual Studio to build, or use the command line and build, though do not forget to get the appropriate NuGet packages.

## How to use
After you open the program, drag and drop the boot ROM (dmg_boot.bin) to load the boot ROM to memory.
Then drag and drop the ROM you want to play to boot the game.
You can drag another ROM if you want to play another game, without having to redo the boot ROM again, the loading the boot ROM is just for first time launch.

Unfourtunately, not a lot of stuff is supported, theres still a lot left to be implemented (APU, MBCs, proper cycle timing, etc). Battery saves aren't supported yet either.
Main goal of this project was just to get a emulator booting up a game and get Pokemon running, perhaps I will pick it up in the future but to be honest there are better
emulators like SameBoy that work insanely better than this one. This is just a fun side project lol.

### Controls
Start - Enter/Return
Select - Backspace
DPad - Arrow Keys
A - X
B - Z

## Screenshots
![Pokemon Red](assets/pkmn.png)
![Donkey Kong Land](assets/dk.png)
![Super Mario Land](assets/mario.png)


