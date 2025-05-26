# Obtain OS type
ifeq ($(shell uname), Linux)
    OS_TYPE = linux
else ifeq ($(shell uname), Darwin)
    OS_TYPE = macos
else ifeq ($(OS), Windows_NT)
    OS_TYPE = windows
else
    OS_TYPE = unknown
endif

# Obtain source files
SOURCE_FILES = $(wildcard ./cccunit/src/ccc/*.cpp) \
               $(wildcard ./cccunit/src/util/*.cpp)

# Set object files
ifeq ($(OS_TYPE), windows)
OBJECT_FILES = $(patsubst ./cccunit/src/%.cpp,./cccunit/build/obj/%.obj,$(SOURCE_FILES))
else ifeq ($(OS_TYPE), linux)
OBJECT_FILES = $(patsubst ./cccunit/src/%.cpp,./cccunit/build/obj/%.o,$(SOURCE_FILES))
endif


# Link
ifeq ($(OS_TYPE), windows)
./build/lib/cccunit.dll: $(OBJECT_FILES)
else ifeq ($(OS_TYPE), linux)
./build/lib/libcccunit.so: $(OBJECT_FILES)
endif
	mkdir -p ./build/lib
ifeq ($(OS_TYPE), windows)
	g++ -shared -o ./build/lib/cccunit.dll $(OBJECT_FILES)
else ifeq ($(OS_TYPE), linux)
	g++ -shared -o ./build/lib/libcccunit.so $(OBJECT_FILES)
endif

# Compile
ifeq ($(OS_TYPE), windows)
./cccunit/build/obj/%.obj: ./cccunit/src/%.cpp
else ifeq ($(OS_TYPE), linux)
./cccunit/build/obj/%.o: ./cccunit/src/%.cpp
endif
	mkdir -p $(dir $@)
	$(COMPILER) -c $< -o $@  -I./cccunit/inc $(COMPILE_FLAGS) -fPIC

clean:
	rm -rf ./build/lib/cccunit.a
	rm -rf ./cccunit/build/obj