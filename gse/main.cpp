#include <stdio.h>
#include <iostream>
using namespace std;

void sendDir(float Pitch, float Yaw, float Roll, float Thrust)
{
  //toDo sendDir to FSW for CIARAN
}

int main(int argc, char* argv[]) {

  //for now the user will have to input the values, once we get into the
  //thick of things we will enhanced control
  float Pitch, Yaw, Roll, Thrust;
  cout << "Please enter the pitch: ";
  cin >> Pitch;
  cout << "Please enter the Yaw: ";
  cin >> Yaw;
  cout << "Please enter the Roll: ";
  cin >> Roll;
  cout << "Please enter the thrust: ";
  cin >> Thrust;

  sendDir(Pitch, Yaw, Roll, Thrust);

  return 0;
}
