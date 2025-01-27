# Design guideline

> - Simple is better than complex.
> - Complex is better than complicated.
> - Flat is better than nested.
> - Sparse (data) is better than dense.
> - Readability counts.
> - Practicality beats purity.
> - Now is better than never.

## Links
[Noel's blog](https://gamesfromwithin.com/category/c)

## Functions parameters 
- Input last (can have default value, needs to be rightmost in this case)
    `(out >) io > in` 
- According to the above (see below example why to avoid ptr parameters for non-sideeffect like parameters)
    `(&/* >) const& > v `

### Example: Fake-pure, but copy-less/performant and clear intention:
```cpp
// only use for non-returned, but modified (side effect) parameters
[[nodiscard]] S const& f(S* ioPtr)
// or to avoid ptr->ref conversion
[[nodiscard]] S const& f(S& ioRef)
{
    // Forces &-operator usage if ptr is further passed to additional functions -> tramp data
    S& ref(*ptr); 

    ++ref.m;
    return m;
}
```

## Struct/Class general
- Struct: POD (Plain Old Data) -> no functions
- Name private member variables: name_
- OOP/AoS if there is 1; DOD/SoA if there are more
- Prefer NMNF function [Nmsp::f(C& o)] over member functions [o.f()], if no need for private access / public API (interface) is sufficient

## Class members (variables) and methods (member functions)
- Use struct as default
- class with private members 
**NOTE**: Encapsulation (:= How much has to change if implementation changes) -> Low level parameters
- `public/struct` = default (KISS)
- `private/class` + non-trivial getter/setter= to handle invariants or restrict to (controlled) access
**NOTE**: refactor `public` -> getter/setter later if actually needed.*

## Example
```cpp
// Module.h
class C 
{ // has_invariant ? class : struct
public:
    // independent members
public:
    // interface/functions for invariants
// private: 
    // internal helper functions belong to .cpp 
private:
    // invariant members
};

namespace CModule
{
    // functions for C only modification
}

// System.h
namespace SSystem
{
    // functions for multi-class modification
    // -> use multiple modules
}

```

# Concepts
## SoA
- := Struct of Arrays (of (entity-)members)
<!-- - If access other than by index needed: Add an additional mapping-member Id(entifier)->Index 
 - Decide if duplication for mapping, or require O(n) lookup of values -->
- Add utility methods to modify containers. Also eg. for accessing individual members and/or returning a single entity struct
- Transient (= Intermediate/Temporary/TransMoveation) data `struct`:
    - Forward declared `unique_ptr` member in `SoA.h`
    - Define it (and its member structs) in `SoA.cpp`

## SparseSet
- Maps _integer_ `id` to `idx` and `idx` to `data` (array to array) (potentially BIG!)

## DenseMap 
- Maps _arbitrary_ `id` to `idx` and `idx` to `data` (hashmap to array)

## Functor for existing function
```cpp
void func(int arg)
{
  std::cout << "Do something with arg " << arg << "\n";
}

struct fFunctor
{
  void operator()(int arg) {func(arg);}

  fFunctor() = default;
  fFunctor(int arg, bool ctorTrigger = false)
  {
    if (ctorTrigger)
    {
      std::cout << "Trigger in ctor: ";
      func(arg);
    }
  }
};
```

