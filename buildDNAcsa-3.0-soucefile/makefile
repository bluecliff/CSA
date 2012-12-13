#compile buildcsa.cpp, build the buildcsa 
buildDNAcsa : buildDNAcsa.o csa.o succintCode.o rank_select.o unaryCode.o compactIntArray.o
	g++ -o buildDNAcsa buildcsa.o csa.o succintCode.o rank_select.o unaryCode.o compactIntArray.o
buildDNAcsa.o : buildDNAcsa.cpp def.h csa.h
	g++ -c buildDNAcsa.cpp
csa.o : csa.h csa.cpp def.h ./rank_select/rank_select.h ./succintCode/succintCode.h
	g++ -c csa.cpp
succintCode.o : ./succintCode/succintCode.cpp ./succintCode/succintCode.h ./compactIntArray/compactIntArray.h ./unaryCode/unaryCode.h
	g++ -c ./succintCode/succintCode.cpp
rank_select.o : ./rank_select/rank_select.cpp ./rank_select/rank_select.h def.h ./compactIntArray/compactIntArray.h
	g++ -c ./rank_select/rank_select.cpp
unaryCode.o : ./unaryCode/unaryCode.cpp ./unaryCode/unaryCode.h ./compactIntArray/compactIntArray.h 
	g++ -c ./unaryCode/unaryCode.cpp
compactIntArray.o : ./compactIntArray/compactIntArray.cpp ./compactIntArray/compactIntArray.h
	g++ -c ./compactIntArray/compactIntArray.cpp
clean :
	rm  buildcsa.o csa.o succintCode.o rank_select.o unaryCode.o compactIntArray.o
