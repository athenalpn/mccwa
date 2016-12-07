#include "catch.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <numeric>

using namespace std;

// Demo 1
TEST_CASE("std::cout displays output", "[stl]") {
	cout << "Hello, world #1" << endl;
}

TEST_CASE("std::cin takes input", "[stl]") {
	string name;
	cout << "What is your name?" << endl;
	// cin >> name; // Hangs up the test when uncommented
	cout << "Thanks, " << name << endl;
}

TEST_CASE("string has length", "[stl]") {
	string advice("Don't panic");
	string empty_string;

	auto string_length = advice.length();

	REQUIRE(string_length == 11);
	REQUIRE(empty_string.length() == 0);
}

TEST_CASE("can substring", "[stl]") {
	string advice("Don't panic");

	auto substring = advice.substr(6, 5);
	auto substring2 = advice.substr(6);

	REQUIRE(substring == "panic");
	REQUIRE(substring2 == "panic");
}

TEST_CASE("can find", "[stl]") {
	string advice("Don't panic");

	auto apostrophe_position = advice.find('\'');
	auto z_position = advice.find('z');

	REQUIRE(apostrophe_position == 3);
	REQUIRE(z_position == string::npos);
}

TEST_CASE("can append", "[stl]") {
	string advice("Don't panic");

	advice.append("***").append("!!!!");

	REQUIRE(advice == "Don't panic***!!!!");
}

TEST_CASE("can access elements with operator[] and at", "[stl]") {
	string advice("Don't panic");
	REQUIRE('D' == advice[0]);
	REQUIRE('t' == advice[4]);
	REQUIRE('D' == advice.at(0));
	REQUIRE('t' == advice.at(4));
}

char first_char(const char *x) {
	return x[0];
}

TEST_CASE("string c_str allows c-style function calls", "[stl]") {
	string advice("Don't panic");

	auto result = first_char(advice.c_str());

	REQUIRE('D' == result);
}

TEST_CASE("string c_str has null termination", "[stl]") {
	string advice("Don't panic");

	auto last_char = advice.c_str()[11];

	REQUIRE('\0' == last_char);
}

TEST_CASE("string data allows c-style function calls", "[stl]") {
	string advice("Don't panic");

	auto result = first_char(advice.data());

	REQUIRE('D' == result);
}

TEST_CASE("access with operator[] and at", "[stl]") {
	vector<int> primes{ 0, 0, 3, 5, 7, 11, 13 };
	
	primes[0] = 1;
	primes.at(1) = 2;

	REQUIRE(primes[0] == 1);
	REQUIRE(primes.at(1) == 2);
}

TEST_CASE("vector front gets first element", "[stl]") {
	vector<int> primes{ 0, 2, 3, 5, 7, 11, 13 };

	primes.front() = 1;

	REQUIRE(primes.front() == 1);
}

TEST_CASE("vector back gets last element", "[stl]") {
	vector<int> primes{ 1, 2, 3, 5, 7, 11, 0 };

	primes.back() = 13;

	REQUIRE(primes.back() == 13);
}

int first_int(const int *x) {
	return x[0];
}

TEST_CASE("vector data allows c-style function calls", "[stl]") {
	vector<int> primes{ 1, 2, 3, 5, 7, 11, 13 };

	auto result = first_int(primes.data());

	REQUIRE(result == 1);
}

TEST_CASE("vector emplace_back appends element", "[stl]") {
	vector<int> primes{ 1, 2, 3, 5, 7, 11, 13 };

	primes.emplace_back(17);

	REQUIRE(primes.size() == 8);
	REQUIRE(primes.back() == 17);
}

TEST_CASE("vector empty returns true when contains no elements", "[stl]") {
	vector<int> primes;

	auto is_empty = primes.empty();

	REQUIRE(is_empty);
}

TEST_CASE("vector clear removes all elements", "[stl]") {
	vector<int> primes{ 1, 2, 3, 5, 7, 11, 13 };

	primes.clear();

	REQUIRE(primes.empty());
}

TEST_CASE("map can emplace", "[stl]") {
	map<string, string> commander;

	commander.emplace("William Adama", "Galactica");

	REQUIRE(commander.at("William Adama") == "Galactica");
}

TEST_CASE("map can erase", "[stl]") {
	map<string, string> commander;
	commander.emplace("William Adama", "Galactica");
	commander.erase("William Adama");

	REQUIRE(commander.empty());
}

TEST_CASE("map can be accessed with operator[] and at", "[stl]") {
	map<string, string> commander;

	commander.emplace("William Adama", "Galactica");

	REQUIRE(commander.at("William Adama") == "Galactica");
	REQUIRE(commander["William Adama"] == "Galactica");
}

TEST_CASE("Iterator example; assign, increment, retrieve", "[stl]") {
	vector<int> primes{ 0, 2, 3, 5, 7, 11, 13 };
	auto prime_iterator = primes.begin();
	*prime_iterator = 1;
	++prime_iterator;
	REQUIRE(primes[0] == 1);
	REQUIRE(*prime_iterator == 2);
}

// Demo 2
TEST_CASE("Can use iterators to loop over elements", "[stl]") {
	vector<string> commanders {"Zaphod Beeblebrox", "Jean-Luc Picard", "Han Solo"};
	auto characters = 0;

	auto iterator = commanders.begin();
	while (iterator != commanders.end()) {
		characters += iterator->size();
		++iterator;
	}

	REQUIRE(characters == 40);
}


TEST_CASE("vector can use range-based for to loop over elements", "[stl]") {
	vector<int> primes{ 1, 2, 3, 5, 7, 11, 13 };
	auto sum_of_primes = 0;

	for (const auto& element : primes) {
		sum_of_primes += element;
	}

	REQUIRE(sum_of_primes == 42);
}


