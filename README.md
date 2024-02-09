# ROGUELIKE
development of a (maybe) roguelike game with c++ and raylib

# REQUIREMENTS
- raylib
- emscripten

# TODOs
# active
[0] Catch up to backup branch

# Shortterm


# Longterm 


# Continuous


# Notes
Maptypes:
- Cellular Automata (dungeon)
- Rogue-like (rooms and tunnels)
- Square rooms grid
- Extend side -> new room wall
- Random border match

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