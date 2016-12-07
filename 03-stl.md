layout: true
class: left, middle
---
background-image: url(graphics/MCCWA.png)
background-size: contain
---

# Modern C++ for the C-Wielding Aspirant
## The Standard Template Library
Josh Lospinoso

---

# The Standard Template Library
* *Strings*: `length`, `substr`, `find`, `append`, `operator[]`, `data`, `c_str`,
* *Streams*: `cin`, `cout`, `ofstream`, `ifstream`
* *Templates*: using generic types with STL
* *Vectors*: `operator[]`, `front`, `back`, `data`, `emplace_back`, `empty`, `clear`
* *Maps*: `emplace`, `operator[]`, `at`, `erase`,
* *Iterators*: range-based `for` loops, flavors, `begin`, `end`, `back_inserter`
* *Algorithms*: `erase`-`remove` idiom, `accumulate`, `iota`, `find`, `copy`, `transform`, `replace`, `sort`
* *Lambdas*: using anonymous functions

---

# Don't Be Fancy
The STL maintainers are probably better at C++ than you.

When programming something, ask whether there's
an STL concept you can use instead.

If you need a data structure, there's probably a container already written.

If you are performing a common algorithmic task, there's probably already
something written (e.g. counting, searching, replacing, finding).
---

# Why use the STL?
If you don't, you will probably:

* make mistakes,
* generate less efficient code,
* waste your time, and
* waste others' time.

---

# What's going on here?
```cpp
int g( int a[], int l, int r) {
   int pivot, i, j, t;
   pivot = a[l];
   i = l; j = r+1;
   while(true) {
   	do ++i; while( a[i] <= pivot && i <= r );
   	do --j; while( a[j] > pivot );
   	if( i >= j ) break;
   	t = a[i]; a[i] = a[j]; a[j] = t;
   }
   t = a[l]; a[l] = a[j]; a[j] = t;
   return j;
}
```
---
_(continued)_
```cpp
void f( int a[], int l, int r) {
   int j;
   if( l < r ) {
    j = g( a, l, r);
    f( a, l, j-1);
    f( a, j+1, r);
   }
}

int a[] = {2, 8, 5, 6, 1, 1, 0};
f(a);
```
---
# How about here?

```cpp
vector<int> a({2, 8, 5, 6, 1, 1, 0});
sort(a.begin(), a.end());
```
---

If you use `std::sort`, everyone knows what's going on.

Hand-jamming common algorithms means less transparency, more errors,
inefficiency, and wasted programmer time.

---
# STL Strings
The `std::string` class is a safe alternative to `char*` for string data.

You should use it wherever you can!

* safer
* more convenient
* easy to tackle common tasks
* memory is managed for you

```cpp
string my_name("Josh");
my_name += " Lospinoso";
auto name_length = my_name.length();
auto space_position = my_name.find(" ");
auto last_name = my_name.substr(space_position + 1);
// ...
```
---

# A subset of `string` methods

---
# `length`
`length` returns the length of the string in bytes

```cpp
string advice("Don't panic");
auto string_length = advice.length();
REQUIRE(string_length == 11);
```
---
# `substr`
`substr` returns a newly constructed string object initialized to a copy of a
substring of the original string.

```cpp
string advice("Don't panic");
auto substring = advice.substr(6, 5);
REQUIRE(substring == "panic");
```
---
# `find`
`find` returns the position of the first match. If no match is found, `string::npos` is returned.

```cpp
string advice("Don't panic");
auto apostrophe_position = advice.find('\'');
REQUIRE(apostrophe_position == 3);
```
---
# `append`
`append` adds the given characters to the end of a string.

```cpp
string advice("Don't panic");
advice.append("!");
REQUIRE(advice == "Don't panic!");
```
---
# `operator[]`
You can retrieve elements of a string with brackets:

If the index is out of bounds, you get undefined behavior.

Use `.at(int index)` for a bounds-checked version

```cpp
string advice("Don't panic");
REQUIRE('D' == advice[0]);
REQUIRE('t' == advice[4]);
REQUIRE('D' == advice.at(0));
REQUIRE('t' == advice.at(4));
```
---
# `data` and `c_str`
Interacting with C-style APIs is no excuse to use old-style C strings. You can access the underlying
`char` array with `data` or `c_str`:

```cpp
char crufty_old_function(const char *x, int x_len);
// ...
string advice("Don't panic");
auto result = crufty_old_function(advice.c_str(), advice.length());
```

Note that `c_str()` is *null terminated* while `data()` is not.

---

> Fifty years of programming language research and we end up with C++?
> > Richard O'Keefe

