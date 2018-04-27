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

float UI::handleKeyboard(CmdTlm &cmdtlm, float &last_thrust) {
  float pitch, yaw, roll, thrust;
  const float rate = 0.25;
  const Uint8 *keyboard = SDL_GetKeyboardState(NULL);
  if (keyboard[SDL_SCANCODE_RCTRL]) {
    cmdtlm.manual();
  } else if(keyboard[SDL_SCANCODE_RALT]) {
    cmdtlm.autonomous();
  }
  if (keyboard[SDL_SCANCODE_RETURN]) {
    pitch = -1;
    yaw = 1;
    roll = -1;
    thrust = -1;
    last_thrust = -1;
  } else {
    // Forward
    if (keyboard[SDL_SCANCODE_W] || keyboard[SDL_SCANCODE_UP]) {
      // And back
      if (keyboard[SDL_SCANCODE_S] || keyboard[SDL_SCANCODE_DOWN]) {
        pitch = 0;
        // Just forward
      } else {
        pitch = rate;
      }
      // Backward
    } else if (keyboard[SDL_SCANCODE_S] || keyboard[SDL_SCANCODE_DOWN]) {
      pitch = -rate;
      // None
    } else {
      pitch = 0;
    }
    // Left
    if (keyboard[SDL_SCANCODE_A] || keyboard[SDL_SCANCODE_LEFT]) {
      // And Right
      if (keyboard[SDL_SCANCODE_D] || keyboard[SDL_SCANCODE_RIGHT]) {
        roll = 0;
        // Just left
      } else {
        roll = -rate;
      }
      // Right
    } else if (keyboard[SDL_SCANCODE_D] || keyboard[SDL_SCANCODE_RIGHT]) {
      roll = rate;
      // None
    } else {
      roll = 0;
    }
    // Rotate Left
    if (keyboard[SDL_SCANCODE_Q] || keyboard[SDL_SCANCODE_PAGEUP]) {
      // And right
      if (keyboard[SDL_SCANCODE_E] || keyboard[SDL_SCANCODE_PAGEDOWN]) {
        yaw = 0;
        // Just left
      } else {
        yaw = -rate;
      }
      // Rotate Right
    } else if (keyboard[SDL_SCANCODE_E] || keyboard[SDL_SCANCODE_PAGEDOWN]) {
      yaw = rate;
    } else {
      yaw = 0;
    }

    // Thrust inputted using keys '`' through '0' are -1 through 1.
    // ### Key 0 IS MAX THRUST!!! ###
    if (keyboard[SDL_SCANCODE_ESCAPE] || keyboard[SDL_SCANCODE_GRAVE]) {
      last_thrust = -1.0;
    } else if (keyboard[SDL_SCANCODE_1]) {
      last_thrust = -0.8;
    } else if (keyboard[SDL_SCANCODE_2]) {
      last_thrust = -0.6;
    } else if (keyboard[SDL_SCANCODE_3]) {
      last_thrust = -0.4;
    } else if (keyboard[SDL_SCANCODE_4]) {
      last_thrust = -0.2;
    } else if (keyboard[SDL_SCANCODE_5]) {
      last_thrust = 0.0;
    } else if (keyboard[SDL_SCANCODE_6]) {
      last_thrust = 0.2;
    } else if (keyboard[SDL_SCANCODE_7]) {
      last_thrust = 0.4;
    } else if (keyboard[SDL_SCANCODE_8]) {
      last_thrust = 0.6;
    } else if (keyboard[SDL_SCANCODE_9]) {
      last_thrust = 0.8;
    } else if (keyboard[SDL_SCANCODE_0]) {
      last_thrust = 1.0;
    }
    if (keyboard[SDL_SCANCODE_SPACE]) {
      if (last_thrust <= 0.9 && !keyboard[SDL_SCANCODE_LCTRL]) {
        thrust = last_thrust + 0.1;
      } else {
        thrust = last_thrust;
      }
    } else if (keyboard[SDL_SCANCODE_LCTRL]) {
      if (last_thrust >= -0.9) {
        thrust = last_thrust - 0.1;
      } else {
        thrust = last_thrust;
      }
    } else {
      thrust = last_thrust;
    }
  }
  return thrust;
}

