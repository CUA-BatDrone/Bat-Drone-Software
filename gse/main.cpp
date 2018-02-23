#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <stdint.h>

#include "SDL.h"

using namespace std;

int main(int argc, char* argv[]) {

  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Texture *texture;
  // SDL_Event event;


  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
    return 3;
  }
  if (!(window = SDL_CreateWindow("Bat Drone Ground Station", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_RESIZABLE))) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window: %s", SDL_GetError());
    return 3;
  }
  SDL_DisableScreenSaver();
  if (!(renderer = SDL_CreateRenderer(window, -1, 0))) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create renderer: %s", SDL_GetError());
    return 3;
  }
  if (!(texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, 80, 60))) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create texture: %s", SDL_GetError());
    return 3;
  }

  while(!SDL_QuitRequested()) {
    unsigned char *pixels;
    int pitch;
    SDL_LockTexture(texture, NULL, (void **) &pixels, &pitch);
    for (int i = 0; i < pitch * 60; i += 3) {
      pixels[i] = pixels[i+1] = pixels[i+2] = rand();
    }
    SDL_UnlockTexture(texture);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
    SDL_Delay(100);
  }

  // SDL_Delay(3000);

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}


//
// void sendDir(float Pitch, float Yaw, float Roll, float Thrust)
// {
//   //toDo sendDir to FSW for CIARAN
// }
//
// int main(int argc, char* argv[]) {
//
//
//   //for now the user will have to input the values, once we get into the
//   //thick of things we will enhanced control
//   float Pitch, Yaw, Roll, Thrust;
//   cout << "Please enter the pitch: ";
//   cin >> Pitch;
//   cout << "Please enter the Yaw: ";
//   cin >> Yaw;
//   cout << "Please enter the Roll: ";
//   cin >> Roll;
//   cout << "Please enter the thrust: ";
//   cin >> Thrust;
//
//   sendDir(Pitch, Yaw, Roll, Thrust);
//
//   return 0;
// }
