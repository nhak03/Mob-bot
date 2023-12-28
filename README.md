Swiss Banker will be a themed 1930s mob boss economy bot.

Created by Nicolas Hak using the library DPP - https://dpp.dev/index.html

Found Faults
1. Dictionary class can not handle multithreading
--> simple mutexes to resolve, add locks to each user node obj, and overall dictionary obj when inserting/deleting nodes
