# game of life (wasm+cpp)

## Requirements
- [Emscripten](https://emscripten.org/index.html)
- C++17

## Usage
### setup
```
source </path/to/emsdk>/emsdk_env.sh
```

### build
```
em++ main.cpp -s WASM=1 -O3 -o index.js
```

### run
```
emrun --browser chrome index.html
```
