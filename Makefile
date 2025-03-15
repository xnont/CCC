debug:
	make -f ./cccmain/Makefile COMPILE_FLAGS="-Og -g -std=c++17 -W -Wall"
	make -f ./cccproject/Makefile COMPILE_FLAGS="-Og -g -std=c++17 -W -Wall"

release:
	make -f ./cccmain/Makefile COMPILE_FLAGS="-O2 -std=c++17 -W -Wall"
	make -f ./cccproject/Makefile COMPILE_FLAGS="-O2 -std=c++17 -W -Wall"

clean:
	make -f ./cccmain/Makefile clean
	make -f ./cccproject/Makefile clean