float UI::handleJoystick(SDL_Joystick *joystick, CmdTlm &cmdtlm) {
  if (SDL_JoystickGetButton(joystick, 0)){
    cmdtlm.manual();
  } else if (SDL_JoystickGetButton(joystick, 1)) {
    cmdtlm.autonomous();
  }
  float thrust;
  float pitch, roll, yaw;
  thrust = SDL_JoystickGetAxis(joystick, 3) / (float)-0x7FFF;
  pitch = SDL_JoystickGetAxis(joystick, 1) / (float)-0x7FFF;
  roll = SDL_JoystickGetAxis(joystick, 0) / (float)0x7FFF;
  yaw = SDL_JoystickGetAxis(joystick, 2) / (float)0x7FFF;
  cmdtlm.control(roll, pitch, thrust, yaw);
  return thrust;
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
    const uint16_t low = 2000, high = 4000;
    const uint16_t tlow = 2800, thigh = 3600;
    uint16_t offset = -low;
    const float scale = 255.0f / (high - low);
    unsigned char (*rgbFrame)[80][3];
    int pixel_pitch;
    SDL_LockTexture(texture, NULL, (void **)&rgbFrame, &pixel_pitch);
    for (int y = 0; y < 60; y++) {
      for (int x = 0; x < 80; x++) {
        if (lwirFrame[y][x] > high) {
          rgbFrame[y][x][0] = 255;
          rgbFrame[y][x][1] = 255;
          rgbFrame[y][x][2] = 0;
        } else if (lwirFrame[y][x] < low) {
          rgbFrame[y][x][0] = 0;
          rgbFrame[y][x][1] = 255;
          rgbFrame[y][x][2] = 255;
        } else {
          uint16_t value = lwirFrame[y][x] * scale;
          if (lwirFrame[y][x] > thigh) {
            rgbFrame[y][x][0] = rgbFrame[y][x][1] = rgbFrame[y][x][2] = value;
          } else if (lwirFrame[y][x] < tlow) {
            rgbFrame[y][x][0] = rgbFrame[y][x][1] = rgbFrame[y][x][2] = value;
          } else {
            rgbFrame[y][x][0] = 0;
            rgbFrame[y][x][1] = value;
            rgbFrame[y][x][2] = 0;
          }
        }
      }
      /*uint16_t value = (((uint16_t *)frame)[i] + offset) * scale;
      pixels[i * 3] = pixels[i * 3 + 1] = pixels[i * 3 + 2] = value;*/
    }
    SDL_UnlockTexture(texture);
  }
}

void UI::blob(uint16_t x, uint16_t y) {
  blobx = x;
  bloby = y;
}

void UI::control(float aileron, float elevator, float thrust, float rudder) {
  fsw_aileron = aileron;
  fsw_elevator = elevator;
  fsw_thrust = thrust;
  fsw_rudder = rudder;
}

void UI::blobs(vector<Commands::Blob> & blobs) {
  unique_lock<mutex> ul(blob_array_mutex);
  blob_array = blobs;
}

void UI::mainLoop() {
  unique_lock<mutex> ul(texture_mutex);
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Joystick *joystick = NULL;
  int last_num_joysticks = 0;
  enum { P, I, D } active_pid_field = P;
  class PIDValues {
  public:
    float p, i, d;
    PIDValues() : p(0), i(0), d(0) {}
  } pid_values;
  

  // Initialize
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

  // Main Loop
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
      switch (e.type) {
        case SDL_QUIT: {
          run = false;
          continue;
        }
        case SDL_KEYDOWN: {
          switch (e.key.keysym.scancode) {
            case SDL_SCANCODE_B: {
              active_pid_field = P;
              break;
            }
            case SDL_SCANCODE_N: {
              active_pid_field = I;
              break;
            }
            case SDL_SCANCODE_M: {
              active_pid_field = D;
              break;
            }
            case SDL_SCANCODE_PERIOD: {
              // Increment PID
              switch (active_pid_field) {
                case P: {
                  pid_values.p++;
                  break;
                }
                case I: {
                  pid_values.i++;
                  break;
                }
                case D: {
                  pid_values.d++;
                  break;
                }
              }
              cmdtlm.pid(pid_values.p, pid_values.i, pid_values.d);
              break;
            }
            case SDL_SCANCODE_COMMA: {
              // Decrement PID
              switch (active_pid_field) {
                case P: {
                  pid_values.p--;
                  break;
                }
                case I: {
                  pid_values.i--;
                  break;
                }
                case D: {
                  pid_values.d--;
                  break;
                }
              }
              cmdtlm.pid(pid_values.p, pid_values.i, pid_values.d);
              break;
            }
            case SDL_SCANCODE_SLASH: {
              // Reset PID
              switch (active_pid_field) {
                case P: {
                  pid_values.p = 0;
                  break;
                }
                case I: {
                  pid_values.i = 0;
                  break;
                }
                case D: {
                  pid_values.d = 0;
                  break;
                }
              }
              cmdtlm.pid(pid_values.p, pid_values.i, pid_values.d);
              break;
            }
          }
        }
      }
    }
    ControlPacketElement c;
    // Direction inputted with WASDQE or arrowkeys and page up/down.
    if (joystick) {
      last_thrust = handleJoystick(joystick, cmdtlm);
    } else {
      last_thrust = handleKeyboard(cmdtlm, last_thrust);
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

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_Rect rect;
      rect.w = 32;
      rect.h = 32;
    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    blob_array_mutex.lock();
    for (Commands::Blob &blob : blob_array) {
      rect.x = blob.x * w / 80 - 16;
      rect.y = blob.y * h / 60 - 16;
      SDL_RenderDrawRect(renderer, &rect);
    }
    blob_array_mutex.unlock();
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    rect.x = blobx * w / 80 - 16;
    rect.y = bloby * h / 60 - 16;
    SDL_RenderDrawRect(renderer, &rect);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 64);
    SDL_RenderDrawLine(renderer, fsw_aileron * w, 0, fsw_aileron * w, h);
    SDL_RenderDrawLine(renderer, 0, fsw_elevator, w, fsw_elevator * h);

    ul.lock();
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
  }

  if (joystick) {
    SDL_JoystickClose(joystick);
  }
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}