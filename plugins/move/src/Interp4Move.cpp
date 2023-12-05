#include <iostream>
#include "Interp4Move.hh"
#include <unistd.h>


using std::cout;
using std::endl;


extern "C" {
  AbstractInterp4Command* CreateCmd(void);
  const char* GetCmdName() { return "Move"; }
}




/*!
 * \brief
 *
 *
 */
AbstractInterp4Command* CreateCmd(void)
{
  return Interp4Move::CreateCmd();
}


/*!
 *
 */
Interp4Move::Interp4Move()
{}


/*!
 *
 */
void Interp4Move::PrintCmd() const
{
  /*
   *  Tu trzeba napisać odpowiednio zmodyfikować kod poniżej.
   */
  cout << GetCmdName() << " " << this->objectName << " " << this->speed << " " << this->pathLength << endl;
}


/*!
 *
 */
const char* Interp4Move::GetCmdName() const
{
  return ::GetCmdName();
}


/*!
 *
 */
bool Interp4Move::ExecCmd(AbstractScene &rScn)
{
  auto optional_obj = rScn.FindMobileObj(this->objectName.c_str());
  if(!optional_obj.has_value()) {
    return false;
  }
  auto obj = optional_obj.value();

  int steps = 10*std::floor(this->pathLength/this->speed); //Krok co 100ms
  for (int i = 0; i < steps; i++)
  {
    Vector3D position = obj->GetPositoin_m();

    position[0] += this->speed / static_cast<double>(steps);
    obj->SetPosition_m(position);
    rScn.update(obj);
    usleep(100*1000);
  }

  return true;
}


/*!
 *
 */
bool Interp4Move::ReadParams(std::istream& Strm_CmdsList)
{
  Strm_CmdsList >> this->objectName >> this->speed >> this->pathLength;
  return Strm_CmdsList.good();
}


/*!
 *
 */
AbstractInterp4Command* Interp4Move::CreateCmd()
{
  return new Interp4Move();
}


/*!
 *
 */
void Interp4Move::PrintSyntax() const
{
  cout << "   Move  NazwaObiektu  Szybkosc[m/s]  DlugoscDrogi[m]" << endl;
}
