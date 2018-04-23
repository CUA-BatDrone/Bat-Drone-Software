#include "cmd_tlm.hpp"
#include "ui.hpp"
#include "packet_elements.hpp"
#include <thread>
#include <SDL.h>
#include <iostream>

using namespace std;

UI::UI(CmdTlm &cmdtlm) : cmdtlm(cmdtlm) {
  run = true;
  texture = NULL;
}

ControlPacketElement UI::handleKeyboard(float &last_thrust) {
  ControlPacketElement c;
  const float rate = 0.25;
  const Uint8 *keyboard = SDL_GetKeyboardState(NULL);
  if (keyboard[SDL_SCANCODE_RETURN]) {
    c.pitch = -1;
    c.yaw = 1;
    c.roll = -1;
    c.thrust = -1;
    last_thrust = -1;
  }
  else {
    // Forward
    if (keyboard[SDL_SCANCODE_W] || keyboard[SDL_SCANCODE_UP]) {
      // And back
      if (keyboard[SDL_SCANCODE_S] || keyboard[SDL_SCANCODE_DOWN]) {
        c.pitch = 0;
        // Just forward
      }
      else {
        c.pitch = rate;
      }
      // Backward
    }
    else if (keyboard[SDL_SCANCODE_S] || keyboard[SDL_SCANCODE_DOWN]) {
      c.pitch = -rate;
      // None
    }
    else {
      c.pitch = 0;
    }
    // Left
    if (keyboard[SDL_SCANCODE_A] || keyboard[SDL_SCANCODE_LEFT]) {
      // And Right
      if (keyboard[SDL_SCANCODE_D] || keyboard[SDL_SCANCODE_RIGHT]) {
        c.roll = 0;
        // Just left
      }
      else {
        c.roll = -rate;
      }
      // Right
    }
    else if (keyboard[SDL_SCANCODE_D] || keyboard[SDL_SCANCODE_RIGHT]) {
      c.roll = rate;
      // None
    }
    else {
      c.roll = 0;
    }
    // Rotate Left
    if (keyboard[SDL_SCANCODE_Q] || keyboard[SDL_SCANCODE_PAGEUP]) {
      // And right
      if (keyboard[SDL_SCANCODE_E] || keyboard[SDL_SCANCODE_PAGEDOWN]) {
        c.yaw = 0;
        // Just left
      }
      else {
        c.yaw = -rate;
      }
      // Rotate Right
    }
    else if (keyboard[SDL_SCANCODE_E] || keyboard[SDL_SCANCODE_PAGEDOWN]) {
      c.yaw = rate;
    }
    else {
      c.yaw = 0;
    }

    // Thrust inputted using keys '`' through '0' are -1 through 1.
    // ### Key 0 IS MAX THRUST!!! ###
    if (keyboard[SDL_SCANCODE_ESCAPE] || keyboard[SDL_SCANCODE_GRAVE]) {
      last_thrust = -1.0;
    }
    else if (keyboard[SDL_SCANCODE_1]) {
      last_thrust = -0.8;
    }
    else if (keyboard[SDL_SCANCODE_2]) {
      last_thrust = -0.6;
    }
    else if (keyboard[SDL_SCANCODE_3]) {
      last_thrust = -0.4;
    }
    else if (keyboard[SDL_SCANCODE_4]) {
      last_thrust = -0.2;
    }
    else if (keyboard[SDL_SCANCODE_5]) {
      last_thrust = 0.0;
    }
    else if (keyboard[SDL_SCANCODE_6]) {
      last_thrust = 0.2;
    }
    else if (keyboard[SDL_SCANCODE_7]) {
      last_thrust = 0.4;
    }
    else if (keyboard[SDL_SCANCODE_8]) {
      last_thrust = 0.6;
    }
    else if (keyboard[SDL_SCANCODE_9]) {
      last_thrust = 0.8;
    }
    else if (keyboard[SDL_SCANCODE_0]) {
      last_thrust = 1.0;
    }
    if (keyboard[SDL_SCANCODE_SPACE]) {
      if (last_thrust <= 0.9 && !keyboard[SDL_SCANCODE_LCTRL]) {
        c.thrust = last_thrust + 0.1;
      }
      else {
        c.thrust = last_thrust;
      }
    }
    else if (keyboard[SDL_SCANCODE_LCTRL]) {
      if (last_thrust >= -0.9) {
        c.thrust = last_thrust - 0.1;
      }
      else {
        c.thrust = last_thrust;
      }
    }
    else {
      c.thrust = last_thrust;
    }
  }
  return c;
}

