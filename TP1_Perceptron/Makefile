GPP=g++
GCC=gcc
FLAGS=  -g2 -Wall -O0 -I../Class -I../Utils  -D_DEBUG 
LIBS=  
OBJS=Debug/main.o Debug/GNUPlot.o Debug/Neurona.o Debug/Red.o Debug/utils.o Debug/Config.o 

all: Debug Debug/TP1_Perceptron.bin

clean:
	rm -rf ${OBJS} Debug/TP1_Perceptron.bin

Debug/TP1_Perceptron.bin: ${OBJS}
	${GPP} ${OBJS} ${LIBS} -o $@

Debug:
	mkdir Debug

Debug/main.o: main.cpp ../Class/Neurona.h ../Utils/utils.h ../Class/GNUPlot.h ../Class/Config.h
	${GPP} ${FLAGS} -c main.cpp -o $@

Debug/GNUPlot.o: ../Class/GNUPlot.cpp ../Class/GNUPlot.h
	${GPP} ${FLAGS} -c ../Class/GNUPlot.cpp -o $@

Debug/Neurona.o: ../Class/Neurona.cpp ../Class/Neurona.h ../Utils/utils.h
	${GPP} ${FLAGS} -c ../Class/Neurona.cpp -o $@

Debug/Red.o: ../Class/Red.cpp ../Class/Red.h ../Class/Neurona.h ../Utils/utils.h
	${GPP} ${FLAGS} -c ../Class/Red.cpp -o $@

Debug/utils.o: ../Utils/utils.cpp ../Utils/utils.h
	${GPP} ${FLAGS} -c ../Utils/utils.cpp -o $@

Debug/Config.o: ../Class/Config.cpp ../Class/Config.h
	${GPP} ${FLAGS} -c ../Class/Config.cpp -o $@

