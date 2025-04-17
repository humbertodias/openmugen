
[![CI](https://github.com/humbertodias/sdl-openmugen/actions/workflows/ci.yml/badge.svg)](https://github.com/humbertodias/sdl-openmugen/actions/workflows/ci.yml)
[![CD](https://github.com/humbertodias/sdl-openmugen/actions/workflows/cd.yml/badge.svg)](https://github.com/humbertodias/sdl-openmugen/actions/workflows/cd.yml)
![GitHub all releases](https://img.shields.io/github/downloads/humbertodias/sdl-openmugen/total)


# OpenMugen

**OpenMugen** is a 2D fighting game engine inspired by the classic M.U.G.E.N, built using SDL2. 
The goal is to provide a simple, extensible, and cross-platform foundation for creating customizable fighting games.


![OpenMugen Gameplay](https://github.com/user-attachments/assets/e7a9a194-2dde-4d4f-b41a-24b9154fe2fb)



## 🕹 Controls

| Player | Movement | Punch     | Kick     |
|:------:|:--------:|:---------:|:--------:|
| **P1** | `A` `D`  | `Y` `U`   | `H` `J`  |
| **P2** | `Q`      | —         | —        |

> **Note:** Controls for Player 2 are still under development.


## 📦 Requirements

The following libraries must be installed:

- `SDL2`
- `SDL2_image`
- `SDL2_gfx`
- `SDL 1.2 compatibility layer`


## ⚙️ Build Instructions

### 🐧 Ubuntu/Debian

```bash
sudo apt install libsdl2-dev libsdl2-image-dev libsdl2-gfx-dev libsdl1.2-compat
```

To build the project:

```bash
cmake -Bbuild -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

## ▶️ Run

Once built, run the executable:

```bash
./build/OpenMugen
```

## 🚧 In Development

- [ ] Full Player 2 support  
- [ ] Custom animations  
- [ ] Improved collision system  
- [ ] Basic AI opponent  

## 🤝 Contributing

Pull requests are welcome! Feel free to open issues for bug reports or feature suggestions.


## 📄 License

This project is licensed under the [MIT License](LICENSE).
If you'd like, I can also help you add sections for custom characters, modding, or controller support. Want to include any of those?