ControlPacketElement UI::handleJoystick(SDL_Joystick *joystick) {
  ControlPacketElement c;
  c.thrust = SDL_JoystickGetAxis(joystick, 3) / (float)-0x7FFF;
  c.pitch = SDL_JoystickGetAxis(joystick, 1) / (float)-0x7FFF;
  c.roll = SDL_JoystickGetAxis(joystick, 0) / (float)0x7FFF;
  c.yaw = SDL_JoystickGetAxis(joystick, 2) / (float)0x7FFF;
  return c;
}

SDL_Joystick *UI::getJoystick() {
  for (int i = 0; i < SDL_NumJoysticks(); ++i) {
    SDL_Joystick *joystick = SDL_JoystickOpen(i);
    if (joystick) {
      cout << "Using " << SDL_JoystickName(joystick) << endl;
      return joystick;
    }
  }
  cout << "Using keyboard" << endl;
  return NULL;
}

void UI::lwirFrame(const uint16_t lwirFrame[60][80]) {
  unique_lock<mutex> ul(texture_mutex);
  if (texture && run) {
    // 2800 to 3200 is human
    const uint16_t low = 2800, high = 3200;
    uint16_t offset = -low;
    const float scale = 255.0f / (high - low);
    unsigned char (*rgbFrame)[80][3];
    int pixel_pitch;
    SDL_LockTexture(texture, NULL, (void **)&rgbFrame, &pixel_pitch);
    for (int y = 0; y < 60; y++) {
      for (int x = 0; x < 80; x++) {
        if (lwirFrame[y][x] > high) {
          rgbFrame[y][x][0] = 255;
          rgbFrame[y][x][1] = 192;
          rgbFrame[y][x][2] = 192;
        } else if (lwirFrame[y][x] < low) {
          rgbFrame[y][x][0] = 0;
          rgbFrame[y][x][1] = 64;
          rgbFrame[y][x][2] = 64;
        } else {
          uint16_t value = lwirFrame[y][x] * scale;
          rgbFrame[y][x][0] = rgbFrame[y][x][1] = rgbFrame[y][x][2] = value;
        }
      }
      /*uint16_t value = (((uint16_t *)frame)[i] + offset) * scale;
      pixels[i * 3] = pixels[i * 3 + 1] = pixels[i * 3 + 2] = value;*/
    }
    SDL_UnlockTexture(texture);
  }
}

void UI::blob(uint16_t x, uint16_t y) {
  cout << "Blob at (" << x << "," << y << ")" << endl;
  blobx = x;
  bloby = y;
}

void UI::mainLoop() {
  unique_lock<mutex> ul(texture_mutex);
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Joystick *joystick = NULL;
  int last_num_joysticks = 0;


  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
    return;
  }
  if (!(window = SDL_CreateWindow("Bat Drone Ground Station", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_RESIZABLE))) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window: %s", SDL_GetError());
    return;
  }
  SDL_DisableScreenSaver();
  if (SDL_GL_SetSwapInterval(-1)) {
    SDL_GL_SetSwapInterval(1);
    cout << "VSYNC ON" << endl;
  }
  if (!(renderer = SDL_CreateRenderer(window, -1, 0))) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create renderer: %s", SDL_GetError());
    return;
  }
  if (!(texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, 80, 60))) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create texture: %s", SDL_GetError());
    return;
  }
  float last_thrust = -1;
  while (run) {
    this_thread::sleep_for(chrono::milliseconds(1000/60));
  ul.unlock();

    // Setup game controllers
    int num_joysticks = SDL_NumJoysticks();
    if (num_joysticks != last_num_joysticks) {
      last_num_joysticks = num_joysticks;
      joystick = getJoystick();
    }

    // check for events
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        run = false;
        continue;
      }
    }
    ControlPacketElement c;
    // Direction inputted with WASDQE or arrowkeys and page up/down.
    if (joystick) {
      cmdtlm.control(handleJoystick(joystick));
      last_thrust = -1.0;
    }
    else {
      try {
        cmdtlm.control(handleKeyboard(last_thrust));
      } catch (string e) {

      }
    }

    //unsigned char *pixels;
    //int pixel_pitch;
    //SDL_LockTexture(texture, NULL, (void **)&pixels, &pixel_pitch);
    //for (int i = 0; i < pixel_pitch * 60; i += 3) {
    //  pixels[i] = pixels[i + 1] = pixels[i + 2] = rand();
    //}
    //SDL_UnlockTexture(texture);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    ul.lock();
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_Rect rect;
    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    rect.x = blobx - 16;
    rect.y = bloby - 16;
    rect.w = 32;
    rect.h = 32;
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderDrawRect(renderer, &rect);
    SDL_RenderPresent(renderer);
    //SDL_Delay(400);
  }

  // SDL_Delay(3000);

  if (joystick) {
    SDL_JoystickClose(joystick);
  }
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
