class ControlPacketElement;
class CmdTlm;
#include <SDL.h>
#include <stdint.h>
#include <mutex>
#include <commands.hpp>


class UI : public Commands{
protected:
  CmdTlm &cmdtlm;
  bool run;
  mutex texture_mutex;
  int blobx, bloby;
  SDL_Texture *texture;
  SDL_Joystick *getJoystick();
  ControlPacketElement handleJoystick(SDL_Joystick *joystick);
  ControlPacketElement handleKeyboard(float &last_thrust);
public:
  UI(CmdTlm &cmdtlm);
  void mainLoop();
  virtual void lwirFrame(const uint16_t frame[60][80]);
  virtual void blob(uint16_t x, uint16_t y);
};
