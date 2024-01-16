# ROGUELIKE
development of a (maybe) roguelike game with c++ and raylib

# REQUIREMENTS
- raylib
- emscripten

# TODOs
# active
[0] Rename Utility functions to "convert..." and move to Systems.h

[0] Split files (esp. game folder)

[1] Order of rendering


# Shortterm
[1] Input system assigns components on key/mouse controlled

[1.2] Add collision system

[2] Map generation


# Longterm 

# Continuous

# Notes
Maptypes:
- Cellular Automata (dungeon)
- Rogue-like (rooms and tunnels)
- Square rooms grid
- Extend side -> new room wall
- Random border match


## Variadic function template:
### Distinct types:
{
    template <typename Type, typename... VariadicTypes>
    void doSomething(Type firstArg, VariadicTypes... variadicArgs) // Recursive variadic function
    {
        // Handle first argument
        cout << firstArg << endl;
    
        // Handle remaining (variadic) arguments
        doSomething(variadicArgs...);
    }
}

### Similar types:
{
    template <class Type>
    void func2( std::initializer_list<Type> arguments )
    {
        for( auto argument : arguments )
        {
            std::cout << argument << std::endl ;
        }
    }
}

# Done
[x] compile for linux & windows !

[x] Render sprites/textures

[x] make ECS

[x] realize viewport system (render relative to window -> update Rendersystem: pos 0,0 & transform 0, 0 == center of screen)

[x] adjust texture to default direction right
