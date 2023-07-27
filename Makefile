COMPILE = g++ -std=c++17 -Wall -c
LINK = g++ -std=c++17 -Wall -o
REMOVE = rm -f
MEMCHECK = valgrind --leak-check=full

# Main target
bot: a_main.o action_buy.o action_work.o action_gamble.o action_sell.o commons.o Dictionary.o 
# $(LINK) bot mybot.o action_buy.o action_work.o commons.o Dictionary.o -ldpp
	$(LINK) bot obj/* -ldpp 


# Compilation rule for Dictionary.cpp
Dictionary.o: src/Dictionary.cpp src/Dictionary.h
	$(COMPILE) src/Dictionary.cpp -o obj/Dictionary.o

# Comiliation for commons.cpp
commons.o: src/commons.cpp src/commons.h
	$(COMPILE) src/commons.cpp -o obj/commons.o

# Compilation for action_work.cpp
action_work.o: src/action_work.cpp src/action_work.h 
	$(COMPILE) src/action_work.cpp -o obj/action_work.o

# action_gamble.cpp
action_gamble.o: src/action_gamble.cpp src/action_gamble.h
	$(COMPILE) src/action_gamble.cpp -o obj/action_gamble.o

# Compilation for action_buy.cpp
action_buy.o: src/action_buy.cpp src/action_buy.h
	$(COMPILE) src/action_buy.cpp -o obj/action_buy.o

# action_sell.cpp
action_sell.o: src/action_sell.cpp src/action_sell.h
	$(COMPILE) src/action_sell.cpp -o obj/action_sell.o

# Compilation rule for mybot.cpp
a_main.o: mybot.cpp
	$(COMPILE) mybot.cpp -o obj/a_main.o

# Cleanup rule
clean:
	$(REMOVE) bot mybot.o obj/*