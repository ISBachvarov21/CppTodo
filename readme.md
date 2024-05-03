# cpp-todo
A simple todo app that runs in the browser.

## Building
### With docker
Requirements:
- Docker
- WSL (if on windows)

```
cd server
docker compose up --build
```
### Without docker
Requirements:
- CMake > 3.12

```
cd server
cmake -S . -B build
cmake --build build
```

#### *Notes:*
In case of console logging not working, use `stdbuf -oL <path to executable>`.

This will modify the program output so that it buffers every line.