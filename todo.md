# Crude TODO list
Bunch of random ideas to randomly pick up and implement

## Engine
### Physics
- [ ] Circle based basic collisions
- [ ] Rigid body simulation generalization to simulate other objects than tanks (still circle based)
- [ ] AABB box collisions
- [ ] Variable time step in physics calulcations
- [ ] Ray casting for collisions with objects (for further sensors implementation)

### Visuals
- [x] Fluid camera support with zoom capabilites

### GUI
- [x] Basic GUI subsystem support for spawning windows with some text 
- [x] Buttons
- [ ] Closable windows
- [ ] FIXME Event polling is generating events every render frame
- [ ] PERFORMANCE Render gui on separate texture and rerender only on change

## Gameplay
- [x] Basic waypoint travel mechanism for physics and visual testing
- [ ] Shooting projectiles, every tank can do it right?
- [ ] Fuel management, pickups
- [ ] Distance sensors

## Lua API
- [ ] Some movement basic API (rotate, driving) with lua bindings
- [ ] Scanner/radar to find other tanks and pickups

## Others
- [ ] Address in code TODOs
- [x] Create UT target and write few crude tests
- [ ] Build system should split production and test targets to not build UTs in production
- [ ] Split github actions to run production and UT separately