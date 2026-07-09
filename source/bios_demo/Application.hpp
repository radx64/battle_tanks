#pragma once

#include <SFML/Graphics.hpp>

#include "engine/Application.hpp"

namespace bios
{

class Application : public engine::Application
{
public:
    Application();

protected:
    void onInit() override;
    void onClose() override;
    void onEvent(const sf::Event& event) override;
    void onUpdate(float timeStep) override;
    void onRender() override;

private:
    void drawScene(sf::RenderTarget& target);
    void drawPostScreen(sf::RenderTarget& target, float t);
    void drawBootScreen(sf::RenderTarget& target, float t);

    void drawText(sf::RenderTarget& target, const std::string& text,
        float x, float y, sf::Color color = sf::Color(0xC0, 0xC0, 0xC0));

    sf::Font font_;
    sf::Text text_;

    sf::RenderTexture screen_;
    sf::Sprite screenSprite_;
    sf::Shader crtShader_;
    bool crtAvailable_{false};

    sf::Clock bootClock_;
    float elapsed_{0.f};
    static constexpr float BOOT_DURATION_{25.f};
};

}  // namespace bios
