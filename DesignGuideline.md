# Design guideline
## Struct/Class general
- Struct: POD (Plain Old Data) -> no functions
- Only private member variables: name_
- OOP if there is 1; DOD if there are more
- For DOD (Data oriented design): Class/Object owns component-container (SparseSet for individual access)
- For OOP: Object owns (an individual) component
    - Prefer NMNF function [Nmsp::f(C& o)] over member functions [o.f()]
    - Choose closest scope parameter that contains all information needed (member > object)

## Functions
- namespace: domain/related functions (interclass, non-class)
- methods: member if private member needed (class internal behaviour), free function (NMNF aka. non-member non-friend) if no private member needed (can work with existing interface). -> This should lead to the most concise interface (as small as possible, as big as needed)
- When creating a member function: can it make use of a more "atomic" member function and therefore be made NMNF?
- If for readability only (split long functions without need/probability of reuse): no new file, but forward declare


## Refactor Composition instead of Inheritance
- Refactor with (Interface) Composition when another version of the same type (:interface) is needed:

## General
- Prefer reference over pointer wherever possible

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

## Handles for SoA (https://youtu.be/QbffGSgsCcQ?si=5xdlcdhjIdP-QKVQ&t=1080)
```cpp
struct SoA
{
    static inline std::vector<int> members;
};

struct SoAHandle
{
    size_t index;
    int& id() { return SoA::members[index]; }
};

namespace SoAUtility
{
    SoAHandle create(int member)
    {
        SoA::members.push_back(member);
        return SoAHandle{SoA::members.size() - 1};
    };
}
```
