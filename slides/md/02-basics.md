layout: true
class: left, middle
---
background-image: url(graphics/MCCWA.png)
background-size: contain
---

# Modern C++ for the C-Wielding Aspirant
## The Basics
Josh Lospinoso

---

The Basics
==

* *Types and modifiers*: `bool`, `char`, `int`, `double`, `unsigned`, `auto`, `const`
* *Functions and parameters*: qualifiers, `*` pointers, `&` references, default values
* *User types*: `struct`s, `class`es, members, functions, access, inheritance, `virtual`, `override`, `delete`, namespaces
* *The object lifecycle*: constructors and destructors
* *Inheritance*: only what you need to know
* *Namespaces*: keeping your classes straight
* *Headers and source*: what goes where
---

Types
==
You can use all of the C types that you know and love, including:

* (signed) char, unsigned char
* int, unsigned int
* short, unsigned short
* long, unsigned long
* float, double, long double
* void

---

`auto`
==

Usefully, modern C++ provides us with the `auto` keyword.

Consider the following example:

```cpp
long double x = 42.0L;
```

We're actually specifying `x`'s type twice. Using auto, we can specify it once:

```cpp
auto x = 42.0L;
```

---

# `auto` can keep you from blowing a leg off

```cpp
unsigned int bad = -42;
```

`auto` is also tremendously useful during some kinds of refactoring.

---

# Aside: Unit Testing Frameworks

There are many. You should pick one and stick to it. Or use the one that your
team is using.

For this course, we use [catch](https://github.com/philsquared/Catch) because
it is very unobtrusive and easy to use:

```cpp
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

TEST_CASE( "Example", "[catch]" ) {
  auto x = 6 * 7;
  auto y = 42;
  REQUIRE( x == y );
}
```

---

# Aside: Test Style

Try to write each test so that it `REQUIRE`s one thing.

Having lots of tests is much better than having a big, monolithic test.

Use the given-when-then style!

```cpp
TEST_CASE("given-when-then", "[test]") {
  // Given -- conditions of your test
  long double expected = 42.0L;

  // When -- interact with the thing you are testing
  auto result = 42.0L;

  // Then -- make assertions about the result
  REQUIRE(expected == result);
}
```

---

> There are only two kinds of programming languages: those people always bitch about and those nobody uses.
> > Bjarne Stroustrup

```cpp
bind(demo, "basics", 1);
```

---

`const`
==

`const` is another C++ keyword. Roughly, it means that you promise not to change the value. It is `const`ant.

```cpp
// Does not compile -- "You cannot assign to a variable that is const."
const auto x = 100;
x = 200;
```

`const` is very important, and we will revisit it in the functions/classes portions.

---
# Functions

You can always use C-style functions:

```c
int do_stuff(int in_param, char[] array, double *out_param);
```

Major improvements in C++:
* Passing with _references_
* Passing `struct`s/`class`es _by value_
* `const` parameters

---

# References

References are pointers that cannot be null, and you interact with references
without having to use the _dereference_ operator `->`. For example:

```cpp
struct Point {
  int x, y;
};

void set_to_origin(Point &point) {
  point.x = 0;
  point.y = 0;
}
```

Compare with:

```cpp

void set_to_origin_ptr(Point *point) {
  point->x = 0;
  point->y = 0;
}
```

Be there dragons?

---

# Passing by value

You can pass structs (and classes) _by value_:

```cpp
double distance(Point point) {
  point.x *= point.x;
  point.y *= point.y;
  return sqrt(point.x + point.y);
}
```

The caller's `Point` is unaffected because a new one is _copy constructed_ (more on this
later in the course).

---

# Passing `const`

You can pass by reference/pointer/value with or without a `const` qualifier:

```cpp
double distance_ref(const Point &point) {
  return sqrt(point.x * point.x + point.y * point.y);
}
```

Why is the following function definition nonsensical?

```cpp
double calculate_stuff(const Point point);
```

---

# Default argument values

You can make arguments optional by giving them default values:

```cpp
double function_with_default(double x=100) {
  return x;
}
auto a = function_with_default();
auto b = function_with_default(42);
```

---

# Function design guidelines

* Pass by `const` ref when you want to pass in an argument that will not be modified.
* Pass by value when you want a copy that you can modify/save off/etc.
* Never use raw pointers (unless you have to interact with C functions, of course).

_Note:_ consider whether you must operate on `const` arguments e.g. from an external API!

---

> We shall do a much better programming job, provided we approach the task with a full appreciation of its tremendous difficulty, provided that we respect the intrinsic limitations of the human mind and approach the task as very humble programmers.
> > Alan Turing

```cpp
bind(demo, "basics", 2);
```

---

# User types

C++ `class`es/`struct`s ("classes" from now on) can be very different from C `struct`s.

If an object can be *trivially constructable and destructable*, and it contains only
public member variables, they are the same:

```cpp
struct Point {
  int x, y;
};
```

In all but this simple case, classes and C `struct`s are quite different.

---

# Member functions

In C++, classes can (and usually do) have *member functions*.

These are just like
*free functions* you know from C, except they can access all of the classes other
members.

```cpp
struct CyberBullet {
  double velocity, position;
  double time_to_target_at(double target_position) {
    return (target_position - position) / velocity;
  }
};
```

---

# Access specifiers

Sometimes you don't want users of your class to be able to mess with members.

There are several *access specifiers* you can use in these situations:

* `public`: all access pass
* `protected`: only _subclasses_ can access
* `private`: only this class can access; classes marked `friend` can also access

Don't be fancy, just use `public` and `private`.

---

> C++: Where friends have access to your private members.
> > Gavin Baker

---

# `class` vs `struct`

Easy:

* `class` members are private by default.
* `struct` members are public by default.

All of the next three classes are identical.

---

```cpp
struct CyberBulletStruct {
  double time_to_target_at(double target_position);
private:
  double velocity, position;
};

class CyberBulletClass1 {
  double velocity, position;
public:
  double time_to_target_at(double target_position);
};

class CyberBulletClass2 {
public:
  double time_to_target_at(double target_position);
private:
  double velocity, position;
};
```

---

Pick a style and stick to it.

---

# Object Lifecycle

One of the most powerful aspects of C++ is the object lifecycle model.

In C and C++, `CyberBullet` is put on the _stack_ here:

```cpp
CyberBullet a;
```

and on the _heap_ here:

```cpp
// C
CyberBullet *a = (CyberBullet*)malloc(sizeof(CyberBullet));
// C++
auto a = new CyberBullet();
```

In C, all that's happening is memory allocation on the stack or the heap.

In C++, this object gets *constructed* with a *constructor*.

*Note: don't use `new`. But if you do, you must pair it with a `delete`. More on this in the RAII section.*

---
# Constructors
Constructors are special member functions that initialize objects.

```cpp
struct CyberCannon {
  CyberCannon(double angle, double charges) {
	this->angle = angle;
	this->charges = charges;
  }
  void fire();
private:
  double angle, charges;
};
```

The `this` keyword is a _pointer_ used to refer to the *member variable*.

---

# Member objects
Member objects are constructed before an owning object is constructed.

```cpp
#include <stdio.h>

struct CyberTripod {
  CyberTripod() {
    printf("Tripod is set up.");
  }
};

struct CyberMachineGun {
  CyberMachineGun() {
    printf("Machine gun is set up.");
  }
  CyberTripod tripod;
};

main () {
  CyberMachineGun gun;
}
```

To Godbolt's [Compiler Explorer!](https://gcc.godbolt.org/)

---

# Member Initializers
Member initializers allow you to set member fields with a special syntax. You should use these whenever possible.

Consider the following:

```cpp
#include <stdio.h>

struct CyberGrenade {
  CyberGrenade(double charge=0) {
    printf("Grenade charge: %d.", charge);
  }
};

struct CyberGrenadeLauncher {
  CyberGrenadeLauncher() {
    grenade = CyberGrenade(100);
  }
  CyberGrenade grenade;
};

main () {
  CyberGrenadeLauncher launcher;
}
```

