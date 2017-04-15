#include "catch.h"

// Demo 1
TEST_CASE("auto assigns type of double", "[basics]") {
	// Given
	long double expected = 42.0L;

	// When
	auto result = 42.0L;

	// Then
	REQUIRE(expected == result);
}

TEST_CASE("auto saves you from unintented casts", "[basics]") {

	unsigned int bad = -42;
	auto ok = -42;

	REQUIRE(bad == 4294967254);
	REQUIRE(ok == -42);
}

// Demo 2
TEST_CASE("const variables can be read from", "[basics]") {
	const auto read_only = 42;

	REQUIRE(read_only == 42);
}

struct Point {
	int x, y;
};

void set_to_origin(Point &point) {
	point.x = 0;
	point.y = 0;
}


TEST_CASE("References modify original value", "[basics]") {
	Point point;
	point.x = 100;
	point.y = 200;

	set_to_origin(point);

	REQUIRE(point.x == 0);
	REQUIRE(point.y == 0);
}


void set_to_origin_ptr(Point *point) {
	point->x = 0;
	point->y = 0;
}

TEST_CASE("Pointers modify original value", "[basics]") {
	Point point;
	point.x = 100;
	point.y = 200;

	set_to_origin_ptr(&point);

	REQUIRE(point.x == 0);
	REQUIRE(point.y == 0);
}

void set_to_origin_copy(Point point) {
	point.x = 0;
	point.y = 0;
}


TEST_CASE("Pass by value does not modify original value", "[basics]") {
	Point point;
	point.x = 100;
	point.y = 200;

	set_to_origin_copy(point);

	REQUIRE(point.x == 100);
	REQUIRE(point.y == 200);
}


TEST_CASE("Pointers are dangerous", "[basics]") {
	auto pointer = (Point*)nullptr;

	// Compiles, then blows up in your face
	//set_to_origin_ptr(pointer);
}

double distance_ref(const Point &point) {
	return sqrt(point.x * point.x + point.y * point.y);
}

TEST_CASE("Const references can be read from", "[basics]") {
	Point point;
	point.x = 3;
	point.y = 4;

	auto distance = distance_ref(point);

	REQUIRE(distance == 5);
}

double nonsense_distance_ref(const Point point) {
	return sqrt(point.x * point.x + point.y * point.y);
}

TEST_CASE("Const by value works, and it's dumb", "[basics]") {
	Point point;
	point.x = 3;
	point.y = 4;

	auto distance = nonsense_distance_ref(point);

	REQUIRE(distance == 5);
}

int function_with_default(int x = 100) {
	return x;
}


TEST_CASE("Parameter has default value", "[basics]") {
	auto result = function_with_default();

	REQUIRE(result == 100);
}

TEST_CASE("Parameter value can be specified", "[basics]") {
	auto result = function_with_default(8675309);

	REQUIRE(result == 8675309);
}


// Demo 3
struct CyberBullet {
	double velocity, position;
	double time_to_target_at(double target_position) {
		return (target_position - position) / velocity;
	}
};

TEST_CASE("Member function accesses member variables", "[basics]") {
	CyberBullet bullet;
	bullet.velocity = 100;
	bullet.position = 0;

	auto result = bullet.time_to_target_at(500);

	REQUIRE(result == 5);
}

struct CyberBulletStruct {
private:
	double velocity, position;
};

class CyberBulletClass {
	double velocity, position;
};

TEST_CASE("Cannot access private variables", "[basics]") {
	CyberBulletStruct bullet_struct;
	// bullet_struct.velocity = 100; // Wont Compile
	CyberBulletClass bullet_class;
	// bullet_class.velocity = 100; // Wont Compile
}

bool cyber_tripod_constructed = false, cyber_tripod_destructed = false;

struct CyberTripod {
	CyberTripod() {
		cyber_tripod_constructed = true;
	}
	~CyberTripod() {
		cyber_tripod_destructed = true;
	}
};

