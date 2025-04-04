./build/lib/cccmain.a: ./cccmain/src/cccmain.cpp
	mkdir -p ./build/lib
	g++ ./cccmain/src/cccmain.cpp -o ./build/lib/cccmain.a -c -I./cccproject/inc $(COMPILE_FLAGS)

clean:
	rm -rf ./build/lib/cccmain.a