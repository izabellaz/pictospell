COMPILE = g++
LINK = g++ -lncurses
FLAGS = -std=c++11 -ggdb -O3
LFLAGS = 
EXENAME = spellcheck

OBJS = ed.o n-gram.o main.o

$(EXENAME): $(OBJS)
	$(LINK) $(LFLAGS) -o $(EXENAME) $(OBJS)

%.o: %.cpp
	$(COMPILE) $(FLAGS) -c $< -o $@

#main.o: main.cpp
#	$(COMPILE) $(FLAGS) -c main.cpp
#
#ed.o: ed.cpp ed.h 
#	$(COMPILE) $(FLAGS) -c ed.cpp
#
#n-gram.o: n-gram.cpp n-gram.h
#	$(COMPILE) $(FLAGS) -c n-gram.cpp

clean: 
	rm  *.o $(EXENAME)

ed.o: ed.cpp
ed.o: ed.h
n-gram.o: n-gram.cpp
n-gram.o: n-gram.h
main.o: main.cpp
main.o: n-gram.h
main.o: ed.h