#include <iostream>
#include "Interp4Set.hh"
#include <unistd.h>


using std::cout;
using std::endl;


extern "C" {
  AbstractInterp4Command* CreateCmd(void);
  const char* GetCmdName() { return "Set"; }
}




/*!
 * \brief
 *
 *
 */
AbstractInterp4Command* CreateCmd(void)
{
  return Interp4Set::CreateCmd();
}


/*!
 *
 */
Interp4Set::Interp4Set()
{}


/*!
 *
 */
void Interp4Set::PrintCmd() const
{
  /*
   *  Tu trzeba napisać odpowiednio zmodyfikować kod poniżej.
   */
  using proper_ostream::operator<<;
  cout << GetCmdName() << " " << this->objectName << " " << this->position << " " << this->angle << endl;
}


/*!
 *
 */
const char* Interp4Set::GetCmdName() const
{
  return ::GetCmdName();
}


/*!
 *
 */
bool Interp4Set::ExecCmd(AbstractScene &rScn)
{
  auto optional_obj = rScn.FindMobileObj(this->objectName.c_str());
  if(!optional_obj.has_value()) {
    return false;
  }
  auto obj = optional_obj.value();
  obj->SetPosition_m(this->position);
  obj->SetAng_Roll_deg(this->angle[0]);
  obj->SetAng_Pitch_deg(this->angle[1]);
  obj->SetAng_Yaw_deg(this->angle[2]);
  rScn.update(obj);
  usleep(250000);

  return true;
}


/*!
 *
 */
bool Interp4Set::ReadParams(std::istream& Strm_CmdsList)
{
  Strm_CmdsList >> this->objectName >> this->position >> this->angle;
  return Strm_CmdsList.good();
}


/*!
 *
 */
AbstractInterp4Command* Interp4Set::CreateCmd()
{
  return new Interp4Set();
}


/*!
 *
 */
void Interp4Set::PrintSyntax() const
{
  cout << "   Set  NazwaObiektu  wsp_x wsp_y wsp_z kat_Ox kat_Oy kat_Oz" << endl;
}
