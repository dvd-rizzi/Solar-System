#ifndef SOLAR_SYSTEM_HPP
#define SOLAR_SYSTEM_HPP

#include <SFML/Graphics.hpp>
#include <cmath>
#include <string>
#include <vector>

struct vec3 {
  double x{0.0};
  double y{0.0};
  double z{0.0};

  vec3& operator+=(vec3 const& o) {
    x += o.x;
    y += o.y;
    z += o.z;
    return *this;
  }
  vec3& operator-=(vec3 const& o) {
    x -= o.x;
    y -= o.y;
    z -= o.z;
    return *this;
  }

  vec3 operator+(vec3 const& o) const { return {x + o.x, y + o.y, z + o.z}; }
  vec3 operator-(vec3 const& o) const { return {x - o.x, y - o.y, z - o.z}; }
  vec3 operator*(double s) const { return {x * s, y * s, z * s}; }
  vec3 operator/(double s) const { return {x / s, y / s, z / s}; }

  double norm2() const { return x * x + y * y + z * z; }
  double norm() const { return std::sqrt(norm2()); }
};

struct Body {
  double mass;
  double radius;

  vec3 coordinates;
  vec3 speed;
  vec3 force{0, 0, 0};
  vec3 acceleration{0, 0, 0};

  sf::Color color;

  Body(double mass_, double radius_, vec3 coordinates_, vec3 speed_,
       sf::Color color_)
      : mass(mass_),
        radius(radius_),
        coordinates(coordinates_),
        speed(speed_),
        color(color_) {}
};

class solar_system {
  std::vector<Body> system;
  double const G = 6.67430e-11;

 public:

  std::vector<Body> const& bodies() const;

  void add_body(Body const& planet);

  void gravitational_pull(Body& a, Body& b);

  void compute_accelerations();

  void compute_position(double dt);

  void compute_speeds(const std::vector<vec3>& old_acc, double dt);

  void step(double dt);
};

#endif