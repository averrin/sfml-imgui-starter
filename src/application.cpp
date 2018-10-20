#include "app/application.hpp"

#include <imgui-sfml/imgui-SFML.h>
#include <imgui.h>

#include <libcolor/libcolor.hpp>

Application::Application(std::string app_name, std::string version)
    : APP_NAME(app_name), VERSION(version) {
  sf::ContextSettings settings;
  settings.antialiasingLevel = 8;
  ImGui::CreateContext();

  ImGuiIO &io = ImGui::GetIO();

  window = new sf::RenderWindow(sf::VideoMode::getDesktopMode(), APP_NAME,
                                sf::Style::Default, settings);
  window->setVerticalSyncEnabled(true);
  ImGui::SFML::Init(*window);

  window->resetGLStates();
}

void Application::processEvent(sf::Event event) {
  ImGui::SFML::ProcessEvent(event);
  switch (event.type) {
  case sf::Event::KeyPressed:
    switch (event.key.code) {
    case sf::Keyboard::Escape:
      window->close();
      break;
    }
    break;
  case sf::Event::Closed:
    window->close();
    break;
  }
}

float value = 1.f;
int count = 15;
float size = 50.f;

void Application::drawMainWindow() {
  ImGui::Begin(APP_NAME.c_str());
  ImGui::Text("\n\nSFML + ImGui starter (%s)\n\n", VERSION.c_str());
  ImGui::SliderInt("Count", &count, 1, 200);
  ImGui::SliderFloat("Size", &size, 4.f, 200.f);
  ImGui::SliderFloat("Value", &value, 0.f, 1.f);
  ImGui::End();
}

int Application::serve() {
  log.info("serve");
  sf::Clock deltaClock;
  while (window->isOpen()) {
    sf::Event event;
    while (window->pollEvent(event)) {
      processEvent(event);
    }
    auto bgColor = sf::Color(23, 23, 23);
    window->clear(bgColor);

    float padding = size/10;
    auto yOffset = 0.f;
    for (auto y = 0; y < count; y++) {
      auto offset = 0.f;
      auto startColor = Color::fromWebName("red");
      startColor.saturation(1 - y/float(count));
      startColor.value(value);
      for (auto i = 0; i < count; i++) {
        auto sfColor =
            sf::Color(startColor.red(), startColor.green(), startColor.blue());

        sf::VertexArray quad(sf::Quads, 4);

        quad[0].position = sf::Vector2f(padding + offset, padding + yOffset);
        quad[0].color = sfColor;
        quad[1].position = sf::Vector2f(padding + size + offset, padding + yOffset);
        quad[1].color = sfColor;
        quad[2].position = sf::Vector2f(padding + size + offset, padding + size + yOffset);
        quad[2].color = sfColor;
        quad[3].position = sf::Vector2f(padding + offset, padding + size + yOffset);
        quad[3].color = sfColor;

        window->draw(quad);
        offset += size + padding;
        startColor.hue(startColor.hue() + 360.f / count);
      }
      yOffset += size + padding;
    }

    ImGui::SFML::Update(*window, deltaClock.restart());
    drawMainWindow();
    ImGui::SFML::Render(*window);
    window->display();
  }
  log.info("shutdown");
  ImGui::SFML::Shutdown();
  return 0;
}
