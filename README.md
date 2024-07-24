# ROGUELIKE
A roguelike made with raylib.
Platforms: Linux, Windows, Browser


# TODOs
## Active (Rework features that existed before Restructuring)
- [ ] Keep filtered list of map tiles to render
- [ ] Realize viewport/camera system
- [ ] Add input to center on hero 
- [ ] Implement zoom
- [ ] Edge pan if player model goes out of deadzone area
- [ ] Implement movment (replace hold-to-move by L_SHIFT to move-until-something happens)
- [ ] Implement vision (fog of war: unseen, seen, visible)
- [ ] Print tile info in panel (under cursor)


## Shortterm
- [ ] Fire event on camera change to trigger tile filter recalculation
- [ ] Character triggers edgePan
- [ ] Changeable key bindings


## Longterm
- [ ] Add rogue theme (ASCII)
    - Floor : '.'
    - Wall  : '#'
    - Hero  : '@'
    - Stairs: '%'
- [ ] Map change
- [ ] Add enemy
    - World holds list of Units
- [ ] (Hold) KEY to show map overview
- [ ] Add touch support
- [ ] Save/Load for Account > Character > CurrentRun
- [ ] Account settings via lua


## Continuous
- Map generation

- Check for obsolete [[maybe_unused]] and //* comments


# Ideas
- [ ] Lua/Jinx settings file?


# Notes

## Controls

### Mouse
| Control     | Function                           |
| ----------- | ---------------------------------- |
| RMB         | Toggle cursor control (Mouse/Keys) |
| LMB         | Click to toggle select             |
| Mouse wheel | Zoom                               |


### Keyboard
| Control                           | Function                |
| --------------------------------- | ----------------------- |
| WASD / Arrow keys                 | Hero movement           |
| L_SHIFT                           | Continuous hero movment |
| CTRL + 0 (both numpad and normal) | Reset Zoom              |
| F1                                | Toggle developer mode   |
| F4                                | Exit                    |
| F11                               | Toggle Maximize window  |


### Touch
Not implemented yet

## Maptypes:
- Cellular Automata (dungeon)
- Rogue-like (rooms and tunnels)
- Square rooms grid
- Extend side -> new room wall
- Random border match
- Quad tree ()

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