```cpp
bind(demo, "stl", 4);
```

---
# STL Streams

Streams provide a clean, serial interface for input and output (I/O) to arbitrary objects.

Examples of arbitrary objects:
* devices
* the console
* files
* C++ objects

Streams provide an abstraction over I/O with these objects.
---
# `cin` / `cout`: interacting with the console

Standard input/standard output are synchronized with `cin`/`cout`.

Like other streams, you interact with `cin` and `cout` in the following way:

```cpp
string name;
cout << "What is your name?" << endl;
cin >> name;
cout << "Thanks, " << name << endl;
```

In the context of strings, these operators DO NOT mean bitwise left/right shift.

The stream object will be on the left-hand side of the operator.

---
# `ofstream`: file output

Files can be written to using streams.

1. Create the `ofstream` with the filename.
2. Check that the file was successfully open with `is_open`
3. Stream data into the `ofstream`
4. Optional: clean up with `close`

```cpp
ofstream file("advice.txt");
if (file.is_open()) {
  file << "Don't panic!";
  file.close();
} else {
  cout << "Unable to open file.";
}
```
---
# `ifstream`: file input

Files can also be read from using streams.

1. Create the `ifstream` with the filename.
2. Check that the file was successfully open with `is_open`
3. Stream data from the `ifstream`
4. Optional: clean up with `close`

```cpp
string line;
ifstream file("advice.txt");
if (file.is_open()) {
  while (getline(file, line)) {
    cout << line << '\n';
  }
  file.close();
} else {
  cout << "Unable to open file.";
}
```
---
# Templates

Templates are how C++ programmers perform generic programming.

Generic programming, or _programming against generic types_, means writing
data structures and algorithms without regard for the _specific types_ on which
these structures and algorithms are built.

```cpp
// Tedious and brittle:
int DataStructureOfInts::next();
string DataStructureOfStrings::next();
Foo DataStructureOfFoos::next();

// Awesome:
T DataStructure<T>::next();
```
---
Templates are extremely powerful, and they can get extremely complicated. They are one of the most
important tools in a library writer's kit.

Fortunately, you are not library writers (yet!), so we can surf right over all of these complications.

_Using_ templated code is far easier than _writing_ templated code. Just put the type you want to use in angle brackets!

---
# STL `vector`
The `vector` class is a smart, resizable array. You can think of them as generically-typed strings.

```cpp
vector<int> primes{ 1, 2, 3, 5, 7, 11 };
primes.emplace_back(13);
int sum_of_primes = 0;
for(const auto& element : primes) {
  sum_of_primes += element;
}
REQUIRE(sum_of_primes == 42);
```
---
> It is the bestest data structure.
> > Stephan T. Lavavej (on `std::vector`)
---
# `operator[]`

You can access elements using the bracket operator `[]`.

If the index is out of bounds, you get undefined behavior.

Use `.at(int index)` for a bounds-checked version

```cpp
vector<int> primes{ 0, 0, 3, 5, 7, 11, 13 };
primes[0] = 1;
primes.at(1) = 2;
REQUIRE(primes[0] == 1);
REQUIRE(primes.at(1) == 2);
```

Note that you cannot assign to `const` vector elements.

---
# `front`

Retrieve the first element in a container

```cpp
vector<int> primes{ 0, 2, 3, 5, 7, 11, 13 };
primes.front() = 1;
REQUIRE(primes.front() == 1);
```
`front` returns a reference, so you can assign to it (as long as the vector is not `const`).

Behavior is undefined if the `vector` is empty.
---
# `back`

Retrieve the last element in a container

```cpp
vector<int> primes{ 1, 2, 3, 5, 7, 11, 0 };
primes.back() = 13;
REQUIRE(primes.back() == 13);
```
`back` returns a reference, so you can assign to it (as long as the vector is not `const`).

Like `front`, behavior is undefined if the `vector` is empty.
---
# `data`

As in `string`, `vector` is backed by a contiguous array in memory.

You can access this underlying array with `.data()`.

You should only need this when interacting with C functions.

```cpp
void do_stuff(const int *x, int size_of_x);
vector<int> primes{ 1, 2, 3, 5, 7, 11, 13 };
do_stuff(primes.data(), primes.size());
```
---
# `emplace_back`

Add an element to the end of the vector

A new element is constructed _in place_ at the end of the vector using the arguments to `emplace_back`.

```cpp
vector<int> primes{ 1, 2, 3, 5, 7, 11, 13 };
primes.emplace_back(17);
REQUIRE(primes.size() == 8);
REQUIRE(primes.back() == 17);
```

