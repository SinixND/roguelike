# ROGUELIKE
development of a (maybe) roguelike game with c++ and raylib

# REQUIREMENTS
- raylib
- emscripten

# TODOs
# active
[0] Refactor tilePosition to float position

[1] Implement vision (fog of war: unseen, seen, visible)


# Shortterm
[1] Map generation

[1] Map change


# Longterm 
[0] Indicate select option (different cursor color on hover?)

[0] (Hold) TAB to show map overview

[0] Add touch support


# Continuous


# Notes

## Controls

### Mouse
| Control     | Function                           |
| ----------- | ---------------------------------- |
| RMB         | Toggle cursor control (Mouse/Keys) |
| LMB         | Click to toggle select             |
| Mouse wheel | Zoom                               |


### Keyboard
| Control                           | Function            |
| --------------------------------- | ------------------- |
| WASD / Arrow keys                 | Cursor movement     |
| L_SHIFT                           | Fast cursor movment |
| CTRL + 0 (both numpad and normal) | Reset Zoom          |
| ESCAPE / CAPS_LOCK                | Deselect            |


### Touch
Not implemented yet

## Maptypes:
- Cellular Automata (dungeon)
- Rogue-like (rooms and tunnels)
- Square rooms grid
- Extend side -> new room wall
- Random border match

## Design guide
### Struct/Class general
- Struct: POD 
- Getter / Setter only when needed
- Only private member variables: name_

### Functions
- namespace: domain/related functions (interclass, non-class)
- Class: member if private member needed (class internal behaviour), free if no private member needed (eg. getter/setter)
- For readability only: no new file, but forward declare

### New class:
- Define members
- Check existing classes for communality
    - if yes, extract:
        - "Has-a" (preferred) -> Component
        - "Is-a" -> Parent
    - check present methods (became inter-class / free function?)



## Refactor Composition instead of Inheritance
Refactor with (Interface) Composition when another version of the smae type (:interface) is needed:

1) Old:
```cpp
class ConcreteComponent {
public:
    void reset() {};
    void update(int) {};
};

class Class {
public:
    Class(ConcreteComponent component)
    : component(component) {
        component.reset();
    }

    void process() {
        component.update(21);
    }

private:
    ConcreteComponent component;
};
```
2) New:
```cpp
class IComponentInterface {
public:
    virtual ~IComponent(){};
    virtual void reset() = 0;
    virtual void update(int) = 0;
};

class ConcreteComponent : public IComponentInterface {
public:
    void reset() override {};
    void update(int) override {};
};

class Class {
public:
    Class(IComponentInterface& interface)
    : interface(interface) {
        interface.reset();
    }

    void process() {
        interface.update(21);
    }

private:
    IComponentInterface& interface;
};
```


# Done
[x] compile for linux & windows

[x] Render sprites/textures

[-] make ECS (removed)

[x] realize viewport/camera system

[x] Implement zoom

[x] Check class design (see notes); make diagram to track classes

[-] Command pattern for rendering and layers (not implemented -> need to handle abiguous keys in SparseSet)

[x] Implement movment
