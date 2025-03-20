# ROGUELIKE
A roguelike made with raylib to practise programming (concepts).
Platforms: Linux, Windows, Browser


# TODOs
## Active 
- [ ] Extend attributes/leveling
    - Possibility of classes: different starting distribution
    - And/Or change class if certain distribution threshold(s) met
- [ ] Enemy scaling


## Shortterm
- [ ] Add inspection mode
- [ ] CombatSystem
- [ ] Add AI behaviour with new enemy types
    - Simple types named after stats? (`Normal/Default/Standard`, `Strong`, `Quick`, `Healthy`)


## Longterm
- [ ] Colorize enemies: weak: green, normal: white(?), strong: red, above: purple
- [ ] Regen when exploring undetected areas?
- [ ] Add overlay boxes for enemy/object information (wanted?)
- [ ] Changeable key bindings
- [ ] Save/Load for Account > Character > CurrentRun
- [ ] Account settings via lua
- [ ] Implement zoom?
- [ ] Setup docker (non-trvial for GUI apps?)

## Ideas
- [ ] Make game a mix of `rogue` and `factorio`
- [ ] (Hold) KEY to show map overview
- [ ] 2.5D display? (consider change of visible map -> direction biased, camera rotation?)
- [ ] Lua/Jinx settings file?
- [ ] Use shaders
    - (send matrix/vector of shadows by SetShaderValue()) and render one big black overlay with applied shader?
    - to render Fogs

## Continuous
- Check for obsolete `[[maybe_unused]]`, `//` comments and `TODO`: 
- [ ] Check on emscripten mouse bug [seems to be emscripten side, investigate later; probably not an easy fix]


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


## Concepts used in project
- Generic Makefile (no cmake)
- Support for linux / web (library: emscripten) / windows
- Support for keyboard / mouse / touch
- Texture/Sprite rendering (library: raylib)
- Making of a simple ECS
    - Research on SparseSet and Archetype
- OOP/DOD
    Lots of thoughts went into OOP vs DOD/POD design.
    The basic takeaway was: 
    - loose coupling/dependencies via getters/setters/interfaces; more code, possible cost in performance and complexity;
    - PODs for coupled design, but clear program
    - default to: public and non-member/non-friend until specific action needed
- AoS classes (for single objects) vs SoA (for multiple)
- Made DenseMap(arbitrary id)/SparseSet(`size_t` id) for contiguous container with persistant id on erase
- Static publisher-subscriber system to communicate between different program areas (only enums, no variables)
- Shadowcast a FogOfWar layer over Tiles
- Time-dependent / smooth movement 
- A* pathfinding
- Procedural map generation


# Implemented ToDo's
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
- [x] Split Transform (single turn) and Movement (multi-turn) data
- [x] Refactor public/private (see Notes) ~~namespace Class/Module/System~~
- [x] Separate System (not related to class) and Module (former member functions) via namespace?
- [x] Refactor to DenseMap<size_t, Type> (instead of SparseSet -> check emscripten)
- [x] Refactor to fake-return function signature (returns modified parameter)
- [ ] Split Component(temp) and Data(persistent)? (existential programming) -> not used, more cognitive load by hidden state, considered micro-optimization
- [ ] Implement ncurses (raycurses?) -> no
- [x] End/Restart game on death
- [x] Add experience system
- [x] Add attribute system
