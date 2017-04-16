
layout: true
class: left, middle
---
background-image: url(graphics/MCCWA.png)
background-size: contain
---

# Modern C++ for the C-Wielding Aspirant
## Resource Management
Instructor: Josh Lospinoso

---

# Resources
* *Resources*: files, heap-allocated memory, sockets, registry entries
* *Resource Allocation Is Initialization*: the One True Path
* *Exceptions*: for when things go wrong
* *Copy and move*: lvalue, rvalue, `move` and `copy` constructors
* *Local scope*: object lifecycle guarantees
* *Unique pointer*: singular ownership
* *Shared pointer*: shared ownership

---
# Resources

Resources are things that your program interacts with that must be _acquired_ and subsequently _released_.

Some common examples:

* Files
* Heap-allocated memory
* Network sockets
* Registry entries
* Mutexes

Failure to _release_ resources is called a _resource leak_, and they are bad.
---
# Managing resources in C is a pain in the ass

C is a relatively low-level language, and managing resources is tedious.

Coping strategies include:

* Nested `if` statements
* `goto` cleanup
* `do`-`while(0)`-`break`
* appending 'pp' to the source's filename

---
# Nested `if` statements
```cpp
file = ifstream("REAMDE.txt");
if(file.is_open()) {
	memory = malloc(...);
	if(memory != NULL) {
		// ...
	}
}
```
---
# `goto` cleanup
```cpp
file = ifstream("REAMDE.txt");
if(!file.is_open()) {
	goto cleanup;
}

memory = malloc(...);
if(memory == NULL) {
	goto cleanup;
}

cleanup:
// ...
```
---
# `do`-`while(0)`-`break`
```cpp
do {
	file = ifstream("REAMDE.txt");
  if(!file.is_open()) {
    break;
  }

  memory = malloc(...);
  if(memory == NULL) {
    break;
  }
} while(0);
```
---
# Resources, the object lifecycle, and RAII

C++ makes strong guarantees about when objects get destructed.

By tying a class to a resource, you can make strong guarantees about when a resource is acquired/released.

Resource acquisition is initialization (of this class). This is the RAII paradigm.

---
```cpp
struct File {
  File(const string &file_name, const string &mode) {
    raw_file = fopen(file_name.c_str(), mode.c_str());
  }
  ~File() {
    fclose(raw_file);
  }
  void write(const string &input) {
    fprintf(raw_file, "%s", input.c_str());
  }
private:
  FILE *raw_file;
};
```
---
# Exceptions

Exceptions are a powerful C++ feature. When something goes wrong in your program, you can _throw an exception_.

```cpp
file = CreateFile(...);
if(file == INVALID_HANDLE_VALUE) {
	throw std::exception("Couldn't create file.");
}
```

This causes your program to unwind the stack until an _exception handler_ is found:

```cpp
try {
	// Do something that might throw an exception
} catch (std::exception e) {
	// Handle the exception gracefully
}
```

*All objects on the stack between the `throw` and the `try`-`catch` are destructed!*

*Don't throw in a destructor!*

---
```cpp
struct File {
  File(const string &file_name, const string &mode) {
    raw_file = fopen(file_name.c_str(), mode.c_str());
	if(nullptr == raw_file) {
		throw exception("Could not open file.");
	}
  }
  ~File() {
    fclose(raw_file);
  }
  void write(const string &input) {
    fprintf(raw_file, "%s", input.c_str());
  }
private:
  FILE *raw_file;
};
```
---
# When to use exceptions

Deciding when to throw (and when to catch) exceptions is a bit of an art.

Some rules of thumb:
* Do use exceptions when it would be tedious to return an error status, or if different kinds of errors
will be handled in different stack frames (i.e. parts of the call chain).
* Do use exceptions when a critical error occurs (e.g. out of memory) and your program is probably going to bail.
* Do NOT use exceptions to control execution flow in healthy program states. They are not
a drop-in replacement for `if`-`then`.


---
# Moving and Copying

Now that we understand how important object lifecycle is, let's talk about how to
manage these lifecycles throughout a program.

There are three general categories of _constructors_:

* Default/parameterized constructors
* Copy constructors
* Move constructors

---
# Default/parameterized constructors

Up to now, this is the kind of constructor we've been dealing with.

These constructors create new objects (with resources, think _newly acquired resource_).

