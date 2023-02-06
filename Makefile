EMCC=em++
CFLAGS=-std=c++17 -O3
EMCCFLAGS=-s WASM=1 -s USE_SDL=2 -s EXPORT_ES6
EMRUNFLAGS=--browser chrome
SRCS=main.cpp
OBJS=$(SRCS:%.cpp=%.o)
TGT=index.js
WASMOBJ=$(TGT:%.js=%.wasm)

build: $(OBJS) $(TGT)

run: build
	emrun $(EMRUNFLAGS) index.html

# $<: dependency
# $@: target
%.o: %.cpp
	$(EMCC) $(CFLAGS) -c $< -o $@

$(TGT): $(SRCS)
	$(EMCC) $(CFLAGS) $(EMCCFLAGS) $(OBJS) -o $(TGT)

clean:
	rm -f *.o *.wasm $(TGT)

.PHONY: build run clean
