
[![CI](https://github.com/humbertodias/sdl-openmugen/actions/workflows/ci.yml/badge.svg)](https://github.com/humbertodias/sdl-openmugen/actions/workflows/ci.yml)
[![CD](https://github.com/humbertodias/sdl-openmugen/actions/workflows/cd.yml/badge.svg)](https://github.com/humbertodias/sdl-openmugen/actions/workflows/cd.yml)
![GitHub all downloads](https://img.shields.io/github/downloads/humbertodias/sdl-openmugen/total)


# 🥋 OpenMugen

**OpenMugen** is a free and open-source 2D fighting game engine inspired by the classic **M.U.G.E.N**, built with **SDL2**.  
Its main goal is to offer a **simple**, **extensible**, and **cross-platform** foundation for creating customizable fighting games.


[**Play**](https://humbertodias.github.io/sdl-openmugen/) directly in your browser 


## 🎮 Controls

| Player | Movement     | Punch       | Kick       |
|--------|--------------|-------------|------------|
| **P1** | `A` / `D`    | `Y`, `U`    | `H`, `J`   |
| **P2** | `←` / `→`    | -    | -   |

> ⚠️ **Note:** Player 2 controls are **under development**.

---

## 🖥️ Desktop Version

### Requirements (Ubuntu/Debian)

```bash
sudo apt install libsdl2-dev libsdl2-image-dev libsdl2-gfx-dev
```

### Build Instructions

```bash
cmake -Bbuild -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

Then run:

```bash
./build/OpenMugen
```
<img alt="OpenMugen Gameplay" src="https://github.com/user-attachments/assets/e7a9a194-2dde-4d4f-b41a-24b9154fe2fb" />


## 🚧 Roadmap

- [ ] Full Player 2 support  
- [ ] Custom animations and character scripting  
- [ ] Enhanced collision and hitbox system  
- [ ] Basic AI opponent  
- [ ] Sound and music engine


## 🤝 Contributing

Pull requests are welcome!  
If you find a bug or have a feature suggestion, feel free to [open an issue](https://github.com/humbertodias/openmugen/issues).


## 📄 License

OpenMugen is licensed under the [MIT License](LICENSE).
