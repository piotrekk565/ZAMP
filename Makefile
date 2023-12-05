__start__: obj __lines_for_space__ interp __plugins__
	export LD_LIBRARY_PATH="./libs"; ./interp "opis_dzialan.cmd" "config/config.xml"

obj:
	mkdir obj


__lines_for_space__:
	@echo
	@echo
	@echo
	@echo
	@echo


__plugins__:
	cd plugins/move; make
	cd plugins/set; make
	cd plugins/pause; make
	cd plugins/rotate; make

CPPFLAGS=-Wall -pedantic -std=c++17 -Iinc
LDFLAGS=-Wall




interp: obj/config.o obj/klient.o obj/xmlinterp.o obj/main.o
	g++ ${LDFLAGS} -o interp obj/config.o obj/klient.o obj/xmlinterp.o obj/main.o -lxerces-c -ldl

obj/klient.o: src/klient.cpp inc/Klient.hh inc/Port.hh
	g++ -c ${CPPFLAGS} -o obj/klient.o src/klient.cpp

obj/config.o: src/config.cpp inc/Config.hh inc/Vector3D.hh
	g++ -c ${CPPFLAGS} -o obj/config.o src/config.cpp

obj/xmlinterp.o: src/xmlinterp.cpp inc/xmlinterp.hh inc/Config.hh
	g++ -c ${CPPFLAGS} -o obj/xmlinterp.o src/xmlinterp.cpp

obj/main.o: src/main.cpp inc/xmlinterp.hh inc/Config.hh inc/Commands.hh inc/AbstractInterp4Command.hh inc/Klient.hh inc/Scene.hh inc/Port.hh inc/AbstractScene.hh\
            inc/AbstractComChannel.hh
	g++ -c ${CPPFLAGS} -o obj/main.o src/main.cpp

doc:
	cd dox; make

clean:
	rm -f obj/* interp core*


clean_plugins:
	cd plugins/move; make clean
	cd plugins/set; make clean
	cd plugins/pause; make clean
	cd plugins/rotate; make clean

cleanall: clean
	cd plugins/move; make cleanall
	cd plugins/set; make cleanall
	cd plugins/pause; make cleanall
	cd plugins/rotate; make cleanall

	cd dox; make cleanall
	rm -f libs/*
	find . -name \*~ -print -exec rm {} \;

help:
	@echo
	@echo "  Lista podcelow dla polecenia make"
	@echo 
	@echo "        - (wywolanie bez specyfikacji celu) wymusza"
	@echo "          kompilacje i uruchomienie programu."
	@echo "  clean    - usuwa produkty kompilacji oraz program"
	@echo "  clean_plugins - usuwa pluginy"
	@echo "  cleanall - wykonuje wszystkie operacje dla podcelu clean oraz clean_plugins"
	@echo "             oprocz tego usuwa wszystkie kopie (pliki, ktorych nazwa "
	@echo "             konczy sie znakiem ~)."
	@echo "  help  - wyswietla niniejszy komunikat"
	@echo
	@echo " Przykladowe wywolania dla poszczegolnych wariantow. "
	@echo "  make           # kompilacja i uruchomienie programu."
	@echo "  make clean     # usuwa produkty kompilacji."
	@echo
 
