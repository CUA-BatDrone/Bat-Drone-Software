#include "cmd_tlm.hpp"
#include "ui.hpp"
#include "packet_elements.hpp"
#include <thread>
#include <SDL.h>
#include <iostream>

using namespace std;

UI::UI(CmdTlm &cmdtlm) : cmdtlm(cmdtlm), threshold_offset(0) {
  run = true;
  texture = NULL;
}

void UI::handleKeyboard(CmdTlm &cmdtlm, float &thrust) {
  float pitch, yaw, roll;
  float rate;
  if (SDL_GetModState() & KMOD_CAPS) {
    rate = 0.50f;
  } else {
    rate = 0.25f;
  }
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
      thrust = -1.0;
    } else if (keyboard[SDL_SCANCODE_1]) {
      thrust = -0.8;
    } else if (keyboard[SDL_SCANCODE_2]) {
      thrust = -0.6;
    } else if (keyboard[SDL_SCANCODE_3]) {
      thrust = -0.4;
    } else if (keyboard[SDL_SCANCODE_4]) {
      thrust = -0.2;
    } else if (keyboard[SDL_SCANCODE_5]) {
      thrust = 0.0;
    } else if (keyboard[SDL_SCANCODE_6]) {
      thrust = 0.2;
    } else if (keyboard[SDL_SCANCODE_7]) {
      thrust = 0.4;
    } else if (keyboard[SDL_SCANCODE_8]) {
      thrust = 0.6;
    } else if (keyboard[SDL_SCANCODE_9]) {
      thrust = 0.8;
    } else if (keyboard[SDL_SCANCODE_0]) {
      thrust = 1.0;
    }
    //float current_thrust
    //if (keyboard[SDL_SCANCODE_SPACE]) {
    //  if (thrust <= 0.9 && !keyboard[SDL_SCANCODE_LCTRL]) {
    //    current_thrust = thrust + 0.1;
    //  } else {
    //    thrust = last_thrust;
    //  }
    //} else if (keyboard[SDL_SCANCODE_LCTRL]) {
    //  if (last_thrust >= -0.9) {
    //    current_thrust = thrust - 0.1;
    //  } else {
    //    current_thrust = thrust;
    //  }
    //} else {
    //  current_thrust = thrust;
    //}
  }
  cmdtlm.control(roll, pitch, thrust, yaw);
}

