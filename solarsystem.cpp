#include "solarsystem.hpp"

#include <algorithm>
#include <numeric>

class solar_system {
  std::vector<Body> system;
  double const G = 6.67430e-11;

 public:
  void add_body(Body const& planet) { system.push_back(planet); }

  void gravitational_pull(Body& a, Body& b) {
    vec3 gravitational_force;
    vec3 r = b.coordinates - a.coordinates;
    double dist = r.norm();
    if (dist > 0) {
      vec3 r_hat = r / dist;
      gravitational_force = r_hat * (G * (a.mass * b.mass) / (dist * dist));
    }
    a.force += gravitational_force;
    b.force -= gravitational_force;
  }

  void compute_accelerations() {
    for (auto& body : system) {
      body.force = vec3{0, 0, 0};
    }

    for (int i{0}; i < system.size(); ++i) {
      for (int j{i + 1}; j < system.size(); ++j) {
        gravitational_pull(system[i], system[j]);
      }
    }
    for (auto& body : system) {
      body.acceleration = body.force / body.mass;
    }
  }

  void compute_position(double dt) {
    for (auto& body : system) {
      body.coordinates += body.speed * dt + body.acceleration * 0.5 * dt * dt;
    }
  }

  void compute_speeds(const std::vector<vec3>& old_acc, double dt) {
    for (int i = 0; i < system.size(); ++i) {
      system[i].speed += (old_acc[i] + system[i].acceleration) * 0.5 * dt;
    }
  }

  void step(double dt) {
    std::vector<vec3> old_acc(system.size());
    for (int i = 0; i < system.size(); ++i) old_acc[i] = system[i].acceleration;

    compute_position(dt);

    compute_accelerations();

    compute_speeds(old_acc, dt);
  }
  
};