# Design guideline
## Struct/Class general
- Struct: POD (Plain Old Data) -> no functions
- Name private member variables: name_
- OOP if there is 1; DOD if there are more
- For DOD (Data oriented design): Class/Object owns component-container (SparseSet for individual access)
- For OOP: Object owns (an individual) component
    - Prefer NMNF function [Nmsp::f(C& o)] over member functions [o.f()], if no need for private access / public API (interface) is sufficient

## Class members (variables) and methods (member functions)
**NOTE**: Encapsulation (:= How much has to change if implementation changes)
- Default: `private`
    - If read access needed (trivial and non-trivial): `const` getter
    - If non-trivial write access needed: setter
    - If external data needed: NMNF (preferably in namespace)
- If non-const access or trivial write access needed: `public`

#### **NOTE**: 
At first it might seem that `public` access ties clients to implementation/concrete type, while non-const reference getters (NCRG) do not -> leads to more flexible/maintainable and increased encapsulation. 

*But if conversion is added to NCRG, it can't return a reference anymore!!! Additionally, `public` provides implicit const-correctness, which NCRG does not.*


# Concepts
## SoA
- := Struct of Arrays (of (entity-)members)
<!-- - If access other than by index needed: Add an additional mapping-member Id(entifier)->Index 
 - Decide if duplication for mapping, or support O(n) lookup of values -->
- Add utility methods to modify containers. Also eg. for accessing individual members and/or returning a single entity struct
- Transient (= Intermediate/Temporary/Transformation) data `struct`:
    - Forward declared `unique_ptr` member in `SoA.h`
    - Define it (and its member structs) in `SoA.cpp`

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