void UI::handleJoystick(SDL_Joystick *joystick, CmdTlm &cmdtlm, float &thrust) {
  if (SDL_JoystickGetButton(joystick, 0)){
    cmdtlm.manual();
  } else if (SDL_JoystickGetButton(joystick, 1)) {
    cmdtlm.autonomous();
  }
  float pitch, roll, yaw;
  thrust = SDL_JoystickGetAxis(joystick, 3) / (float)-0x7FFF;
  pitch = SDL_JoystickGetAxis(joystick, 1) / (float)-0x7FFF;
  roll = SDL_JoystickGetAxis(joystick, 0) / (float)0x7FFF;
  yaw = SDL_JoystickGetAxis(joystick, 2) / (float)0x7FFF;
  cmdtlm.control(roll, pitch, thrust, yaw);
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
    const uint16_t low = 2000 + threshold_offset, high = 4000 + threshold_offset;
    const uint16_t tlow = 2800 + threshold_offset, thigh = 3600 + threshold_offset;
    uint16_t offset = -low;
    const float scale = 255.0f / (high - low);
    unsigned char (*rgbFrame)[80][3];
    int pixel_pitch;
    SDL_LockTexture(texture, NULL, (void **)&rgbFrame, &pixel_pitch);
    for (int y = 0; y < 60; y++) {
      for (int x = 0; x < 80; x++) {
        if (lwirFrame[y][x] > high) {
          rgbFrame[y][x][0] = 255;
          rgbFrame[y][x][1] = 128;
          rgbFrame[y][x][2] = 128;
        } else if (lwirFrame[y][x] < low) {
          rgbFrame[y][x][0] = 0;
          rgbFrame[y][x][1] = 0;
          rgbFrame[y][x][2] = 128;
        } else {
          uint16_t value = lwirFrame[y][x] * scale;
          if (lwirFrame[y][x] > thigh || lwirFrame[y][x] < tlow) {
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
  } pid_values[3] = { PIDValues(), PIDValues(), PIDValues() };
  int pid_index = 0;
  

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
  ul.unlock();

  // Main Loop
  float last_thrust = -1;
  while (run) {
    // Get window size
    int w, h;
    SDL_GetWindowSize(window, &w, &h);

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
          float pid_step;
          if (e.key.keysym.mod & KMOD_CAPS) {
            pid_step = 0.010f;
          } else {
            pid_step = 0.001f;
          }
          switch (e.key.keysym.scancode) {
            case SDL_SCANCODE_LEFTBRACKET: {
              threshold_offset -= 10;
              cout << "Threshold: " << threshold_offset << endl;
              cmdtlm.threshold(threshold_offset);
              break;
            }
            case SDL_SCANCODE_RIGHTBRACKET: {
              threshold_offset += 10;
              cout << "Threshold: " << threshold_offset << endl;
              cmdtlm.threshold(threshold_offset);
              break;
            }
            case SDL_SCANCODE_F11: {
              SDL_SetWindowFullscreen(window, SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN_DESKTOP ? SDL_WINDOW_SHOWN : SDL_WINDOW_FULLSCREEN_DESKTOP);
            }
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
            case SDL_SCANCODE_Z: {
              pid_index = 0;
              break;
            }
            case SDL_SCANCODE_X: {
              pid_index = 1;
              break;
            }
            case SDL_SCANCODE_C: {
              pid_index = 2;
              break;
            }
            case SDL_SCANCODE_PERIOD: {
              // Increment PID
              switch (active_pid_field) {
                case P: {
                  pid_values[pid_index].p += pid_step;
                  break;
                }
                case I: {
                  pid_values[pid_index].i += pid_step;
                  break;
                }
                case D: {
                  pid_values[pid_index].d += pid_step;
                  break;
                }
              }
              cmdtlm.pid(pid_values[0].p, pid_values[0].i, pid_values[0].d, pid_values[1].p, pid_values[1].i, pid_values[1].d, pid_values[2].p, pid_values[2].i, pid_values[2].d);
              cout << "PID1: " << pid_values[0].p << " " << pid_values[0].i << " " << pid_values[0].d << endl;
              cout << "PID2: " << pid_values[1].p << " " << pid_values[1].i << " " << pid_values[1].d << endl;
              cout << "PID3: " << pid_values[2].p << " " << pid_values[2].i << " " << pid_values[2].d << endl;
              break;
            }
            case SDL_SCANCODE_COMMA: {
              // Decrement PID
              switch (active_pid_field) {
                case P: {
                  pid_values[pid_index].p -= pid_step;
                  break;
                }
                case I: {
                  pid_values[pid_index].i -= pid_step;
                  break;
                }
                case D: {
                  pid_values[pid_index].d -= pid_step;
                  break;
                }
              }
              cmdtlm.pid(pid_values[0].p, pid_values[0].i, pid_values[0].d, pid_values[1].p, pid_values[1].i, pid_values[1].d, pid_values[2].p, pid_values[2].i, pid_values[2].d);
              cout << "PID1: " << pid_values[0].p << " " << pid_values[0].i << " " << pid_values[0].d << endl;
              cout << "PID2: " << pid_values[1].p << " " << pid_values[1].i << " " << pid_values[1].d << endl;
              cout << "PID3: " << pid_values[2].p << " " << pid_values[2].i << " " << pid_values[2].d << endl;
              break;
            }
            case SDL_SCANCODE_SLASH: {
              // Reset PID
              switch (active_pid_field) {
                case P: {
                  pid_values[pid_index].p = 0;
                  break;
                }
                case I: {
                  pid_values[pid_index].i = 0;
                  break;
                }
                case D: {
                  pid_values[pid_index].d = 0;
                  break;
                }
              }
              cmdtlm.pid(pid_values[0].p, pid_values[0].i, pid_values[0].d, pid_values[1].p, pid_values[1].i, pid_values[1].d, pid_values[2].p, pid_values[2].i, pid_values[2].d);
              cout << "PID1: " << pid_values[0].p << " " << pid_values[0].i << " " << pid_values[0].d << endl;
              cout << "PID2: " << pid_values[1].p << " " << pid_values[1].i << " " << pid_values[1].d << endl;
              cout << "PID3: " << pid_values[2].p << " " << pid_values[2].i << " " << pid_values[2].d << endl;
              break;
            }
          }
        }
      }
    }
    // Direction inputted with WASDQE or arrowkeys and page up/down.
    if (joystick) {
      handleJoystick(joystick, cmdtlm, last_thrust);
    } else {
      handleKeyboard(cmdtlm, last_thrust);
    }
    int mousex, mousey;
    if (SDL_BUTTON_LEFT == SDL_BUTTON(SDL_GetMouseState(&mousex, &mousey))) {
      cmdtlm.track(mousex / w, mousey / h);
    }


    // Clear screen
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    
    // Draw frame
    ul.lock();
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    ul.unlock();

    // Draw blobs
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_Rect rect;
    rect.w = 32;
    rect.h = 32;
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

    // Draw controls
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    rect.x = w * 3 / 4;
    rect.y = h * 3 / 4;
    rect.w = w / 4;
    rect.h = h / 4;
    SDL_RenderDrawRect(renderer, &rect);
    SDL_RenderDrawLine(renderer, w * 7 / 8 + fsw_aileron * w / 8, h * 3 / 4, w * 7 / 8 + fsw_aileron * w / 8, h);
    SDL_RenderDrawLine(renderer, w * 3 / 4, h * 7 / 8 - fsw_elevator * h / 8, w, h * 7 / 8 - fsw_elevator * h / 8);

    rect.x = 0;
    SDL_RenderDrawRect(renderer, &rect);
    SDL_RenderDrawLine(renderer, w / 8 + fsw_rudder * w / 8, h * 3 / 4, w / 8 + fsw_rudder * w / 8, h);
    SDL_RenderDrawLine(renderer, 0, h * 7 / 8 - fsw_thrust * h / 8, w / 4, h * 7 / 8 - fsw_thrust * h / 8);

    // Present to screen
    ul.lock();
    SDL_RenderPresent(renderer);
    ul.unlock();

    this_thread::sleep_for(chrono::milliseconds(1000 / 60));
  }

  if (joystick) {
    SDL_JoystickClose(joystick);
  }
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}