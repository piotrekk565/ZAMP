#ifndef CONFIG_HH
#define CONFIG_HH
#include <vector>
#include "Vector3D.hh"
#include "AbstractMobileObj.hh"

struct Plugin {
  std::string name{};
};

struct Object: public AbstractMobileObj {
  std::string name;
  Vector3D shift;
  Vector3D scale;
  Vector3D rot;
  Vector3D transpos;
  Vector3D color;

  Object();

  virtual double GetAng_Roll_deg() const;
  virtual double GetAng_Pitch_deg() const;
  virtual double GetAng_Yaw_deg() const;
  virtual void SetAng_Roll_deg(double Ang_Roll_deg);
  virtual void SetAng_Pitch_deg(double Ang_Pitch_deg);
  virtual void SetAng_Yaw_deg(double Ang_Yaw_deg);
  virtual const Vector3D & GetPositoin_m() const;
  virtual void SetPosition_m(const Vector3D &rPos);
  virtual void SetName(const char* sName);
  virtual const std::string_view GetName() const;

  std::string toCommand() const;

  Object(AbstractMobileObj* other);
};

struct Config {
  std::vector<Plugin> plugins{};
  std::vector<Object> objects{};
};

std::ostream & operator << ( std::ostream &OStrm, const Object &object);

#endif