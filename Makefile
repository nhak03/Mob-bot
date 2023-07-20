COMPILE = g++ -std=c++17 -Wall -c
LINK = g++ -std=c++17 -Wall -o
REMOVE = rm -f
MEMCHECK = valgrind --leak-check=full

# Main target
bot: mybot.o commons.o Dictionary.o 
	$(LINK) bot mybot.o commons.o Dictionary.o -ldpp

# Compilation rule for Dictionary.cpp
Dictionary.o: Dictionary.cpp Dictionary.h
	$(COMPILE) Dictionary.cpp

# Comiliation for commons.cpp
commons.o: commons.cpp commons.h Dictionary.h
	$(COMPILE) commons.cpp

# Compilation rule for mybot.cpp
mybot.o: mybot.cpp
	$(COMPILE) mybot.cpp

# Cleanup rule
clean:
	$(REMOVE) bot mybot.o Dictionary.o commons.o