#include "catch.h"
#include <stdio.h>
#include <memory>

using namespace std;

// Demo 1
TEST_CASE("Can cleanup with gotos", "[raii]") {
	auto file = fopen("tmp.txt", "w");
	if(!file) {
		goto cleanup;
	}
	auto buffer_to_write = (char*)malloc(1000);
	if(!buffer_to_write) {
		goto cleanup;
	}
	memset(buffer_to_write, 0x42, 999);
	buffer_to_write[999] = 0;
	fprintf(file, "%s", buffer_to_write);
cleanup:
	if(buffer_to_write) {
		free(buffer_to_write);
	}
	if(file) { 
		fclose(file);
	}
}

struct File {
  File(const char *file_name, const char *mode) {
    raw_file = fopen(file_name, mode);
  }
  ~File() {
    fclose(raw_file);
  }
  void write(const char *input) const {
    fprintf(raw_file, "%s", input);
  }
private:
  FILE *raw_file;
};

TEST_CASE("Can use RAII to manage objects", "[raii]") {
	File file("tmp.txt", "w");
	string buffer_to_write(1000, 'a');
	buffer_to_write[999] = 0;
	file.write(buffer_to_write.c_str());
}

struct Resource {
	void acquire() {
		if(is_acquired) {
			throw exception("Tried to acquire and already acquired.");
		}
		is_acquired = true;
	}
	void release() {
		if(!is_acquired) {
			throw exception("Tried to release and not acquired.");
		}
		is_acquired = false;
	}
	bool is_acquired = false;
};

struct RaiiTest {
	RaiiTest(Resource &resource)
		: resource(resource){
		resource.acquire();
	}
	~RaiiTest() {
		resource.release();
	}	
	Resource &resource;
};

TEST_CASE("RAII works within a local scope", "[raii]") {
	Resource my_resource;
	REQUIRE(!my_resource.is_acquired);
	{
		RaiiTest test(my_resource);
		REQUIRE(my_resource.is_acquired);
	}
	REQUIRE(!my_resource.is_acquired);
}

TEST_CASE("RAII works with unique_ptr", "[raii]") {
	Resource my_resource;
	REQUIRE(!my_resource.is_acquired);
	{
		auto raii_ptr = make_unique<RaiiTest>(my_resource);
		REQUIRE(my_resource.is_acquired);
	}
	REQUIRE(!my_resource.is_acquired);
}

TEST_CASE("Unique_ptrs can be swapped", "[raii]") {
	unique_ptr<RaiiTest> other_ptr;
	REQUIRE(other_ptr == nullptr);
	
	Resource my_resource;
	REQUIRE(!my_resource.is_acquired);
	{
		auto raii_ptr = make_unique<RaiiTest>(my_resource);
		REQUIRE(my_resource.is_acquired);
		raii_ptr.swap(other_ptr);
	}
	// Still acquired; other_ptr owns resource
	REQUIRE(my_resource.is_acquired);
}

TEST_CASE("Shared_ptrs can be copied", "[raii]") {
	Resource my_resource;
	auto shared_ptr = make_shared<RaiiTest>(my_resource);
	REQUIRE(my_resource.is_acquired);
	{
		// Copy construction
		auto other_ptr = shared_ptr;
		REQUIRE(my_resource.is_acquired);
	}
	// Still acquired
	REQUIRE(my_resource.is_acquired);
}
