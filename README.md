# ROGUELIKE
development of a (maybe) roguelike game with c++ and raylib

# REQUIREMENTS
- raylib
- emscripten

# TODOs
# active
[0] Rework pathfinding

[1] Debug doubled tiles 3rd time (entity == 305 & 385 && texture->id == 7 && position.x == -1 && position.y == 6)

# Shortterm
[0] Implement movment

[1] Show path

[2] Transform (S) relative to **selected** (C) (Cursor if unselected)


# Longterm 
[1] Map generation


# Continuous


# Notes
ECS-Analogy:
- Class members become components, boolean members (states) become tags (= component without data)
- Class methods become systems
- Objects become Entities

Maptypes:
- Cellular Automata (dungeon)
- Rogue-like (rooms and tunnels)
- Square rooms grid
- Extend side -> new room wall
- Random border match

Window-sensitive rendering: https://www.raylib.com/examples/core/loader.html?name=core_window_letterbox


# Done
[x] compile for linux & windows !

[x] Render sprites/textures

[x] make ECS

[x] realize viewport system (render relative to window -> update Rendersystem: pos 0,0 & transform 0, 0 == center of screen)

[x] adjust texture to default direction right

[x] Refactor Map as Entity that has (Tile)Entities-Component which holds ar array of entities, created by SMapSystem that holds internally a static level info, creates map if level is not equal Configs entry (game state)