#ifndef __GLOBAL__H
#define __GLOBAL__H

// Version
#define VER "Alpha 2"
#define OMTITLE "OpenMugen"

#include <SDL2/SDL.h>

// SDL_GFX libery

// standard include
#include <ctype.h>
#include <math.h>
#include <memory.h>
#include <stdarg.h>
#include <stdarg.h>  // For va_list, va_start, va_end
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __linux__
#define strcmpi strcasecmp
#endif
// using namespace std;

// used class
class CPlayer;
class CVirtualMachine;
class CGameTimer;
#include "MemManager.h"
#include "define.h"
#include "structs.h"

// Nate Pendeltons Tokenizer
#include "CmdManager.h"
#include "tokenizer.h"

#include "AirManager.h"
#include "ControllerExecuter.h"
#include "SDLManager.h"
#include "SDL_framerate.h"
#include "SffManager.h"
#include "Stack.h"
#include "StateManager.h"
#include "StateParser.h"
#include "Timer.h"
#include "VM.h"
#include "VideoSystem.h"
#include "engine.h"
#include "game.h"
#include "player.h"
#include "scale2x.h"

// functions
void PrintMessage(char* str, ...);

void InitLogFile();

#endif
