/*
Please give optimizing option like -O1~3.
Refereces:
https://github.com/orgs/Homebrew/discussions/3659
https://stackoverflow.com/questions/73714336/xcode-update-to-version-2395-ld-compile-problem-occurs-computedatomcount-m
*/

#include <SDL2/SDL.h>
#include <chrono>
#include <cstdlib>
#include <emscripten.h>
#include <iostream>
#include <random>
#include <thread>
#include <vector>

using Cells = std::vector<std::vector<int>>;
struct Arg {
  Cells cells;
  int frame_cnt;
};

std::random_device rd;
std::mt19937 mt(rd());

const int CELL_SIZE = 10;
const int NUM_CELL_WIDTH = 32, NUM_CELL_HEIGHT = 32;
const int WINDOW_WIDTH = CELL_SIZE * NUM_CELL_WIDTH,
          WINDOW_HEIGHT = CELL_SIZE * NUM_CELL_HEIGHT;

const int NUM_FRAMES = 10000;
const int FRAME_INTERVAL = 800;

SDL_Window *window;
SDL_Renderer *renderer;

int rand(int max) {
  std::uniform_int_distribution<int> dist(0, max - 1);
  return dist(mt);
}

void init_cells(Cells &cells) {
  int h = cells.size(), w = cells[0].size();
  for (int i = 0; i < h * w / 4; i++) {
    cells[rand(h)][rand(w)] = 1;
  }
}

int is_alive(const Cells &cells, int x, int y) {
  int h = cells.size(), w = cells[0].size();
  int current = cells[y][x];
  int cnt = -current;
  for (int i = -1; i <= 1; i++) {
    for (int j = -1; j <= 1; j++) {
      if ((0 <= x + j && x + j < h) && (0 <= y + i && y + i < w)) {
        cnt += cells[y + i][x + j];
      }
    }
  }
  if ((cnt == 3) || ((current == 1) && (cnt == 2))) {
    return 1;
  } else {
    return 0;
  }
}

Cells update(const Cells &cells) {
  int h = cells.size(), w = cells[0].size();
  Cells new_cells(h, std::vector<int>(w, 0));
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      new_cells[i][j] = is_alive(cells, j, i);
    }
  }
  return new_cells;
}

void setup() {
  SDL_Init(SDL_INIT_VIDEO);
  // uint32_t flags = SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN;
  int flags = 0;
  SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, flags, &window,
                              &renderer);
}

void draw(const Cells &cells) {
  // set black background
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
  // draw green cells
  SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
  SDL_Rect rect = {.x = 10, .y = 10, .w = 150, .h = 100};
  int h = cells.size(), w = cells[0].size();
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      if (cells[i][j]) {
        SDL_Rect cell = {.x = j * CELL_SIZE,
                         .y = i * CELL_SIZE,
                         .w = CELL_SIZE,
                         .h = CELL_SIZE};
        SDL_RenderFillRect(renderer, &cell);
      }
    }
  }
  // Update the screen with any rendering performed since the previous call.
  SDL_RenderPresent(renderer);
}

void quit() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void main_loop(void *void_arg) {
  Arg *arg = static_cast<Arg *>(void_arg);
  if (arg->frame_cnt > NUM_FRAMES) {
    return;
  }

  draw(arg->cells);
  arg->cells = update(arg->cells);
  std::this_thread::sleep_for(std::chrono::milliseconds(FRAME_INTERVAL));
  arg->frame_cnt++;
}

int main() {
  Cells cells(NUM_CELL_HEIGHT, std::vector<int>(NUM_CELL_WIDTH, 0));
  init_cells(cells);

  setup();

  Arg arg = {.cells = cells, .frame_cnt = 0};
  const int fps = 0;
  const int simulate_infinite_loop = 1;
  emscripten_set_main_loop_arg(main_loop, &arg, fps, simulate_infinite_loop);

  quit();

  return 0;
}
