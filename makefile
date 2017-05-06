comp: codeGen.o treePrint.o parser.o scanner.o main.o
	g++ codeGen.o treePrint.o parser.o scanner.o main.o -o comp

codeGen.o: codeGen.cpp
	g++ -c codeGen.cpp

treePrint.o: treePrint.cpp
	g++ -c treePrint.cpp

scanner.o: scanner.cpp
	g++ -c scanner.cpp

parser.o: parser.cpp
	g++ -c parser.cpp

main.o: main.cpp
	g++ -c main.cpp

clean:
	rm -rf *.o comp

cleanTemp:
	rm -rf temp.4280E01

cleanTarget:	
	rm -rf *.asm