```cpp
# Default constructor
Foo a();

# Parameterized constructor
Foo b("bar");
```
---
# Copy constructors

Copy constructors make copies of objects. Generally this includes their members variables.

You must be careful copy constructing objects that are tied to resources!

```cpp
Foo a();

# Copy constructor
Foo b(a);

# Copy assignment constructor
auto c = a;
```
---
# Move constructors

Move constructors take the values from the given parameter. Generally, this includes any
resources the class is responsible for managing.

Move constructors are like zombies. They eat the brains of the moved-from object.

```cpp
Foo a();

# Move constructor
Foo b1(Foo("bas"));
Foo b2(move(a));

# Move assignment constructor
auto c1 = move(a);
auto c2 = Foo("bas");
```

_Note: Moved-from objects are in an undefined, yet destructable state. Do not reuse them unless
you know what you're doing!_

---
# A gentle peek at lvalues and rvalues

Let's have a look at what `Foo`s constructor signatures look like:

```cpp
struct Foo {
	// Default/parameterized constructors
	Foo();
	Foo(const string &a);

	// Copy constructors
	Foo(const Foo& other);
	Foo& operator=(const Foo& other);

	// Move constructors
	Foo(Foo&& other);
	Foo& operator=(Foo&& other);
}
```

I know. "What in the h-e-double-ampersands is &&?"
---

# The universal reference `&&`

You can think of universal references as "rvalues". This is not strictly true; when writing templated functions, the distinction is important.

What's an "rvalue"? Well, it's anything that's not an "lvalue".

What's an "lvalue"? Well, it's anything that has a name.

```cpp
auto x = 100;

my_func(x); // x is an lvalue
my_func(200); // 200 is an rvalue
```

_Note: lvalues and rvalues are complicated and they entail a lot of nuance. This is a very coarse introduction._
---
A function declared with a universal reference parameter can only be invoked with an rvalue reference. (_Note: [not entirely true](https://isocpp.org/blog/2012/11/universal-references-in-c11-scott-meyers)_)

You can turn an lvalue into an rvalue with `std::move`, but be careful!

```cpp
void func(Foo &&rvalue);

Foo a(100);
func(a);       // Bad! Won't compile
func(Foo(200)); // Ok
func(move(a));  // Ok
```
---
# The lightbulb moment

Why do we *move* from rvalue references and *copy* from lvalue references?

```cpp
// Copy constructors
Foo(const Foo& other);
Foo& operator=(const Foo& other);

// Move constructors
Foo(Foo&& other);
Foo& operator=(Foo&& other);
```

What happens if you eat the brains of an lvalue?

This one of the most crucial concepts in modern C++.
---
# Local scope and resources

Consider how the `File` manages resource in `example1` vs. `example2`.

```cpp
void func(File &f);
void func(File &&f);

void example1() {
	File x("foo", "w+");
	func(x);
	x.write("Hello, world!");
	// ...
	// File is acquired up here
	// ...
} // file released here

void example2() {
	func(File("foo", "w+")); // file released just before function returns
	// ...
	// File has been released
	// ...
}
```
---
# `unique_ptr`

Using scope to manage resources is an outstanding pattern, and you should use it wherever you can.

But what if you want to heap-allocate an object?

You use a `unique_ptr`:

```cpp
void func(const File &f);

void example() {
	auto my_file = make_unique<File>("hello.txt", "w+");
	func(*my_file);
}
```

You can treat it just like a pointer, but when it gets destructed, it calls the destructor of the object it owns.

Unique pointers cannot be copied, only moved.

Unique pointers are so-called _smart pointers._
---
# `shared_ptr`

Sometimes you want to share a resource with multiple consumers.

Ideally, you would destruct this _smart pointer_ when all the consumers were done with it.
```cpp
struct Consumer {
	Consumer(shared_ptr<File> f) : f(f) { }
private:
	shared_ptr<File> f;
}

void example() {
	auto my_file = make_shared<File>("hello.txt", "w+");
	Consumer a(my_file);
	Consumer b(my_file);
	Consumer c(my_file);
	// ...
}
```

`shared_ptr` uses the copy constructor, copy assignment, and destructor to keep track of references.

---

> First we crawl. Later we crawl on broken glass.
> > Scott Meyers

```cpp
std::bind(demo, "raii", 7);


Matterbot Challenge
==
Implement an MD5 calculator command. I'll give you a big head start, don't worry!
```
