#include <iomanip>
#include "Config.hh"


Object::Object() {
  double zeros[3] = {0.0};
  double ones[3] = {1.0};
  double color_128[3] = {128.0};
  this->shift = zeros;
  this->scale = ones;
  this->rot = zeros;
  this->transpos = zeros;
  this->color = color_128;
}

Object::Object(AbstractMobileObj* other) {
  this->name = other->GetName();
  this->shift = other->GetPositoin_m();
  this->rot[0] = other->GetAng_Roll_deg();
  this->rot[1] = other->GetAng_Pitch_deg();
  this->rot[2] = other->GetAng_Yaw_deg();
}

std::ostream & operator << ( std::ostream &OStrm, const Object &object)
{
  using server_ostream::operator<<;
  OStrm << "Name=" << object.name << " ";
  OStrm << "Shift=(" << std::setprecision(2) << object.shift << ") ";
  OStrm << "Scale=(" << object.scale << ") ";
  OStrm << "RotXYZ_deg=(" << object.rot << ") ";
  OStrm << "Trans_m=(" << object.transpos << ") ";
  OStrm << "RGB=(" << std::fixed << std::setprecision(0) << object.color << ") ";
  
  return OStrm;
}

std::string Object::toCommand() const {
  std::string result{};
  std::stringstream object_command;
  object_command << *this;
  getline(object_command, result);
  return result;
}


double Object::GetAng_Roll_deg() const {
  return this->rot[0];
}

double Object::GetAng_Pitch_deg() const {
  return this->rot[1];
}

double Object::GetAng_Yaw_deg() const {
  return this->rot[2];
}

void Object::SetAng_Roll_deg(double Ang_Roll_deg) {
  this->rot[0] = Ang_Roll_deg;
}

void Object::SetAng_Pitch_deg(double Ang_Pitch_deg) {
  this->rot[1] = Ang_Pitch_deg;
}

void Object::SetAng_Yaw_deg(double Ang_Yaw_deg) {
  this->rot[2] = Ang_Yaw_deg;
}

const Vector3D & Object::GetPositoin_m() const {
  return this->shift;
}

void Object::SetPosition_m(const Vector3D &rPos) {
  this->shift = rPos;
}

void Object::SetName(const char* sName) {
  this->name = {sName};
}

const std::string_view Object::GetName() const {
  return {this->name};
}