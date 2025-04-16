[![CI](https://github.com/humbertodias/sdl-openmugen/actions/workflows/ci.yml/badge.svg)](https://github.com/humbertodias/sdl-openmugen/actions/workflows/ci.yml)
[![CD](https://github.com/humbertodias/sdl-openmugen/actions/workflows/cd.yml/badge.svg)](https://github.com/humbertodias/sdl-openmugen/actions/workflows/cd.yml)
![GitHub all releases](https://img.shields.io/github/downloads/humbertodias/sdl-openmugen/total)

OpenMugen
=========

Open source King of Fighters code

![openmugen](https://github.com/user-attachments/assets/5df94bec-4126-4a3e-b138-b7d426504ed8)

Controls

| Player | Movement |  Punch  |  Kick   | 
|:------:|:--------:|:-------:|:-------:|
| **P1** |  `ad`  | `y u` | `h j` |
| **P2** | `q` | `` | `` |

### Requisites
* sdl2
* sdl2-image
* sdl2-gfx
* sdl-1.2-compat

Ubuntu
```shell
sudo apt install libsdl2-dev libsdl-image2-dev libsdl2-gfx-dev libsdl1.2-compat
```
Build
```shell
cmake -Bbuild -DCMAKE_BUILD_TYPE=Release
cmake --build build
```
Run
```shell
./build/OpenMugen
```
