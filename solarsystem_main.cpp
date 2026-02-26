#include <SFML/Graphics.hpp>
#include <deque>
#include "solarsystem.hpp"


constexpr unsigned WINDOW_WIDTH = 1200;
constexpr unsigned WINDOW_HEIGHT = 800;

double SCALE = 1e9;

constexpr std::size_t TRAIL_LENGTH = 800;


int main() {
  sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),
                          "N-Body Simulation");

  window.setFramerateLimit(60);

  sf::View view(sf::FloatRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));
  view.setCenter(0.f, 0.f);
  window.setView(view);

  solar_system system;

  Body sun(1.989e30, 12.f, {0, 0, 0}, {0, 0, 0}, sf::Color::Yellow);

  Body earth(5.972e24, 5.f, {1.496e11, 0, 0}, {0, 29780, 0}, sf::Color::Blue);

  Body mars(6.39e23, 4.f, {2.279e11, 0, 0}, {0, 24070, 0}, sf::Color(200, 100, 80));

  system.add_body(sun);
  system.add_body(earth);
  system.add_body(mars);

  system.compute_accelerations();

  std::vector<std::deque<sf::Vector2f>> trails(system.bodies().size());

  double dt = 60 * 60;  

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) window.close();

      if (event.type == sf::Event::MouseWheelScrolled) {
        if (event.mouseWheelScroll.delta > 0)
          view.zoom(0.9f);
        else
          view.zoom(1.1f);
      }
    }

    float camSpeed = 10.f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) view.move(-camSpeed, 0);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) view.move(camSpeed, 0);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) view.move(0, -camSpeed);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) view.move(0, camSpeed);

    window.setView(view);

    system.step(dt);

    auto const& bodies = system.bodies();

    for (std::size_t i = 0; i < bodies.size(); ++i) {
      float x = bodies[i].coordinates.x / SCALE;
      float y = bodies[i].coordinates.y / SCALE;

      trails[i].push_back({x, y});

      if (trails[i].size() > TRAIL_LENGTH) trails[i].pop_front();
    }

    window.clear(sf::Color::Black);

    for (std::size_t i = 0; i < trails.size(); ++i) {
      if (trails[i].size() < 2) continue;

      sf::VertexArray line(sf::LineStrip, trails[i].size());

      for (std::size_t j = 0; j < trails[i].size(); ++j) {
        line[j].position = trails[i][j];
        line[j].color = bodies[i].color;
      }

      window.draw(line);
    }

    for (auto const& body : bodies) {
      sf::CircleShape shape(body.radius);
      shape.setFillColor(body.color);
      shape.setOrigin(body.radius, body.radius);

      float x = body.coordinates.x / SCALE;
      float y = body.coordinates.y / SCALE;

      shape.setPosition(x, y);

      window.draw(shape);
    }

    window.display();
  }

  return 0;
}