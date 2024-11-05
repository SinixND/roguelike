# ROGUELIKE
    A roguelike made with raylib.
    Platforms: Linux, Windows, Browser


# TODOs
## Active (Rework features that existed before Restructuring)
- [ ] End/Restart game on death
- [ ] Add AI behaviour with new enemy types

| Default state before attacked                          | Name       |
| ------------------------------------------------------ | ---------- |
| stationary, defensive                                  | [M]imic    |
| moving, defensive                                      | [G]olem    |
| moving, aggressive                                     | [R]at      |
| moving, aggressive, invisible till (near or first hit) | [P]redator |
| Passes solids                                          | [S]hade    |
|                                                        | [D]jin     |


## Shortterm


## Longterm
- [ ] Add overlay boxes for enemy/object information (wanted?)
- [ ] Changeable key bindings
- [ ] Save/Load for Account > Character > CurrentRun
- [ ] Account settings via lua
- [ ] Implement zoom?
- [ ] (needed?!) Deactivate cursor trigger game action outside map panel 
- [ ] Setup docker (non-trvial for GUI apps)


## Continuous
- Check for possible NMNF functions
- Check for obsolete [[maybe_unused]] and //* comments


# Ideas
- [ ] (Hold) KEY to show map overview
- [ ] Lua/Jinx settings file?
- [ ] Use shaders (send matrix/vector of shadows by SetShaderValue()) and render one big black overlay with applied shader?


# Notes
## Controls
### Mouse
| Control | Function       |
| ------- | -------------- |
| RMB     | Toggle cursor  |
| LMB     | Move to cursor |


### Keyboard
| Control                  | Function                |
| ------------------------ | ----------------------- |
| WASD / Arrow keys / hjkl | Directional input       |
| L_SHIFT                  | Continuous hero movment |
| SPACE                    | Interact / Wait         |
| F1                       | Toggle developer mode   |
| F2                       | Cycle color themes      |
| F4                       | Exit                    |
| F11                      | Toggle Maximize window  |


### Touch
| Control      | Function                |
| ------------ | ----------------------- |
| SWIPE_(DIR)  | Directional input       |
| DRAG         | Continuous hero movment |
| (DOUBLE) TAP | Interact / Wait         |


## Maptypes:
- Cellular Automata (dungeon)
- Rogue-like (rooms and tunnels)
- Square rooms grid
- Extend side -> new room wall
- Random border match
- Quad tree ()


## Docker:
[Install docker](https://itsfoss.com/install-docker-arch-linux/)


# Implemented
- [x] Compile for linux & windows
- [x] Render sprites/textures
- [ ] Make ECS (removed)
- [ ] Command pattern for rendering and layers (will not be implemented -> need to handle abiguous keys in SparseSet: potential duplicate positions for tiles in different layers)
- [x] Refactor tilePosition to float position
- [x] Implement render area (have been named panels)
- [x] Always use Setter/Getter (keep open for possible later changes / flexibility)
- [x] Refactor using spritesheet
- [x] Refactor classes to emphasize NMNF (nonmember, nonfriend) functions
- [x] Change 'emscripten_set_main_loop()' to 'emscripten_set_main_loop_arg()'
- [x] Check function design (split long ones)
- [ ] Check movement functions in regards to game phases (obsolete after movement system change)
- [x] Modified parameters -> pointer
- [x] Observer pattern for event communication
- [x] Add logger
- [x] Keep filtered list of map tiles to render
- [x] Fire event on camera change to trigger tile filter recalculation
- [x] Implement viewport/camera system (-> raylib)
- [ ] Add input to center on hero (obsolete, camera follows hero)
- [ ] Edge pan if player model goes out of deadzone area (obsolete, camera follows hero)
- [ ] Character triggers edgePan (obsolete, camera follows hero)
- [x] Implement collision
- [x] Implement movment (replace hold-to-move by L_SHIFT to move-until-something happens)
- [x] Implement vision (fog of war: unseen, seen, visible)
- [x] Hold shift to keep moving (abort if key not down!)
- [x] Pathfind movement by clicking or taping
- [x] Add viewport size to gameCamera to reduce arguments
- [x] Print tile info in panel (under cursor, only special tiles!)
- [x] Split inactive and active map tiles
- [x] Map change
- [x] Reduce render calls by chunking map
- [x] Rework const correctness (includes: public members for direct access)
- [x] Rework Makefile
- [x] Add wait action
- [x] Add enemy movement
- [x] Split GameScene into Scene/Main and Game
- [x] Implement custom gesture registration (raylib has/had issues)
- [x] Add health and damage
- [x] Implement killing and spawning
- [x] Remove all includes and check for possible forward declarations