_Note_: A similar method, `push_back` exists that will _copy_ or _move_ the argument rather than
construct a new one. This distinction is important, and we will revisit it later.

---
# `empty`

Returns true if the vector contains no elements.

```cpp
vector<int> primes;
REQUIRE(primes.empty());
```
---

# `clear`

Removes all elements. Note that the removed elements are destroyed.

```cpp
vector<int> primes{ 1, 2, 3, 5, 7, 11, 13 };
primes.clear();
REQUIRE(primes.empty());
```
---
# STL `map`

The STL `map` is an associative container (i.e. a key-value store).

```cpp
map<string, string> commander;
bool found_galactica = false;
commander.emplace("William Adama", "Galactica");
commander.emplace("James Kirk", "Enterprise");
for(const auto& element : commander) {
	if (element.first == "William Adama" && element.second == "Galactica") {
		found_galactica = true;
	}
}
REQUIRE(found_galactica);
```
---
# `emplace`

Insert an element into the map. As with `vector`, `emplace` constructs the new entry in-place for lower overhead.

```cpp
commander.emplace("William Adama", "Galactica");
```

`insert` is a similar function which will involve move/copy operations to insert the new entry.
---
# `operator[]` and `at`

Element access for `map`s is very similar to that of `vectors`

You can access elements using `operator[]`.

If the index is out of bounds, you get undefined behavior.

Use `.at(K index)` for a bounds-checked version.

```cpp
map<string, string> commander;
commander.emplace("William Adama", "Galactica");
REQUIRE(commander.at("William Adama") == "Galactica");
REQUIRE(commander["William Adama"] == "Galactica");
```

Note that you cannot assign to `const` map elements.


---
# `erase`

Erase the given element. The entry with the given key will be removed from the `map`.

```cpp
map<string, string> commander;
commander.emplace("William Adama", "Galactica");
commander.erase("William Adama");
REQUIRE(commander.empty());
```

_note_: `empty()` returns `true` if the `map` has no elements.

---

> There are two ways of constructing a software design. One way is to make it so simple that there are obviously no deficiencies. And the other way is to make it so complicated that there are no obvious deficiencies.
> > Tony Hoare

```cpp
bind(demo, "stl", 5);
```

---
# STL Iterators

Iterators are fundamental building blocks of the STL. They connect *containers* to *algorithms*.

All STL containers have `begin` and `end` methods that return iterators.

Think of them as civilized versions of pointers.

```cpp
vector<int> primes{ 0, 2, 3, 5, 7, 11, 13 };
auto prime_iterator = primes.begin();
*prime_iterator = 1;
++prime_iterator;
REQUIRE(primes[0] == 1
);
REQUIRE(*prime_iterator == 2);
```
---
# range-based for loops

For any object that defines `.begin()` and `.end()` functions to obtain iterators (e.g. `vector` and `map`),
there is a very clean language construct for looping over that object's elements:

```cpp
vector<int> primes{ 1, 2, 3, 5, 7, 11, 13 };
int sum_of_primes = 0;
for(const auto& element : primes) {
	sum_of_primes += element;
}
REQUIRE(sum_of_primes == 42);
```

_note_: This loop is equivalent to

```cpp
for(auto iter = primes.begin(); iter != primes.end(); ++iter) {
	const auto &element = *iter;
	// ...
}
```

---

# Aside: STL Iterator Flavors

Category          | Ability                        | Example Providers
------------------|--------------------------------|-----------------------------:
Input         | R forward                  | istream
Output        | W forward                  | ostream, back_inserter
Forward       | R/W forward                | forward_list
Bidirectional | R/W forward/backward       | list, set, map
RandomAccess  | R/W & random access        | vector, string

You'll see _bidirectional_ (from `map`), _random access_ (from `vector` and `string`), and _output_ iterators (from `back_inserter`).

We won't sweat the details here, just keep it on your radar for the future.

---
# STL Algorithms

The STL Algorithms are your best friend. If you need to search, sort, manipulate, etc. a range
of elements, there's probably something here for you.

Algorithms are built upon STL Iterators, so if you have a data range that you can represent as iterators,
this beautiful toolkit is available to you.

All STL Containers expose iterators though their `begin` and `end` methods. Different algorithms
have varied requirements on the iterators they accept. Some, like `find_if` require only an `InputIterator`.
Others, like `sort` require the more featureful `RandomAccessIterator`.

---
# `remove`

Removes all specified elements by (a) pushing them to the end of the range and (b) returning
the new end.

```cpp
vector<int> primes{ 1, 2, 3, 5, 7, 11, 13 };
auto new_end = remove(primes.begin(), primes.end(), 5);
REQUIRE(primes == vector<int>({ 1, 2, 3, 7, 11, 13, 13 }));
REQUIRE(primes.end() == new_end + 1);
```

