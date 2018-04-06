class ControlPacketElement;
class CmdTlm;
#include <SDL.h>
#include <stdint.h>


class UI {
private:
  CmdTlm &cmdtlm;
  bool run;
  SDL_Texture *texture;
  SDL_Joystick *getJoystick();
  ControlPacketElement handleJoystick(SDL_Joystick *joystick);
  ControlPacketElement handleKeyboard(float &last_thrust);
public:
  UI(CmdTlm &cmdtlm);
  void mainLoop();
  void updateTexture();
  void updateTexture(const uint16_t frame[60][80]);
};
