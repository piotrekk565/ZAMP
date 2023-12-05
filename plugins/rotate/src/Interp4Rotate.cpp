#include <iostream>
#include "Interp4Rotate.hh"
#include <unistd.h>


using std::cout;
using std::endl;


extern "C" {
  AbstractInterp4Command* CreateCmd(void);
  const char* GetCmdName() { return "Rotate"; }
}




/*!
 * \brief
 *
 *
 */
AbstractInterp4Command* CreateCmd(void)
{
  return Interp4Rotate::CreateCmd();
}


/*!
 *
 */
Interp4Rotate::Interp4Rotate()
{}


/*!
 *
 */
void Interp4Rotate::PrintCmd() const
{
  /*
   *  Tu trzeba napisać odpowiednio zmodyfikować kod poniżej.
   */
  cout << GetCmdName() << " " << this->objectName << " " << this->axis << " " << this->angleSpeed << " " << this->rotationAngle << endl;
}


/*!
 *
 */
const char* Interp4Rotate::GetCmdName() const
{
  return ::GetCmdName();
}


/*!
 *
 */
bool Interp4Rotate::ExecCmd(AbstractScene &rScn)
{
  auto optional_obj = rScn.FindMobileObj(this->objectName.c_str());
  if(!optional_obj.has_value()) {
    return false;
  }
  auto obj = optional_obj.value();

  int steps = 10*std::floor(this->rotationAngle/this->angleSpeed); //Krok co 100ms
  for (int i = 0; i < steps; i++)
  {
    Vector3D position = obj->GetPositoin_m();
    double angle_change = this->angleSpeed / static_cast<double>(steps);
    if( !this->axis.compare("X") ) {
      obj->SetAng_Roll_deg(obj->GetAng_Roll_deg() + angle_change);
    } else if( !this->axis.compare("X") ) {
      obj->SetAng_Pitch_deg(obj->GetAng_Pitch_deg() + angle_change);
    } else {
      obj->SetAng_Yaw_deg(obj->GetAng_Yaw_deg() + angle_change);
    }
    rScn.update(obj);
    usleep(100*1000);
  }

  return true;
}


/*!
 *
 */
bool Interp4Rotate::ReadParams(std::istream& Strm_CmdsList)
{
  Strm_CmdsList >> this->objectName >> this->axis >> this->angleSpeed >> this->rotationAngle;
  return Strm_CmdsList.good();
}


/*!
 *
 */
AbstractInterp4Command* Interp4Rotate::CreateCmd()
{
  return new Interp4Rotate();
}


/*!
 *
 */
void Interp4Rotate::PrintSyntax() const
{
  cout << "   Rotate  NazwaObiektu  Nazwa osi Szybkosc katowa  Kat obrotu" << endl;
}
