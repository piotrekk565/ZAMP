

__start__: obj __lines_for_space__ ../../libs/libInterp4Set.so 
	@echo "  Tworzenie wtyczki zakonczone."
	@echo

obj:
	mkdir obj

CPPFLAGS=-Wall -fPIC -pedantic -std=c++17 -Iinc -I../../inc
LDFLAGS=-Wall -shared

__lines_for_space__:
	@echo
	@echo
	@echo
	@echo
	@echo

../../libs/libInterp4Set.so: obj/Interp4Set.o
	g++ ${LDFLAGS} -o ../../libs/libInterp4Set.so obj/Interp4Set.o
	@echo
	@echo "  Wtyczka dla polecenia 'Set' zostala utworzona."
	@echo


obj/Interp4Set.o: src/Interp4Set.cpp ../../inc/AbstractInterp4Command.hh\
                   ../../inc/AbstractScene.hh ../../inc/AbstractComChannel.hh\
                   inc/Interp4Set.hh 
	g++ -c ${CPPFLAGS} -o obj/Interp4Set.o src/Interp4Set.cpp


clean:
	rm -f obj/*

cleanall: clean
	find . -name \*~ -print -exec rm {} \;
