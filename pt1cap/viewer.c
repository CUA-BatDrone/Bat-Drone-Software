#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <SDL2/SDL.h>

int main(int argc, char* argv[]) {
  if (argc > 4 || argc < 2) {
    printf(
      "Displays frames in file created by pt1cap\n"\
      "\n"\
      "Usage: %s <filename> [blue_level red_level]\n"\
      "\n"\
    , argv[0]);
    return -1;
  }
  int paused = 0;
  int frame = 0;
  int end = 0;
  int run = 1;
  long offset;
  float scale;
  printf("%i\n", argc);
  if(argc > 3) {
    offset = -atol(argv[2]);
    scale = 255.0 / (atof(argv[3]) + offset);
    printf("%li %f\n", offset, scale);
  } else {
    // 3400 to 3900 is human
    offset = -3400; // to 3800
    scale = 255.0/(3900-3400);
  }

  FILE *fd = fopen(argv[1], "r");
  printf("%s\n", argv[1]);

  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Texture *texture;


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

  const int w = 80;
  const int h = 60;
  const int r = 0;
  const int g = 1;
  const int b = 2;
  while(run) {
    unsigned long next_ticks = SDL_GetTicks() + 40;
    uint16_t y16[60][80];
    uint8_t (*rgb)[60][80][3];
    if(!paused && !end) {
      frame++;
      int pitch;
      if(!fread((void *) &y16, sizeof(y16), 1, fd)) {
        printf("End of file\n");
        end = 1;
        continue;
      }
      SDL_LockTexture(texture, NULL, (void **) &rgb, &pitch);
      for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
          float pixel = ((float) (y16[y][x] + offset)) * scale;
          if (pixel > 0xFF) {
            (*rgb)[y][x][r] = 0xFF;
            (*rgb)[y][x][g] = 0;
            (*rgb)[y][x][b] = 0;
          } else if (pixel < 0) {
            (*rgb)[y][x][r] = 0;
            (*rgb)[y][x][g] = 0;
            (*rgb)[y][x][b] = 0xFF;
          } else {
            (*rgb)[y][x][r] = (*rgb)[y][x][g] = (*rgb)[y][x][b] = pixel;
          }
        }
      }
      SDL_UnlockTexture(texture);
      SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
      SDL_RenderClear(renderer);
      SDL_RenderCopy(renderer, texture, NULL, NULL);
      SDL_RenderPresent(renderer);
      unsigned long ticks = SDL_GetTicks();
      if (ticks < next_ticks) {
        SDL_Delay(next_ticks - ticks);
      }
    }
    SDL_Event e;
    while(SDL_PollEvent(&e)) {
      int x, y;
      switch(e.type) {
      case SDL_QUIT:
        run = 0;
        break;
      case SDL_KEYDOWN:
        paused = !paused;
        break;
      case SDL_MOUSEBUTTONUP:
        x = e.button.x * 80 / 640;
        y = e.button.y * 60 / 480;
        printf("frame %i\ncoord %i,%i\nintensity %u\n", frame, x, y, y16[y][x]);
      }
    }
  }
  fclose(fd);

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
