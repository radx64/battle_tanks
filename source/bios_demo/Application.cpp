#include "Application.hpp"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <string>
#include <vector>

#include <fmt/format.h>

namespace bios
{

namespace
{
constexpr float WINDOW_WIDTH = 1024.f;
constexpr float WINDOW_HEIGHT = 768.f;

constexpr unsigned int CHAR_SIZE = 18;
constexpr float LINE_HEIGHT = 22.f;
constexpr float MARGIN_X = 48.f;
constexpr float MARGIN_Y = 40.f;

const sf::Color TEXT_GREY{0xC0, 0xC0, 0xC0};
const sf::Color TEXT_WHITE{0xF0, 0xF0, 0xF0};
const sf::Color TEXT_CYAN{0x40, 0xC0, 0xC0};
const sf::Color TEXT_YELLOW{0xE0, 0xE0, 0x40};

constexpr std::uint32_t MEMORY_TOTAL_KB = 65536;

float clampf(float v, float lo, float hi)
{
    return std::max(lo, std::min(hi, v));
}
}  // namespace

Application::Application()
: engine::Application{"CRT shader test", "bios", {WINDOW_WIDTH, WINDOW_HEIGHT}}
{
}

void Application::onInit()
{
    if (not font_.loadFromFile("./resources/fonts/Px437_IBM_VGA_8x16.ttf"))
    {
        logger_.error("Could not load BIOS font.");
    }
    text_.setFont(font_);
    text_.setCharacterSize(CHAR_SIZE);

    // The base Application draws a profiling overlay each frame; make it
    // invisible so it does not clutter the BIOS screen.
    profilingText_.setFillColor(sf::Color::Transparent);
    profilingText_.setOutlineThickness(0.f);

    screen_.create(static_cast<unsigned int>(WINDOW_WIDTH),
        static_cast<unsigned int>(WINDOW_HEIGHT));
    screenSprite_.setTexture(screen_.getTexture(), true);

    if (sf::Shader::isAvailable() &&
        crtShader_.loadFromFile("./resources/shaders/crt.frag", sf::Shader::Fragment))
    {
        crtAvailable_ = true;
        crtShader_.setUniform("texture", sf::Shader::CurrentTexture);
        crtShader_.setUniform("resolution", sf::Glsl::Vec2{WINDOW_WIDTH, WINDOW_HEIGHT});
        logger_.debug("CRT shader loaded.");
    }
    else
    {
        logger_.error("CRT shader unavailable; rendering without post-processing.");
    }

    bootClock_.restart();
}

void Application::onClose()
{
}

void Application::onEvent(const sf::Event& event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::Escape)
        {
            close();
        }
        else if (event.key.code == sf::Keyboard::R)
        {
            bootClock_.restart();
        }
    }
}

void Application::onUpdate(float /*timeStep*/)
{
    elapsed_ = bootClock_.getElapsedTime().asSeconds();
    if (elapsed_ > BOOT_DURATION_)
    {
        bootClock_.restart();
        elapsed_ = 0.f;
    }
}

void Application::onRender()
{
    screen_.clear(sf::Color::Black);
    drawScene(screen_);
    screen_.display();

    if (crtAvailable_)
    {
        crtShader_.setUniform("time", elapsed_);
        window_.draw(screenSprite_, &crtShader_);
    }
    else
    {
        window_.draw(screenSprite_);
    }
}

void Application::drawText(sf::RenderTarget& target, const std::string& text,
    float x, float y, sf::Color color)
{
    text_.setString(text);
    text_.setFillColor(color);
    text_.setPosition(std::round(x), std::round(y));
    target.draw(text_);
}

void Application::drawScene(sf::RenderTarget& target)
{
    // Timeline of the boot sequence (seconds):
    //   0.0 - 1.2 : black screen, monitor warming up
    //   1.2 - 9.0 : AMI POST screen (memory count-up + prompts)
    //   9.0 - 18  : device detection + hand-off to the boot loader
    constexpr float POST_START = 1.2f;
    constexpr float BOOT_START = 9.0f;

    if (elapsed_ < POST_START)
    {
        return;  // warm-up: nothing on screen yet
    }

    if (elapsed_ < BOOT_START)
    {
        drawPostScreen(target, elapsed_ - POST_START);
    }
    else
    {
        drawBootScreen(target, elapsed_ - BOOT_START);
    }
}

