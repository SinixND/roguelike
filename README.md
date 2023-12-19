# ROGUELIKE
development of a (maybe) roguelike game with c++ and raylib

# REQUIREMENTS
- raylib
- emscripten

# TODOs
# active
[5] Render sprites/textures
    - check managers for entities, or entities for components, or on component change notify systems

# Shortterm
[0] make ECS
[4] split globals and settings

# Longterm 

# Continuous
[0] plan structure with UML

# Notes
Id signature{0}; 
to bit: bit(Id){return 1 << n}

For Bits:
compare      bit(x) & bit(a)
compare n    bit(x) & ( bit(a) | bit(n) )
set          bit(x) |= bit(a)
set n        bit(x) |= ( bit(a) | bit(n) )
unset        bit(x) &= ~bit(a) 
unset n      bit(x) &= ~( bit(a) | bit(n) )

## Variadic function template:
### Distinct types:
template <typename Type, typename... VariadicTypes>
void doSomething(Type firstArg, VariadicTypes... variadicArgs) // recursive variadic function
{
    // Handle first argument
    cout << firstArg << endl;
 
    // Handle remaining (variadic) arguments
    doSomething(variadicArgs...);
}

### Similar types:
template <class Type>
void func2( std::initializer_list<Type> arguments )
{
    for( auto argument : arguments )
    {
        std::cout << argument << std::endl ;
    }
}

# Done
[x] compile for linux & windows !
