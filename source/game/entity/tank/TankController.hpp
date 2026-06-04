#pragma once

#include "game/lua/ScriptContext.hpp"

#include <memory>

namespace game::entity {class Tank;}

namespace game::entity 
{

class TankController
{
public:
    TankController(game::entity::Tank* tank, std::vector<sf::Vector2i>& waypoints);
    virtual ~TankController() = default;
    lua::ScriptContext* getScript();
    void reload();

protected:
    std::unique_ptr<lua::ScriptContext> script_;
};

}  // namespace game::entity
