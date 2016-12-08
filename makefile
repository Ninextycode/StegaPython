headers = sha512.h TempBasedStegonography.h

sources = VectorSubroutines.cpp Filer.cpp Sha512.cpp\
    HighLevelCrypto.cpp LowLevelCrypto.cpp\
    HighLevelStego.cpp LowLevelStego.cpp\
    sha512.cpp main.cpp  

objects = VectorSubroutines.o Filer.o Sha512.o\
    HighLevelCrypto.o LowLevelCrypto.o\
    HighLevelStego.o LowLevelStego.o\
    sha512.o main.o  

CC = g++ -std=c++14 -fpic -g
LINK_FLAGS = -shared -lboost_python -lboost_unit_test_framework
output = stega.so
exec = stega

exec: $(objects)
	$(CC) -o $(exec) $(objects) 


module: $(objects)
	$(CC) $(LINK_FLAGS) -o $(output) $(objects) 


Sha512.o: Sha512.cpp TempBasedStegonography.h
	$(CC) -c Sha512.cpp

VectorSubroutines.o: VectorSubroutines.cpp TempBasedStegonography.h
	$(CC) -c VectorSubroutines.cpp

Filer.o: Filer.cpp TempBasedStegonography.h
	$(CC) -c Filer.cpp
	
HighLevelStego.o: HighLevelStego.cpp TempBasedStegonography.h
	$(CC) -c HighLevelStego.cpp
	
HighLevelCrypto.o: HighLevelCrypto.cpp TempBasedStegonography.h
	$(CC) -c HighLevelCrypto.cpp
	
LowLevelStego.o: LowLevelStego.cpp TempBasedStegonography.h
	$(CC) -c LowLevelStego.cpp

LowLevelCrypto.o: LowLevelCrypto.cpp TempBasedStegonography.h
	$(CC) -c LowLevelCrypto.cpp

sha512.o: sha512.cpp sha512.h
	$(CC) -c sha512.cpp

	
main.o: main.cpp TempBasedStegonography.h
	$(CC) -c -I/usr/include/python3.4 main.cpp
	

	
clean:
	rm -f $(objects) $(output) 
