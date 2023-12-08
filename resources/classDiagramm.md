```mermaid
classDiagram 
A ..> B : uses
note for A "as parameter" 
C --> D : holds 
note for C "reference"
E --|> F : is-a 
note for E "inherits base"
G ..|> H : implements 
note for G "inherits interface"
I o-- J : has 
note for I "copied instance"
K *-- L : owns 
note for K "created instance"
```

```mermaid
classDiagram 
    class main {
        + ActiveScene activeScene
        + appLoop() void
    }
    main *-- Scene : owns
    main ..> ActiveScene : uses

    class Scene {
        <<Interface>>
        + update()* void
        + initialize()* void
        + deinitialize()* void
        + processInput()* void
        + updateState()* void
        + renderOutput()* void
    }
    Scene ..> ActiveScene : uses

    class ActiveScene {
        - Scene scene
        - ActiveScene instance
        + get() Scene
        + set(Scene)
        + getInstance() ActiveScene
    }
    ActiveScene --> ActiveScene


    class GameScene {
        + InitializeMap()
    }
    GameScene ..|> Scene : implements
    GameScene *-- Renderer : owns
    GameScene *-- Controller : owns

    class Renderer {
        + execute(Renderable) void
    }
    Renderer --> Renderable : uses

    class Renderable {
        <<Interface>>
        + render()* void
    }

    class Positional {
        <<Interface>>
        - Vector2 position
        + getPosition() Vector2
        + setPosition() Vector2
        + move(Direction, distance)* void
    }

    class Controller {
        + execute(Controllable) void
    }

    class Controllable {
        <<Interface>>
        - bool selected
        + isSelected()* bool
        + select()* void
    }
    Controllable ..|> Positional : implements

    class Map { 
        + Field[] fields
        + Tile[] tiles
        + Unit[] units
        + Item[] items
    }
    note for Map "key-value-container"
    Map *-- Field : has
    Map *-- Tile : has
    Map *-- Item : has
    Map *-- Unit : has

    class Field {
        Vector2 position
        Tile tile
        Item item
        Unit unit
    }
    Field --> Tile : holds
    Field --> Item : holds
    Field --> Unit : holds

    class Tile {
        <<Interface>>
        - bool passable
        + isPassable() bool
    }

    class Wall
    Wall ..|> Tile : implements
    Wall ..|> Positional : implements
    Wall ..|> Renderable : implements
    
    class Floor 
    Floor ..|> Tile : implements
    Floor ..|> Positional : implements
    Floor ..|> Renderable : implements


    class Unit {
        <<Interface>>
        - int Health
        + getHealth() int
        + setHealth(health) void
    }

    class Player 
    Player ..|> Unit : implements
    Player ..|> Controllable : implements
    Player ..|> Renderable : implements


    class NPC {
        <<Interface>>
    }
    NPC ..|> Positional : implements
    
    class Monster 
    Monster ..|> Unit : implements
    Monster ..|> NPC : implements
    Monster ..|> Renderable : implements


    class Item 
    Item ..|> Positional : implements
    Item ..|> Renderable : implements
```