void Application::drawPostScreen(sf::RenderTarget& target, float t)
{
    float y = MARGIN_Y;
    const float x = MARGIN_X;

    drawText(target, "RADEKS BIOS(C)2026 Polish Semiconductors Inc.,", x, y, TEXT_WHITE);
    y += LINE_HEIGHT;
    drawText(target, "BIOS Date: 07/09/26 22:11:04 Ver: 08.00.15", x, y);
    y += LINE_HEIGHT * 2.f;

    drawText(target, "Press DEL to run Setup", x, y);
    y += LINE_HEIGHT;
    drawText(target, "Press F11 for BOOT OPTIONS", x, y);
    y += LINE_HEIGHT * 2.f;

    drawText(target, "CPU : Battle Tanks(R) Core(TM) i0 @ 166MHz", x, y);
    y += LINE_HEIGHT;
    drawText(target, "Speed : 166 MHz", x, y);
    y += LINE_HEIGHT * 2.f;


    const float memProgress = clampf(t / 3.5f, 0.f, 1.f);
    std::uint32_t testedKb =
        static_cast<std::uint32_t>(std::round(MEMORY_TOTAL_KB * memProgress));
    testedKb = (testedKb / 1024u) * 1024u;  // step in 1 MB blocks

    if (memProgress < 1.f)
    {
        drawText(target, fmt::format("Memory Test : {:>8}K", testedKb), x, y, TEXT_YELLOW);
    }
    else
    {
        drawText(target,
            fmt::format("Memory Test : {:>8}K OK", MEMORY_TOTAL_KB), x, y, TEXT_YELLOW);
    }

    const float logoX = WINDOW_WIDTH - MARGIN_X - 150.f;
    sf::RectangleShape logo{sf::Vector2f{170.f, 66.f}};
    logo.setPosition(std::round(logoX), MARGIN_Y);
    logo.setFillColor(sf::Color::Black);
    logo.setOutlineColor(TEXT_CYAN);
    logo.setOutlineThickness(2.f);
    target.draw(logo);
    drawText(target, "ENERGY STAR LOGO", logoX + 14.f, MARGIN_Y + 8.f, TEXT_CYAN);
    drawText(target, "SHOULD BE HERE", logoX + 20.f, MARGIN_Y + 34.f, TEXT_CYAN);

    // Bottom status bar with a fake tag string.
    drawText(target,
        "(C)Polish Semiconductors, Inc.",
        x, WINDOW_HEIGHT - MARGIN_Y - LINE_HEIGHT * 2.f, TEXT_GREY);
    drawText(target,
        "60-0409-009999-00101111-070926-BTANKS-1",
        x, WINDOW_HEIGHT - MARGIN_Y - LINE_HEIGHT, TEXT_GREY);
}

void Application::drawBootScreen(sf::RenderTarget& target, float t)
{
    float y = MARGIN_Y;
    const float x = MARGIN_X;

    struct Line
    {
        float at;            // seconds into the boot phase before it appears
        std::string text;
        sf::Color color;
    };

    static const std::vector<Line> lines = {
        {0.0f, "Detecting IDE drives ...", TEXT_GREY},
        {0.8f, "  Primary Master   : ST3500 Battle Tanks HDD  2GB", TEXT_GREY},
        {1.4f, "  Primary Slave    : None", TEXT_GREY},
        {2.0f, "  Secondary Master : Floppy 1.44MB", TEXT_GREY},
        {2.6f, "  Secondary Slave  : None", TEXT_GREY},
        {3.4f, "", TEXT_GREY},
        {3.6f, "Initializing USB Controllers .. Done.", TEXT_GREY},
        {4.4f, "Auto-Detecting Boot Devices ... Done.", TEXT_GREY},
        {5.2f, "", TEXT_GREY},
    };

    for (const auto& line : lines)
    {
        if (t >= line.at)
        {
            drawText(target, line.text, x, y, line.color);
        }
        y += LINE_HEIGHT;
    }

    if (t >= 5.6f)
    {
        const int dots = std::min(12, static_cast<int>((t - 5.6f) * 12.f));
        drawText(target,
            "Verifying DMI Pool Data " + std::string(std::max(0, dots), '.'),
            x, y, TEXT_GREY);
    }
    y += LINE_HEIGHT * 2.f;

    if (t >= 7.0f)
    {
        drawText(target, "Booting from Hard Disk...", x, y, TEXT_WHITE);
        y += LINE_HEIGHT;
    }

    if (t >= 10.0f)
    {
        drawText(target, "No bootable devices found. Press F12 to reboot.", x, y, TEXT_WHITE);
        y += LINE_HEIGHT;
    }

    if (t >= 11.6f)
    {
        const bool on = std::fmod(elapsed_, 1.0f) < 0.5f;
        if (on)
        {
            sf::RectangleShape cursor{sf::Vector2f{10.f, 18.f}};
            cursor.setPosition(x, std::round(y + 2.f));
            cursor.setFillColor(TEXT_WHITE);
            target.draw(cursor);
        }
    }
}

}  // namespace bios
