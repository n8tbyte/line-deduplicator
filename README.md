# line-deduplicator

## Quick Build
g++ -O1 -DNDEBUG main.cpp -o main.exe

## Performance (games, real-time)
g++ -O3 -march=native -flto main.cpp -o main.exe

## Production (recommended)
g++ -O2 -DNDEBUG -Wall main.cpp -o main.exe

## Small size (embedded, downloads)
g++ -Os -s -flto main.cpp -o main.exe
g++ -Os -s -flto -fno-exceptions -fno-rtti -Wl,--gc-sections main.cpp -o main.exe

## Debug (development)
g++ -O0 -g -Wall main.cpp -o main.exe

## Security (banking, servers)
g++ -O2 -DNDEBUG -fstack-protector-strong -D_FORTIFY_SOURCE=2 main.cpp -o main.exe

## Static (no dependencies)
g++ -O2 -static main.cpp -o main.exe

## Full Production Build
g++ -O3 -march=native -flto -DNDEBUG -Wall -Wextra -Wpedantic -fstack-protector-strong -D_FORTIFY_SOURCE=2 main.cpp -o main.exe