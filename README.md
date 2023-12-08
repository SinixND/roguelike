# ROGUELIKE
development of a (maybe) roguelike game with c++ and raylib

# REQUIREMENTS
- raylib
- emscripten

# TODOs
# Current
[0] rework current scene to non-singleton
[1] render function arguments
[2] player (controlable unit) at runtime
[3] units are globals -> move
[4] split globals and settings

# Shortterm
[0] plan struchture with UML
[0] implement environment

# Longterm 
[0] compile for linux & windows !

# Notes

## UML

```mermaid
classDiagram
    class main
    main *-- Scene : consists-of

    class Scene {
        <<abstract>>
        + update() void
        + initialize()* void
        + deinitialize()* void
    }
    Scene <|.. GameREWORKME : is-a

    class GameREWORKME{
        -Character player
    }
    GameREWORKME *-- Character : consists-of

    class Renderer{
        <<abstract>>
        + render(Renderable) void
    }
    Renderer <|-- GameREWORKME : is-a
    Renderer --> Renderable : uses

    class Renderable{
        <<Interface>>
        + render()* void
    }
    Renderable <|-- Character : is-a

    class Character
```

# Done
[x] Render unit
