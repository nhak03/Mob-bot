COMPILE = g++ -std=c++17 -Wall -c
LINK = g++ -std=c++17 -Wall -o
REMOVE = rm -f
MEMCHECK = valgrind --leak-check=full

# Main target
bot: mybot.o action_work.o commons.o Dictionary.o 
	$(LINK) bot mybot.o action_work.o commons.o Dictionary.o -ldpp

# Compilation rule for Dictionary.cpp
Dictionary.o: Dictionary.cpp Dictionary.h
	$(COMPILE) Dictionary.cpp

# Comiliation for commons.cpp
commons.o: commons.cpp commons.h Dictionary.h
	$(COMPILE) commons.cpp

# Compilation for action_work.cpp
action_work.o: action_work.cpp action_work.h commons.h Dictionary.h
	$(COMPILE) action_work.cpp

# Compilation rule for mybot.cpp
mybot.o: mybot.cpp
	$(COMPILE) mybot.cpp

# Cleanup rule
clean:
	$(REMOVE) bot mybot.o Dictionary.o commons.o action_work.o