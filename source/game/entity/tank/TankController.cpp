#include "game/entity/tank/TankController.hpp"

#include <vector>

#include <SFML/System.hpp>

namespace game::entity 
{

TankController::TankController(std::string_view scriptFile, game::entity::Tank* tank, std::vector<sf::Vector2i>& waypoints)
{
    script_ = std::make_unique<lua::ScriptContext>(scriptFile, tank, waypoints);
}

lua::ScriptContext* TankController::getScript()
{
    if (!script_)
    {
        throw std::runtime_error("Tank does not have script attached");
    }
    return script_.get();
};

void TankController::reload()
{
    if (script_)
    {
        script_->reload();
    }
}

}  // namespace game::entity