TEST_CASE("map can use range-based for to find elements", "[stl]") {
	map<string, string> commander;
	bool found_galactica = false, found_enterprise = false;
	commander.emplace("William Adama", "Galactica");
	commander.emplace("James Kirk", "Enterprise");

	for (const auto& element : commander) {
		if (element.first == "William Adama" && element.second == "Galactica") {
			found_galactica = true;
		}
		if (element.first == "James Kirk" && element.second == "Enterprise") {
			found_enterprise = true;
		}
	}

	REQUIRE(found_galactica);
	REQUIRE(found_enterprise);
}


TEST_CASE("Can assign to iterators", "[stl]") {
	vector<string> commanders {"XXXXXXXXXX"};

	auto iterator = commanders.begin();
	*iterator = "Malcom Reynolds";

	REQUIRE(commanders[0] == "Malcom Reynolds");
}

TEST_CASE("Begin and end are equal when container empty", "[stl]") {
	vector<string> commanders;

	REQUIRE(commanders.begin() == commanders.end());
}

TEST_CASE("Can add to iterators", "[stl]") {
	vector<string> commanders {"Turanga Leela", "David Bowman"};
	auto iterator = commanders.begin();
	iterator++;

	auto result = *iterator;

	REQUIRE(result == "David Bowman");
}

TEST_CASE("Can subtract from iterators", "[stl]") {
	vector<string> commanders {"Turanga Leela", "David Bowman"};
	auto iterator = commanders.end();
	iterator--;

	auto result = *iterator;

	REQUIRE(result == "David Bowman");
}

TEST_CASE("remove shifts elements", "[stl]") {
	vector<int> primes{ 1, 2, 3, 5, 7, 11, 13 };

	auto new_end = std::remove(primes.begin(), primes.end(), 5);

	REQUIRE(primes == vector<int>({ 1, 2, 3, 7, 11, 13, 13 }));
}

TEST_CASE("Accumulate adds elements", "[stl]") {
	vector<int> original {1, 2, 3};

	auto sum = accumulate(original.begin(), original.end(), 0);

	REQUIRE(sum == 6);
}

TEST_CASE("remove returns new end", "[stl]") {
	vector<int> primes{ 1, 2, 3, 5, 7, 11, 13 };

	auto new_end = remove(primes.begin(), primes.end(), 5);

	REQUIRE(primes.end() == new_end + 1);
}

TEST_CASE("erase-remove paradigm", "[stl]") {
	vector<int> primes{ 1, 2, 3, 5, 7, 11, 13 };

	primes.erase(remove(primes.begin(), primes.end(), 5), primes.end());

	REQUIRE(primes == vector<int>({ 1, 2, 3, 7, 11, 13 }));
}

TEST_CASE("accumulate computes sum", "[stl]") {
	vector<int> original{ 1, 2, 3 };

	auto sum = accumulate(original.begin(), original.end(), 0);

	REQUIRE(sum == 6);
}

TEST_CASE("iota fills a vector", "[stl]") {
	vector<int> numbers(10);

	iota(numbers.begin(), numbers.end(), 0);

	REQUIRE(numbers == vector<int>({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }));
}

TEST_CASE("can replace numbers in a vector", "[stl]") {
	vector<int> numbers(6);
	iota(numbers.begin(), numbers.end(), 0);
	numbers[1] = 100;
	numbers[3] = 100;
	numbers[5] = 100;

	replace(numbers.begin(), numbers.end(), 100, 250);

	REQUIRE(numbers == vector<int>({ 0, 250, 2, 250, 4, 250 }));
}

TEST_CASE("can sort", "[stl]") {
	vector<int> numbers({ 2, 8, 5, 6, 1, 1, 0 });
	sort(numbers.begin(), numbers.end());
	REQUIRE(numbers == vector<int>({ 0, 1, 1, 2, 5, 6, 8 }));
}

TEST_CASE("can copy", "stdl")
{
	vector<int> source{ 1, 2, 3, 4 }, destination(4);

	copy(source.begin(), source.end(), destination.begin());

	REQUIRE(destination == vector<int>({ 1, 2, 3, 4 }));
}

TEST_CASE("can copy into back_inserter", "[stl]") {
	vector<int> source{ 1, 2, 3, 4 };
	vector<int> destination;

	copy(source.begin(), source.end(), back_inserter(destination));

	REQUIRE(destination == vector<int>({ 1, 2, 3, 4 }));
}

TEST_CASE("can transform a vector", "[stl]") {
	vector<int> numbers({4,0,2});
	vector<string> output;

	transform(numbers.begin(), numbers.end(), back_inserter(output),
		[](auto len){
		  return string(len, 'a');
		}
	);

	REQUIRE(output == vector<string>({ "aaaa", "", "aa"}));
}


TEST_CASE("can transform a vector, with lambda capture", "[stl]") {
	vector<int> numbers({ 4,0,2 });
	vector<string> output;
	char my_char = 'b';

	transform(numbers.begin(), numbers.end(), back_inserter(output),
		[my_char](auto len) {
		return string(len, my_char);
	}
	);

	REQUIRE(output == vector<string>({ "bbbb", "", "bb" }));
}


TEST_CASE("can find_if", "[stl]") {
	vector<int> numbers({2, 4, 5, 6});

	auto first_odd = find_if(numbers.begin(), numbers.end(),
		[](auto num){
		  return num % 2 == 1;
		}
	);

	REQUIRE(first_odd == numbers.begin() + 2);
}
