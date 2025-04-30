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
SOURCE_FILES = $(wildcard ./cccproject/src/ccc/*.cpp) \
               $(wildcard ./cccproject/src/util/*.cpp)

# Set object files
ifeq ($(OS_TYPE), windows)
OBJECT_FILES = $(patsubst ./cccproject/src/%.cpp,./cccproject/build/obj/%.obj,$(SOURCE_FILES))
else ifeq ($(OS_TYPE), linux)
OBJECT_FILES = $(patsubst ./cccproject/src/%.cpp,./cccproject/build/obj/%.o,$(SOURCE_FILES))
endif


# Link
ifeq ($(OS_TYPE), windows)
./build/lib/libcccproject.lib: $(OBJECT_FILES)
else ifeq ($(OS_TYPE), linux)
./build/lib/libcccproject.a: $(OBJECT_FILES)
endif
	mkdir -p ./build/lib
ifeq ($(OS_TYPE), windows)
	ar rcs ./build/lib/libcccproject.lib $(OBJECT_FILES)
else ifeq ($(OS_TYPE), linux)
	ar rcs ./build/lib/libcccproject.a $(OBJECT_FILES)
endif

# Compile
ifeq ($(OS_TYPE), windows)
./cccproject/build/obj/%.obj: ./cccproject/src/%.cpp
else ifeq ($(OS_TYPE), linux)
./cccproject/build/obj/%.o: ./cccproject/src/%.cpp
endif
	mkdir -p $(dir $@)
	$(COMPILER) -c $< -o $@  -I./cccproject/inc $(COMPILE_FLAGS)

clean:
	rm -rf ./build/lib/cccproject.a
	rm -rf ./cccproject/build/obj