How many `CyberGrenades` did we make? To the [Compiler Explorer](https://gcc.godbolt.org/)

---

# Member Initializers

We can eliminate an extra construction using a _member initializer_:

```cpp
struct CyberGrenadeLauncher {
  CyberGrenadeLauncher()
    : grenade(100){
  }
  CyberGrenade grenade;
};
```

---

# List initialization

There are yet more ways to construct objects in C++. If you use curly
braces instead of parenthesis when calling a constructor, you are performing
_list initialization_:

```cpp
CyberGrenade cg1 {100};
CyberGrenade cg2 = {100};
auto cg3 = CyberGrenade{100};
```

These can save you from _narrowing conversions_.

You must be extremely careful when your type accepts `std::initializer_list` as
a constructor parameter! I do not recommend you use list initialization for these
types (e.g. `std::vector`).

---

# Narrowing conversions

```cpp
void fun(double val, int val2) {
    int x2 = val; 	// if val==7.9, x2 becomes 7 (bad)
    char c2 = val2; // if val2==1025, c2 becomes 1 (bad)
	
    int x3 {val}; 	// error: possible truncation (good)
    char c3 {val2}; // error: possible narrowing (good)
    char c4 {24}; 	// OK: 24 can be represented exactly as a char (good)
    char c5 {264}; 	// error (assuming 8-bit chars): 264 cannot be 
					// represented as a char (good)
    int x4 {2.0}; 	// error: no double to int value conversion (good)
}
```

---
# Destructors
Destructors are special member functions that are called when an object's lifetime ends.

Recall: Member objects are constructed before an owning object is constructed. Destructors are applied in the reverse order.

The purpose of the destructor is to release resources that the object is responsible for. C++ destructors are amazing, and you should love them.

Prepare to be amazed.

---


```cpp
#include <stdlib.h>
#include <stdio.h>

struct File {
  File(const char *file_name, const char *mode) {
    raw_file = fopen(file_name, mode);
  }
  ~File() {
    fclose(raw_file);
  }
  void write(const char *input) {
    fprintf(raw_file, "%s", input);
  }
private:
  FILE *raw_file;
};

main() {
  File x("foo", "w+");
  x.write("Hello, world!");
}
```

We manage resources with object lifecycles in C++.

---

# Still not amazed?

Let's deconstruct what's going on.

```cpp
int main() {
  File x("foo", "w+");

  // Do stuff

  if (something_bad_happened)
    return ERROR;

  // Do more stuff

  return SUCCESS;
}
```

`File` never leaks!

---

> Garbage collection is neither necessary nor sufficient for quality software.
> > Bjarne Stroustrup, Herb Sutter, and Gabriel Dos Reis

> If Java had true garbage collection, most programs would delete themselves upon execution.
> > Robert Sewell

We will spend _a lot_ of time on this fundamental concept towards the end of the course.

---
# `const` functions
Functions marked `const` are guaranteed not to modify the object's state:

```cpp
struct CyberRanger {
  bool is_hungry() const {
    return hungry;
  }
  void feed_mre() {
    hungry = false;
  }
private:
  bool hungry;
};
```

The `is_hungry` function doesn't modify a `CyberRanger`'s state, so we mark it `const`.

---
# `const` functions
Remember passing by const reference?

```cpp
void check_status(const CyberRanger &ranger) {
  auto x = ranger.is_hungry(); // OK
  //ranger.feed_mre(); -- Won't compile!
}

void feed_hungry_ranger(CyberRanger &ranger) {
  if(ranger.is_hungry())
	ranger.feed_mre(); // OK, ranger is not const
}
```

The `is_hungry` function doesn't modify a `CyberRanger`'s state, so we mark it `const`.
---

# Inheritance

C++ inheritance can be really, really complicated.

The good news is, you shouldn't worry about most of it. You can always turn fancy C++ inheritance hierarchies into simpler _compositions_.

Consider this *interface*:

```cpp
class Logger {
public:
  virtual ~Logger() { }
  virtual void info(const char message[]) = 0;
  virtual void error(const char message[]) = 0;
};
```

* `virtual` means this method is meant to be overridden by a subclass
* `=0` means there is no default implementation and the inheriting class must implement it.
* _Note:_ This is a special type of _abstract class_ called _pure virtual_.
---

This is an *implementation* of a `Logger`:

```cpp
class ConsoleLogger : public Logger {
public:
  virtual void info(const char message[]) override {
    printf("info: %s", message);
  }
  virtual void error(const char message[]) override {
    printf("error: %s", message);
  }
};
```

---

# Why?

```cpp
void do_stuff_and_log(const Logger &logger) {
  logger.info("Doing stuff");
}

main() {
  ConsoleLogger cl;
  do_stuff_and_log(cl);
  
  // After implementing a FileLogger:
  //FileLogger fl;
  //do_stuff_and_log(fl);
}
```

You can easily change the behavior of this function by implementing a new `Logger`.

---
# Cyber Style Guide

Prefer *composition* over *inheritance*.

```cpp
// Inheritance: This is terrible. Do not do this.
struct CyberMission {};

struct StationaryMission : public CyberMission {};
struct NonstationaryMission : public CyberMission {};

struct CyberRaid : public StationaryMission { };
struct CyberAmbush : public StationaryMission { };
struct CyberRecon : public NonstationaryMission { };

struct LongRangeRaid : public CyberRaid { };
struct Patrol : public StationaryMission, public NonstationaryMission { };
// ...
```

---

> It's like, barf me out. Gag me with a spoon!
> > Moon Unit Zappa

---
# Composition Example

```cpp
struct DumbLogger {
  public info(const char[] x) {
    printf("info: %s", x);
  }
}

struct ConsoleFoo : public DumbLogger {
  void do_stuff_and_log() {
    info("Doing stuff");
  }
}

struct Foo {
  void do_stuff_and_log() {
    log.info("Doing stuff");
  }

  Logger &log;
}
```

---
# Composition Example (continued)

```cpp
main() {
  ConsoleLogger cl;
  FileLogger fl;
  
  Foo f;
  f.log = cl;
  f.do_stuff_and_log();
  
  f.log = fl;
  f.do_stuff_and_log();  
}
```

This makes unit testing easier as well.

---
# namespaces

Namespaces provide scope to the names of types, functions, variables, etc. inside.

```cpp
namespace mccwa {
  int foo;
  class bar;
  void bas();
}
```

You can access these scoped items several ways:

```cpp
using namespace mccwa;
using mccwa::bar;

void do_stuff() {
  foo = 10;
  bar x;
  mccwa::bas();
}
```
---


# Headers and source

All of your knowledge from C translates over to C++:

* Put exported functionality in header files
* `include` imported functionality from other headers
* Put implementation details in _source files_ (here, `.cpp` rather than `.c`)
* Don't put a `using` directive in a header.

---

If you wish to use a type in more than one place, you should put it in a header file.

You _may_ put the entire type definition in your header file, e.g.

```cpp
struct CyberBullet {
  double velocity, position;
  double time_to_target_at(double target_position) {
    return (target_position - position) / velocity;
  }
};
```

---

Or you may decide to split the declaration in the `.h` file,

```cpp
struct CyberBullet {
  double velocity, position;
  double time_to_target_at(double target_position);
};
```

from the implementation in the `.cpp` file:

```cpp
double CyberBullet::time_to_target_at(double target_position) {
  return (target_position - position) / velocity;
}
```

---

I prefer _header only_ implementations for small projects, especially when they are libraries for others to use.

For large projects, this approach can result in increased compile times.

There is no universally correct approach.

---

> We are all shaped by the tools we use, in particular: the formalisms we use shape our thinking habits, for better or for worse, and that means that we have to be very careful in the choice of what we learn and teach, for unlearning is not really possible.
> > Edsger Dijkstra

```cpp
bind(demo, "basics", 3);
```
