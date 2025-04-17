
/*

 SDL_framerate: framerate manager

 LGPL (c) A. Schiffler

 */

#include "SDL_framerate.h"


// Initialize the framerate manager
void SDL_initFramerate(FPSmanager* manager)
{
	manager->framecount = 0;
	manager->rate = FPS_DEFAULT;
	manager->rateticks = (1000.0f / (float)FPS_DEFAULT);
	manager->lastticks = SDL_GetTicks();
}

// Set framerate in Hz
int SDL_setFramerate(FPSmanager* manager, int rate)
{
	if ((rate >= FPS_LOWER_LIMIT) && (rate <= FPS_UPPER_LIMIT)) {
		manager->framecount = 0;
		manager->rate = rate;
		manager->rateticks = (1000.0f / (float)rate);
		return 0;
	} else {
		return -1;
	}
}

// Get current target framerate
int SDL_getFramerate(FPSmanager* manager)
{
	if (manager == NULL) {
		return -1;
	} else {
		return manager->rate;
	}
}

// Delay to maintain a constant framerate
void SDL_framerateDelay(FPSmanager* manager)
{
	Uint32 current_ticks = SDL_GetTicks();
	Uint32 target_ticks = manager->lastticks + (Uint32)((float)manager->framecount * manager->rateticks);

	manager->framecount++;

	if (current_ticks <= target_ticks) {
		SDL_Delay(target_ticks - current_ticks);
	} else {
		manager->framecount = 0;
		manager->lastticks = SDL_GetTicks();
	}
}
