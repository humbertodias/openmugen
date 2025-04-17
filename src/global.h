#ifndef __GLOBAL__H
#define __GLOBAL__H

//Version
#define VER     "Alpha 2"
#define OMTITLE "OpenMugen"

#include <SDL2/SDL.h>

//SDL_GFX libery


//standard include
#include <memory.h>
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdarg.h>  // For va_list, va_start, va_end

#ifdef __linux__
#define strcmpi strcasecmp
#endif
//using namespace std;

//used class
class CPlayer;
class CVirtualMachine;
class CGameTimer;
#include "define.h"
#include "structs.h"
#include "MemManager.h"

//Nate Pendeltons Tokenizer
#include "tokenizer.h"
#include "CmdManager.h"

#include "scale2x.h"
#include "Timer.h"
#include "SDL_framerate.h"
#include "VideoSystem.h"
#include "SDLManager.h"
#include "AirManager.h"
#include "SffManager.h"
#include "Stack.h"
#include "StateManager.h"
#include "ControllerExecuter.h"
#include "player.h"
#include "VM.h"
#include "StateParser.h"
#include "engine.h"
#include "game.h"

//functions
void PrintMessage(char *str,...);
void InitLogFile();

#endif