struct CyberMachineGun {
	CyberTripod tripod;
};

TEST_CASE("Member variables are also constructed.", "[basics]") {
	cyber_tripod_constructed = false;

	CyberMachineGun gun;

	REQUIRE(cyber_tripod_constructed);
}

TEST_CASE("Member not constructed until declared", "[basics]") {
	cyber_tripod_constructed = false;
	REQUIRE(!cyber_tripod_constructed);

	CyberTripod tripod;
}

TEST_CASE("Member not destructed while in scope", "[basics]") {
	cyber_tripod_destructed = false;

	CyberTripod tripod;

	REQUIRE(!cyber_tripod_destructed);
}

TEST_CASE("Member destructed after falls out of scope", "[basics]") {
	cyber_tripod_destructed = false;
	{
		CyberTripod tripod;
	}
	REQUIRE(cyber_tripod_destructed);
}

TEST_CASE("New calls constructor", "[basics]") {
	cyber_tripod_constructed = false;

	auto tripod = new CyberTripod();
	
	REQUIRE(cyber_tripod_constructed);
	delete tripod;
}

TEST_CASE("Delete calls destructor", "[basics]") {
	cyber_tripod_destructed = false;
	auto tripod = new CyberTripod();

	delete tripod;
	REQUIRE(cyber_tripod_destructed);
}

struct CyberRanger {
	void do_patrol() {
		hungry = true;
	}
	bool is_hungry() const {
		return hungry;
	}
	void feed_mre() {
		hungry = false;
	}
private:
	bool hungry = false;
};

void check_status(const CyberRanger &ranger) {
	auto x = ranger.is_hungry(); // OK
	//ranger.feed_mre(); // Won't compile!
}

void feed_hungry_ranger(CyberRanger &ranger) {
	if (ranger.is_hungry())
		ranger.feed_mre(); // OK, ranger is not const
}

TEST_CASE("CyberRanger gets hungry after patrol.", "[basics]") {
	CyberRanger ranger;

	ranger.do_patrol();

	REQUIRE(ranger.is_hungry());
}

TEST_CASE("Hungry CyberRanger isn't hungry after feeding.", "[basics]") {
	CyberRanger ranger;

	ranger.do_patrol();
	ranger.feed_mre();

	REQUIRE(!ranger.is_hungry());
}

bool info_called = false, error_called = false;

struct Logger {
	virtual ~Logger() { }
	virtual void info(const char message[]) = 0;
	virtual void error(const char message[]) = 0;
};

class TestableLogger : public Logger {
public:
	void info(const char message[]) override {
		info_called = true;
	}
	virtual void error(const char message[]) override {
		error_called = true;
	}
};

TEST_CASE("Logger doesn't log without being called.", "[basics]") {
	info_called = false;
	error_called = false;

	Logger &logger = TestableLogger();
	
	REQUIRE(!error_called);
	REQUIRE(!info_called);
}

TEST_CASE("Logger interface calls error correctly.", "[basics]") {
	info_called = false;
	error_called = false;

	Logger &logger = TestableLogger();

	logger.error("Hello, world.");

	REQUIRE(error_called);
	REQUIRE(!info_called);
}

TEST_CASE("Logger interface calls info correctly.", "[basics]") {
	info_called = false;
	error_called = false;

	TestableLogger tlogger;

	Logger &logger = tlogger;
	Logger *logger2 = &tlogger;

//	Logger &logger3 = nullptr;
	Logger *logger4 = nullptr;

	logger.info("foo");
	logger2->info("foo");

	logger.info("Hello, world.");

	REQUIRE(!error_called);
	REQUIRE(info_called);
}

namespace mccwa {
	int foo;
	class bar { };
	void bas() { }
}

using namespace mccwa;
using mccwa::bar;

void do_stuff() {
	foo = 10;
	bar x;
	mccwa::bas();
}