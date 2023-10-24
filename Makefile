__start__: obj __lines_for_space__ interp __plugins__
	export LD_LIBRARY_PATH="./libs"; ./interp

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

CPPFLAGS=-Wall -pedantic -std=c++17 -Iinc
LDFLAGS=-Wall




interp: obj/main.o
	g++ ${LDFLAGS} -o interp  obj/main.o -ldl

obj/main.o: src/main.cpp inc/AbstractInterp4Command.hh inc/AbstractScene.hh\
            inc/AbstractComChannel.hh
	g++ -c ${CPPFLAGS} -o obj/main.o src/main.cpp

doc:
	cd dox; make

clean:
	rm -f obj/* interp core*


clean_plugins:
	cd plugins/move; make clean
	cd plugins/set; make clean

cleanall: clean
	cd plugins/move; make cleanall
	cd plugins/set; make cleanall

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
 
