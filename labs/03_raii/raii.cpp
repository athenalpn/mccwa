#include "catch.h"
#include <stdio.h>
#include <memory>

using namespace std;

// Demo 7
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
    File(const string &file_name, const string &mode) {
        raw_file = fopen(file_name.c_str(), mode.c_str());
        if (nullptr == raw_file) {
            throw exception("Could not open file.");
        }
    }
    ~File() {
        if (raw_file != nullptr) {
            fclose(raw_file);
        }
    }
    // disable copy ctor and copy assignment ctor
    File(const File&) = delete;
    File& operator=(const File&) = delete;
    // move ctor and move assignment ctor
    File(File &&f) {
        raw_file = f.raw_file;
        f.raw_file = nullptr;
    }
    File& operator=(File &&f) {
        if (raw_file != nullptr) {
            fclose(raw_file);
        }
        raw_file = f.raw_file;
        f.raw_file = nullptr;
    }
    void write(const string &input) {
        fprintf(raw_file, "%s", input.c_str());
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

TEST_CASE("copy ctors are disabled", "[raii]") {
    File file("tmp.txt", "w");

    // These won't compile:
    //File copy1(file);      // b/c copy ctor was deleted
    //File copy2 = file;     // b/c copy assignment ctor was deleted
}

TEST_CASE("move ctors work", "[raii]") {
    File file("tmp.txt", "w");

    // move constructor
    File move1(File("tmp_move1.txt", "w"));
    File move2(std::move(file));

    // move assignment constructor
    File file2("tmp_move2.txt", "w");
    auto move3 = File("tmp_move3.txt", "w");
    auto move4 = std::move(file2);
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

void remove_files() {
    remove("tmp.txt");
    remove("tmp_move1.txt");
    remove("tmp_move2.txt");
    remove("tmp_move3.txt");
}

TEST_CASE("cleanup the files we created", "[raii]") {
    remove_files();
}
