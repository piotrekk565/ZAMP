

__start__: obj __lines_for_space__ ../../libs/libInterp4Pause.so 
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

../../libs/libInterp4Pause.so: obj/Interp4Pause.o
	g++ ${LDFLAGS} -o ../../libs/libInterp4Pause.so obj/Interp4Pause.o
	@echo
	@echo "  Wtyczka dla polecenia 'Pause' zostala utworzona."
	@echo


obj/Interp4Pause.o: src/Interp4Pause.cpp ../../inc/AbstractInterp4Command.hh\
                   ../../inc/AbstractScene.hh ../../inc/AbstractComChannel.hh\
                   inc/Interp4Pause.hh 
	g++ -c ${CPPFLAGS} -o obj/Interp4Pause.o src/Interp4Pause.cpp


clean:
	rm -f obj/*

cleanall: clean
	find . -name \*~ -print -exec rm {} \;
