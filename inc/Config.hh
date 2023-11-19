#ifndef CONFIG_HH
#define CONFIG_HH
#include <vector>
#include <optional>
#include <sstream>
#include "Vector3D.hh"

struct Plugin {
  std::string name{};
};

struct Object {
  std::string name;
  Vector3D shift;
  Vector3D scale;
  Vector3D rot;
  Vector3D transpos;
  Vector3D color;

  Object() {
    double zeros[3] = {0.0};
    double ones[3] = {1.0};
    double color_128[3] = {128.0};
    this->shift = zeros;
    this->scale = ones;
    this->rot = zeros;
    this->transpos = zeros;
    this->color = color_128;
  }
};

struct Config {
  std::vector<Plugin> plugins{};
  std::vector<Object> objects{};
};

#endif