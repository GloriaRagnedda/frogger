all: compile linking
	@rm *.o
	@chmod +x Frogger
	@echo "Esegui!"
	
compile:
	@echo "Sto compilando" 
	@gcc -c source/main.c -o main.o
	@gcc -c source/sfondo.c -o sfondo.o
	@gcc -c source/piante.c -o piante.o
	@gcc -c source/rana.c -o rana.o
	@gcc -c source/coccodrilli.c -o coccodrilli.o
	@gcc -c source/frogger.c -o frogger.o
	@gcc -c source/menu.c -o menu.o
	@gcc -w -c source/variabili_globali.c -o variabili_globali.o



linking:
	@echo "Sto Linkando"
	@gcc *.o -lncurses -lpthread -o Frogger