# Crude TODO list
Bunch of random ideas to randomly pick up and implement

## Engine
### Physics
- [x] Circle based basic collisions
- [x] Rigid body simulation generalization to simulate other objects than tanks (still circle based)
- [ ] AABB/OBB box collisions
- [ ] Variable time step in physics calulcations
- [ ] Ray casting for collisions with objects (for further sensors implementation)

### Visuals
- [x] Fluid camera support with zoom capabilites

### GUI
- [x] Basic GUI subsystem support for spawning windows with some text
- [x] Buttons
- [x] Closable windows
- [x] Resizable windows
    - [x] 🐛: You can close window while resizing by passing cursor over close button
    - [x] 🐛: When window is first resized and then dragged it jumps to some offset position (like where topbar was before resize)
    - [x] 🐛: Window focusing is not working properly after resizing
- [ ] Some basic dynamic layout support like (consider removing alignment variable in position in favour of layout):
  - [x] Horizontal
  - [x] Vertical
  - [ ] Box (like 2x2 grid)
- [ ] Edit box
  - [x] Text input
  - [ ] Selection support
    - [ ] 🐛 Moving cursor does not clear previous selection
  - [ ] Copy paste mechanics
- [x] 🐛 Event polling is generating events every render frame, also creating some nice events would be easier to handle
- [ ] 🏎️ Render gui on separate texture and rerender only on changes
- [ ] Consider creating render command batches and split gui from sfml implementation

## Gameplay
- [x] Basic waypoint travel mechanism for physics and visual testing
- [x] Shooting projectiles, every tank can do it right?
- [ ] Fuel management, pickups
- [ ] Distance sensors

## Lua API
- [ ] Some movement basic API (rotate, driving) with lua bindings
- [ ] Scanner/radar to find other tanks and pickups

## Others
- [ ] Address in code TODOs and FIXMEs :) meh
- [x] Create UT target and write few crude tests
- [ ] Build system should split production and test targets to not build UTs in production
- [ ] Split github actions to run production and UT separately
