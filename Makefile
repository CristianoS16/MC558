############################################################################################
# Originaly written by:	 Welverton Rodrigues da Silva					  				
# Last modified by:	 Elisa Dell'Arriva at 10/22/21										
############################################################################################


###############################################################################################
######################### Compilação com suporte a múltiplos arquivos #########################
###############################################################################################

.PHONY: build clean

LAB = exe
OBJ = obj

GFLAGS  = -ansi -pedantic -Wall -lm 

build: *.cpp $(OBJ)  #$(OBJECTS)
	c++ $(GFLAGS) $(OBJ) -o $(LAB) -lm

$(OBJ): *.cpp
	c++ $(GFLAGS) -g -c $< -o $@

clean:
	rm -f *.o $(OBJ)

reset: clean
	rm -f $(LAB)


#############################################################################
####################### Suporte para testar o programa ######################
#############################################################################

.PHONY: testar 

testar:
	@set -e ; \
	if [ ! -d testes ] ; then \
		echo "\033[1;31mDiretório 'testes' não encontrado!\033[0m" ; \
		exit 1 ; \
	fi ; \
	for entrada in testes/*.in ; do \
		echo "\033[1;34mTestando \033[4;34m$$entrada\033[0m" ; \
		saida=$$(echo "$$entrada" | sed -re 's/\.in$$/.out/' ) ; \
		resposta=$$(echo "$$entrada" | sed -re 's/\.in$$/.res/' ) ; \
		diff=$$(echo "$$entrada" | sed -re 's/\.in$$/.dif/' ) ; \
		./$(LAB) < "$$entrada" > "$$saida" ; \
		if diff "$$resposta" "$$saida" > "$$diff" ;  then \
			echo "\033[1;32mResultado correto!\033[0m" ; \
		else \
			echo "\033[1;31mFalhou!\033[0m" ; \
			echo ""; \
		fi ; \
		echo ; \
	done
