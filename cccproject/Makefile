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

SOURCE_FILES = $(wildcard ./cccproject/src/ccc/*.cpp)
OBJECT_FILES = $(SOURCE_FILES:./cccproject/src/ccc/%.cpp=./cccproject/build/obj/%.o)

./build/lib/cccproject.a: $(OBJECT_FILES)
	mkdir -p ./build/lib
	ar rcs ./build/lib/cccproject.a $(OBJECT_FILES)
	
./cccproject/build/obj/%.o: ./cccproject/src/ccc/%.cpp
	mkdir -p ./cccproject/build/obj
	g++ -c $< -o $@  -I./cccproject/inc $(COMPILE_FLAGS)

clean:
	rm -rf ./build/lib/cccproject.a
	rm -rf ./cccproject/build/obj