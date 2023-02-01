/*
Please give optimizing option like -O1~3.
Refereces:
https://github.com/orgs/Homebrew/discussions/3659
https://stackoverflow.com/questions/73714336/xcode-update-to-version-2395-ld-compile-problem-occurs-computedatomcount-m
*/

#include <chrono>
#include <cstdlib>
#include <iostream>
#include <random>
#include <thread>
#include <vector>

using Cells = std::vector<std::vector<int>>;
std::random_device rd;
std::mt19937 mt(rd());
const int N = 32;
const int N_FRAMES = 1000;
const int FRAME_INTERVAL = 800;

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

void show(const Cells &cells) {
  std::system("clear");
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      std::cout << (cells[i][j] ? "*" : " ");
    }
    std::cout << std::endl;
  }
}

int main() {
  Cells cells(N, std::vector<int>(N, 0));
  init_cells(cells);
  int cnt = 0;
  while (cnt < N_FRAMES) {
    show(cells);
    cells = update(cells);
    std::this_thread::sleep_for(std::chrono::milliseconds(FRAME_INTERVAL));
    cnt++;
  }
  return 0;
}