_Note_: Requires a `ForwardIterator`.
---
# `accumulate`

Returns the result of summing the range. The third argument is the value that the sum begins with.

```cpp
vector<int> original {1, 2, 3};
auto sum = accumulate(original.begin(), original.end(), 0);
REQUIRE(sum == 6);
```

_Note_: Requires a `InputIterator`.
---
# `erase`-`remove` idiom

`erase` is part of the underlying container, but it is presented again here as part of the
`erase`-`remove` idiom. All of the `remove`d elements are destructed and the container is resized.

```cpp
vector<int> primes{ 1, 2, 3, 5, 7, 11, 13 };
primes.erase(remove(primes.begin(), primes.end(), 5), primes.end());
REQUIRE(primes == vector<int>({ 1, 2, 3, 7, 11, 13 }));
```

_Note_: Requires a `ForwardIterator`.
---
# `iota`

Assigns successive values, starting with third argument, to each element in the range.

```cpp
vector<int> numbers(10);
iota(numbers.begin(), numbers.end(), 0);
REQUIRE(numbers == vector<int>({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }));
```

_Note_: Requires a `ForwardIterator`
---
# `replace`

Assigns elements matching the _third_ argument to the new value given as the _fourth_ argument.

```cpp
vector<int> numbers(6);
iota(numbers.begin(), numbers.end(), 0);
numbers[1] = 100;
numbers[3] = 100;
numbers[5] = 100;
replace(numbers.begin(), numbers.end(), 100, 250);
REQUIRE(numbers == vector<int>({ 0, 250, 2, 250, 4, 250 }));
```

_Note_: Requires a `ForwardIterator`
---
# `sort`

Sort elements in the range into ascending orders.

```cpp
vector<int> numbers({2, 8, 5, 6, 1, 1, 0});
sort(numbers.begin(), numbers.end());
REQUIRE(numbers == vector<int>({0, 1, 1, 2, 5, 6, 8}));
```

_Note_: Requires a `RandomAccessIterator`
---

# Lambdas
Lambda functions are unnamed functions that can capture variables within
their scope.

---

A lambda with no capture:
```cpp
auto fn1 = [](int x){
    return x % 42;
  };
REQUIRE(fn1(62) == 20);
```

---

A lambda capturing by _value_:
```cpp
auto divisor = 42;
auto fn2 = [=divisor](int x){
    return x % divisor;
  };
divisor = 1; // Irrelevant
REQUIRE(fn2(62) == 20);
```

---

A lambda capturing by _reference_:
```cpp
auto divisor = 42;
auto fn3 = [&divisor](int x){
    return x % divisor;
  };
divisor = 62; // Relevant
REQUIRE(fn1(62) == 0);
```
---
# `back_inserter`

Back insert iterators are special _OutputIterators_ that allow algorithms
to insert new elements to the end of a container.

Think of these like _adapters_.

We'll demonstrate `back_inserter` with a bonus algorithm...
---

# `copy`

Copies the elements in the range given by the first two arguments
into the range beginning at the third argument.

```cpp
vector<int> source{1, 2, 3, 4}, destination(4);
copy(source.begin(), source.end(), destination.begin());
REQUIRE(destination == vector<int>({ 1, 2, 3, 4 }));
```

Dangerous? With a `back_inserter`:
```cpp
vector<int> source{ 1, 2, 3, 4 }, destination;
copy(source.begin(), source.end(), back_inserter(destination));
REQUIRE(destination == vector<int>({ 1, 2, 3, 4 }));
```
---
# `transform`

Applies a function to the range given by the first two arguments
and assigns the result to the range started by the third argument.

```cpp
vector<int> numbers({4,0,2});
vector<string> output;
transform(numbers.begin(), numbers.end(), back_inserter(output),
	[](auto len){
	  return string(len, 'a');
	}
);
REQUIRE(output == vector<string>({ "aaaa", "", "aa"}));
```
_Note_: Requires both an `InputIterator` and an `OutputIterator`

---

# `find_if`

Returns an iterator to the first element in the given range for which
the given function returns true.

```cpp
vector<int> numbers({2, 4, 5, 6});
auto first_odd = find_if(numbers.begin(), numbers.end(),
	[](auto num){
	  return num % 2 == 1;
	}
);
REQUIRE(first_odd == numbers.begin() + 2);
```
_Note_: Requires an `InputIterator`
---

> The purpose of computation is insight, not numbers.
> > Richard Hamming

```cpp
bind(demo, "stl", 6);
```
