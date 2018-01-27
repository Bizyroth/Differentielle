EXE=table echantillon key_finder dechiffre lineaire
FLAG= -Wall -O2 
all: $(EXE)

table: table.o multi_diff.o heys.o
	gcc -o table table.o multi_diff.o heys.o $(FLAG)

table.o: table.c multi_diff.h
	gcc -c table.c -o table.o $(FLAG)

echantillon: echantillon.o multi_diff.o heys.o
	gcc -o $@ echantillon.o multi_diff.o heys.o $(FLAG)
	
key_finder: key_finder.o multi_diff.o heys.o
	gcc -o $@ key_finder.o multi_diff.o heys.o -pthread $(FLAG)
	
key_finder.o: key_finder.c multi_diff.h
	gcc -o $@ -c key_finder.c -pthread $(FLAG)

dechiffre: dechiffre.o multi_diff.o heys.o
	gcc -o $@ dechiffre.o multi_diff.o heys.o $(FLAG)
	
dechiffre.o: dechiffre.c multi_diff.h
	gcc -c dechiffre.c -o $@ $(FLAG)

lineaire: lineaire.c heys.o
	gcc -o lineaire lineaire.c heys.o $(FLAG) -g

echantillon.o: echantillon.c multi_diff.h
	gcc -c echantillon.c -o $@ $(FLAG)


multi_diff.o: multi_diff.c multi_diff.h heys.h
	gcc -c multi_diff.c -o $@ $(FLAG)
	

heys.o: heys.c heys.h
	gcc -c heys.c -o heys.o

	
clean:
	find . -name '*.o' -exec rm -f {} ';'
	find . -name '*.a' -exec rm -f {} ';'
	find . -name '*.so' -exec rm -f {} ';'
	find . -name '*~' -exec rm -f {} ';'
	find . -name '#*#' -exec rm -f {} ';'
	find . -name '.#*' -exec rm -f {} ';'
	find . -name 'core' -exec rm -f {} ';'
	find . -name '*.core' -exec rm -f {} ';'


mrproper: clean
	rm -f $(EXE)
	rm -f couple couple_chiffre
	
