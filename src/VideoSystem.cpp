//    Open Mugen is a redevelopment of Elecbyte's M.U.G.E.N wich will be 100% compatible to it
//    Copyright (C) 2004  Sahin Vardar
//
//    If you know bugs or have a wish on Open Muegn or (money/girls/a car) for me ;-)
//    Feel free and email me: sahin_v@hotmail.com  ICQ:317502935
//    Web: http://openmugen.sourceforge.net/
//    --------------------------------------------------------------------------
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.

//    You should have received a copy of the GNU General Public License
//    along with this program; if not, write to the Free Software
//    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#include "global.h"

void scale2x(SDL_Surface *src, SDL_Surface *dst);

//Konstructor
CVideoSystem::CVideoSystem() {
    work = NULL;
    window = NULL;
    renderer = NULL;
    texture = NULL;

    nowTime = lastTime = 0;
    nFpsCount = 0;
}

//Destructor
CVideoSystem::~CVideoSystem() {
    PrintMessage("CVideoSystem::~CVideoSystem() Cleaning Up");
    CleanUp();
}

void CVideoSystem::CleanUp() {
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(font);
    SDL_FreeSurface(work);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

bool CVideoSystem::InitSystem() {
    PrintMessage("CVideoSystem::InitSystem()");
    window = SDL_CreateWindow(OMTITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 320, 240,
                              SDL_SWSURFACE | SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    //Check the surface for validate
    if (window == NULL) {
        throw(CError("SDL_SetVideoMode Failed"));
        return false;
    }
    SDL_SetWindowTitle(window, OMTITLE);

    // Load the icon image as an SDL_Surface
    SDL_Surface *icon = SDL_LoadBMP("data/icon.bmp");
    if (icon == NULL) {
        PrintMessage("SDL_LoadBMP failed: %s", SDL_GetError());
        return false;
    }

    // Set the window icon
    SDL_SetWindowIcon(window, icon); // 'window' is your SDL_Window*

    // Free the icon surface after setting the icon
    SDL_FreeSurface(icon);

    //Create the work surface
    work = CreateSurface(320, 240);

    // Create a rectangle for the work surface (320x240)
    SDL_Rect workRect = {0, 0, 320, 240}; // Defining the area to fill

    // Set the draw color to magenta (RGB: 255, 0, 255, A: 255 for full opacity)
    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);

    // Fill the rectangle (the "work" area) with magenta
    SDL_RenderFillRect(renderer, &workRect);

    // Present the renderer (update the window to show the changes)
    SDL_RenderPresent(renderer);

    texture = SDL_CreateTexture(renderer,
                                SDL_PIXELFORMAT_RGB565,
                                SDL_TEXTUREACCESS_STREAMING,
                                work->w,
                                work->h);
    if (texture == NULL) {
        PrintMessage("SDL_CreateTextureFromSurface failed: %s", SDL_GetError());
    }

    //Set the frame manager to 60 Hz
    SDL_initFramerate(&m_FPSmanager);
    SDL_setFramerate(&m_FPSmanager, 60);

    //Loads the debug fonts
    LoadFont();

    return true;
}

void CVideoSystem::LoadFont() {
    FILE *pFile;
    char strTemp[255];
    int i = 0;

    //Loading debug fonts
    font = SDL_LoadBMP("data/DebugFonts.bmp");

    if (font == NULL)
        PrintMessage("CVideoSystem:: DebugFonts not found");

    pFile = fopen("data/font.txt", "r");

    if (pFile == NULL)
        PrintMessage("font.txt not found");

    while (!feof(pFile)) {
        fgets(strTemp, 255, pFile);
        sscanf(strTemp, "%c %i %i", &my_Fonts[i].c, &my_Fonts[i].x, &my_Fonts[i].nWidth);
        i++;
    }

    SDL_Texture *textureFont = SDL_CreateTextureFromSurface(renderer, font); // Convert surface to texture
    if (textureFont == NULL) {
        PrintMessage("SDL_CreateTextureFromSurface failed: %s", SDL_GetError());
        return;
    }

    // Set the color key (transparent color) for the texture using SDL_SetTextureColorMod
    // Here, we're making black (0,0,0) transparent in the texture
    SDL_SetTextureColorMod(textureFont, 0, 0, 0); // Set black to transparent

    // Now you can render the texture
    SDL_RenderCopy(renderer, textureFont, NULL, NULL);
    SDL_RenderPresent(renderer);

    // Clean up texture after use
    SDL_DestroyTexture(textureFont);
}

// To clear the screen
void CVideoSystem::Clear() {
    Uint32 color = SDL_MapRGB(work->format, 0, 0, 0);
    SDL_FillRect(work, NULL, color);
}


//Copy the work surface to screen backbuffer and then flip
void CVideoSystem::Draw() {
    SDL_UnlockSurface(work);

    nowTime = SDL_GetTicks();
    if (nowTime > lastTime + 500) {
        nFps = (float) nFpsCount * 1000 / (nowTime - lastTime);
        nFpsCount = 0;
        lastTime = nowTime;
    }

    DrawText(0, 0, "%2.2f FPS", nFps);

    FilterImage();

    //  scale2x(work,screen);

    if (texture == NULL) {
        PrintMessage("CVideoSystem::BlitSurface Create Texture Failed %s", SDL_GetError());
        return;
    }

    SDL_RenderClear(renderer);

    SDL_UpdateTexture(texture, NULL, work->pixels, work->pitch);

    SDL_RenderCopy(renderer, texture, NULL, NULL); // Render texture to the entire screen
    SDL_RenderPresent(renderer); // Present the renderer to the screen

    //Limit the frame rate to 60 Hz
#ifndef __EMSCRIPTEN__
    SDL_framerateDelay(&m_FPSmanager);
#endif

    nFpsCount++;
}

//this is the MMX version of Scale2x only uses this if we have a CPU with MMX support
void CVideoSystem::FilterImage() {
    /*#ifndef _XBOX
        if(SDL_LockSurface(work) < 0)
            PrintMessage("Was not able to lock work surface");

        if(SDL_LockSurface(screen) < 0)
            PrintMessage("Was not able to lock screen surface");


       scale2x_uint16 *dst0,*dst1;
       scale2x_uint16 *scr0,*scr1,*scr2;

       dst0=(scale2x_uint16*)screen->pixels;
       dst1=dst0+(screen->pitch/2);

       scr0=(scale2x_uint16*)work->pixels;
       scr1=scr0 + (work->pitch/2);
       scr2=scr1 + (work->pitch/2);

        scale2x_16_mmx(dst0,dst1,scr0,scr1,scr2,640);




      for(int i=0;i<240;i++)
      {
          scale2x_16_mmx(dst0,dst1,scr0,scr1,scr2,640);
          dst0+=(screen->pitch/2);
          dst1+=(screen->pitch);

     /*     scr0=scr1;
          scr1=scr2;
          scr2+=(work->pitch/2);


      }

       SDL_UnlockSurface(work);
       SDL_UnlockSurface(screen);

       //Clear the MMX
       scale2x_mmx_emms();
    #endif*/
}

void CVideoSystem::DrawRect(Sint16 x, Sint16 y, Sint16 x1, Sint16 y1, Uint8 R, Uint8 G, Uint8 B) {
}

//Draw a string to video
void CVideoSystem::DrawText(int x, int y, char *strText, ...) {
    char string[255]; // Temporary string

    int nRow = 0;
    int nCol = 0;

    SDL_Rect fontRect;
    SDL_Rect screenRect;

    va_list ap; // Pointer To List Of Arguments
    va_start(ap, strText); // Parses The String For Variables
    vsprintf(string, strText, ap); // Converts Symbols To Actual Numbers
    va_end(ap);

    memset(&fontRect, 0, sizeof(SDL_Rect));
    memset(&screenRect, 0, sizeof(SDL_Rect));

    screenRect.w = 5;
    screenRect.h = 8;


    fontRect = screenRect;

    screenRect.x = x;
    screenRect.y = y;


    for (int i = 0; i < strlen(string); i++) {
        for (int j = 0; j < 255; j++) {
            if (string[i] == my_Fonts[j].c) {
                fontRect.x = my_Fonts[j].x;
                fontRect.w = my_Fonts[j].nWidth;

                j = 255;
            }
        }

        if (string[i] != 32) {
            SDL_BlitSurface(font, &fontRect, work, &screenRect);
            screenRect.x += fontRect.w;
        } else
            screenRect.x += 5;
    }


    //  SDL_BlitSurface(font,NULL,work,NULL);
}

//Creates a SDL Surface
SDL_Surface *CVideoSystem::CreateSurface(int x, int y) {
    SDL_Surface *temp = NULL;
    temp = SDL_CreateRGBSurfaceWithFormat(0, x, y, 16, SDL_PIXELFORMAT_RGB565); // Using RGB565 as format

    if (temp == NULL) {
        PrintMessage("CVideoSystem::CreateSurface Create Surface Failed %s", SDL_GetError());
        return NULL;
    }
    return temp;
}


void CVideoSystem::NormalBlt(SFFSPRITE *lpSprite, s16 x, s16 y, bool bMask) {
    u16 *lpWorkData;
    u16 pitch;

    s16 width = lpSprite->PcxHeader.widht;
    s16 height = lpSprite->PcxHeader.height;
    u8 *byData = lpSprite->byPcxFile;
    u16 *ColorTable = lpSprite->ColorPallet;

    //calculate x and y value
    y -= height - (height - lpSprite->y);
    x -= width - (width - lpSprite->x);

    lpWorkData = (u16 *) work->pixels;
    pitch = work->pitch / 2;

    u16 yClip = 0;
    u16 yClip2 = 0;
    u16 xClip = 0;
    u16 xClip2 = 0;


    if (x + width > XMAX) {
        width -= x + width - XMAX;
    }

    if (x < 0) {
        xClip = -x;
        x = 0;
    }


    if (y + height > YMAX)
        height -= y + height - YMAX;

    if (y < 0) {
        yClip = -y;
        y = 0;
    }

    lpWorkData += y * pitch;
    lpWorkData += x;

    if (!bMask) {
        for (int i = yClip; i < height; i++) {
            for (int j = xClip; j < width; j++) {
                *lpWorkData = ColorTable[byData[j + i * lpSprite->PcxHeader.widht]];
                lpWorkData++;
            }
            lpWorkData -= width - xClip;
            lpWorkData += pitch;
        }
    } else {
        for (int i = yClip; i < height; i++) {
            for (int j = xClip; j < width; j++) {
                if (byData[j + i * lpSprite->PcxHeader.widht] != byData[0])
                    *lpWorkData = ColorTable[byData[j + i * lpSprite->PcxHeader.widht]];
                lpWorkData++;
            }
            lpWorkData -= width - xClip;
            lpWorkData += pitch;
        }
    }
}

void CVideoSystem::NormalFlipH(SFFSPRITE *lpSprite, s16 x, s16 y, bool bMask) {
    u16 *lpWorkData;
    u16 pitch;

    s16 width = lpSprite->PcxHeader.widht;
    s16 height = lpSprite->PcxHeader.height;
    u8 *byData = lpSprite->byPcxFile;
    u16 *ColorTable = lpSprite->ColorPallet;

    //calculate x and y value
    y -= height - (height - lpSprite->y);
    x -= width - lpSprite->x;

    lpWorkData = (u16 *) work->pixels;
    pitch = work->pitch / 2;

    u16 yClip = 0;
    u16 yClip2 = 0;
    u16 xClip = 0;
    u16 xClip2 = 0;


    if (x + width > XMAX) {
        xClip2 = width;
        width -= x + width - XMAX;
        //need for h flip
        xClip2 = xClip2 - width;
    }

    if (x < 0) {
        xClip = -x;
        x = 0;
    }


    if (y + height > YMAX)
        height -= y + height - YMAX;

    if (y < 0) {
        yClip = -y;
        y = 0;
    }

    lpWorkData += y * pitch;
    lpWorkData += x;

    if (!bMask) {
        for (int i = yClip; i < height; i++) {
            for (int j = xClip; j < width; j++) {
                *lpWorkData = ColorTable[byData[width - j + xClip2 - 1 + i * lpSprite->PcxHeader.widht]];
                lpWorkData++;
            }
            lpWorkData -= width - xClip;
            lpWorkData += pitch;
        }
    } else {
        for (int i = yClip; i < height; i++) {
            for (int j = xClip; j < width; j++) {
                if (byData[width - j + xClip2 - 1 + i * lpSprite->PcxHeader.widht] != byData[0])
                    *lpWorkData = ColorTable[byData[width - j + xClip2 - 1 + i * lpSprite->PcxHeader.widht]];
                lpWorkData++;
            }
            lpWorkData -= width - xClip;
            lpWorkData += pitch;
        }
    }
}

u16 CVideoSystem::MapRGB(Uint8 red, Uint8 green, Uint8 blue) {
    // Map RGB to a 16-bit RGB565 format.
    Uint8 r = red >> 3; // Red: 5 bits
    Uint8 g = green >> 2; // Green: 6 bits
    Uint8 b = blue >> 3; // Blue: 5 bits

    // Combine the components into a single 16-bit value
    u16 color = (r << 11) | (g << 5) | b;

    return color; // Return the 16-bit mapped color.
}


/*
void CVideoSystem::NormalBlt(SFFSPRITE *lpSprite,s16 x,s16 y,bool bMask,float xScale,float yScale,PalFX *effect)
{
     u16 *lpWorkData;
     u16 pitch;

     s16 width=(u16)(lpSprite->PcxHeader.widht*xScale);
     s16 height=(u16)(lpSprite->PcxHeader.height*yScale);
     u8* byData=lpSprite->byPcxFile;
     RGBColor *ColorTable=lpSprite->ColorPallet;

     //calculate x and y value
     y-=height-(height-lpSprite->y*yScale);
     x-=width-(width-lpSprite->x*xScale);



     if(SDL_LockSurface(work) < 0)
        PrintMessage("Was not able to lock work surface");

     lpWorkData=(u16*)work->pixels;
     pitch=work->pitch/2;

//clipping

     u16 yClip=0;
     u16 yClip2=0;
     u16 xClip=0;
     u16 xClip2=0;

     Clip(x,y,width,height,xClip,xClip2,yClip,yClip2);


     lpWorkData+=y*pitch;
     lpWorkData+=x;


        //only use the the scaled verion if we need to scale
       if( (xScale != 1.0) || (yScale !=1.0) )
       {

         for(int i=yClip;i<height;i++)
         {

            for(int j=xClip;j<width;j++)
            {
              *lpWorkData=(u16)SDL_MapRGB(screen->format,
                                       ColorTable[byData[(u16)(j/xScale) + (u16)(i/yScale)*lpSprite->PcxHeader.widht]].R,
                                       ColorTable[byData[(u16)(j/xScale) + (u16)(i/yScale)*lpSprite->PcxHeader.widht]].G,
                                       ColorTable[byData[(u16)(j/xScale) + (u16)(i/yScale)*lpSprite->PcxHeader.widht]].B);
               lpWorkData++;

             }
             lpWorkData-=width-xClip;
             lpWorkData+=pitch;

        }

     }


     if( (xScale == 1.0) && (yScale == 1.0) )
     {
          for(int i=yClip;i<height;i++)
     {

      if(!bMask)
      {

        for(int j=xClip;j<width;j++)
        {
           *lpWorkData=(u16)SDL_MapRGB(screen->format,
                                ColorTable[byData[j + i*lpSprite->PcxHeader.widht]].R,
                                ColorTable[byData[j + i*lpSprite->PcxHeader.widht]].G,
                                ColorTable[byData[j + i*lpSprite->PcxHeader.widht]].B);
           lpWorkData++;

        }
        lpWorkData-=width-xClip;
        lpWorkData+=pitch;

       }
       else
       {
        for(int j=xClip;j<width;j++)
        {
           if((u16)SDL_MapRGB(screen->format,
                                ColorTable[byData[j + i*lpSprite->PcxHeader.widht]].R,
                                ColorTable[byData[j + i*lpSprite->PcxHeader.widht]].G,
                                ColorTable[byData[j + i*lpSprite->PcxHeader.widht]].B)
           != (u16)SDL_MapRGB(screen->format,
                                ColorTable[0].R,
                                ColorTable[0].G,
                                ColorTable[0].B) )
           {


           *lpWorkData=(u16)SDL_MapRGB(screen->format,
                                ColorTable[byData[j + i*lpSprite->PcxHeader.widht]].R,
                                ColorTable[byData[j + i*lpSprite->PcxHeader.widht]].G,
                                ColorTable[byData[j + i*lpSprite->PcxHeader.widht]].B);
           }
           lpWorkData++;

        }
        lpWorkData-=width-xClip;
        lpWorkData+=pitch;

       }

     }

     }


     SDL_UnlockSurface(work);
}
*/
