#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <deque>
#include <queue>
#include <stack>
#include <set>
#include <map>
#include <algorithm>
#include <functional>
#include <utility>
#include <bitset>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <cstring>
#include <iomanip> 

using namespace std;

#define REPL(i,n) for(int (i)=0;(i)<(int)(n);(i)++)
#define EACH(c,itr) for(__typeof((c).begin()) itr=(c).begin();itr!=(c).end();itr++)
#define INF (1ll<<60)
typedef long long ll;

ll screen_x, screen_y;

struct win_t {
  ll x;
  ll y;
  ll w;
  ll h;

  win_t(ll x, ll y, ll w, ll h): x(x), y(y), w(w), h(h) {}

  ll min_x() { return x; }
  ll max_x() { return x + w - 1; }
  ll min_y() { return y; }
  ll max_y() { return y + h - 1; }

  bool contains(ll px, ll py) {
    return (min_x() <= px && px <= max_x() && min_y() <= py && py <= max_y());
  }

  bool overlap(win_t &o) {
    return !(o.max_x() < min_x() || o.min_x() > max_x() || 
             o.min_y() > max_y() || o.max_y() < min_y());
  }

  bool valid() {
    return min_x() >= 0 && min_y() >= 0 && max_x() < screen_x && max_y() < screen_y;
  }

  ll bound_of(ll dx, ll dy) {
    return dx > 0 ? max_x() : (dx < 0 ? min_x() : (dy > 0 ? max_y() : min_y()));
  }

  win_t extend(ll dx, ll dy) {
    if (dx < 0) {
      return win_t(x + dx, y, w - dx, h);
    } else if (dx > 0) {
      return win_t(x, y, w + dx, h);
    } else if (dy < 0) {
      return win_t(x, y + dy, w, h - dy);
    } else { // dy > 0
      return win_t(x, y, w, h + dy);
    }
  }
};

bool open_window(vector<win_t> &windows, win_t target)
{
  EACH(windows, w) if (w->overlap(target)) return false;
  windows.push_back(target);
  return true;
}

bool has_overlap(vector<win_t> &windows, win_t target, int skip)
{
  REPL(i, windows.size()) if (i != skip && windows[i].overlap(target)) return true;
  return false;
}

// return -1 if not found, otherwise the index of the window containing the pixel
int find_window(vector<win_t> &windows, ll x, ll y)
{
  REPL(i, windows.size()) if (windows[i].contains(x, y)) return i;
  return -1;
}

bool close_window(vector<win_t> &windows, ll x, ll y)
{
  int i = find_window(windows, x, y);
  if (i != -1) {
    windows.erase(windows.begin() + i);
  }
  return (i != -1);
}

ll move_window(vector<win_t> &windows, int index_to_move, ll dx, ll dy)
{
  ll moved = 0;
  ll bound = windows[index_to_move].bound_of(dx, dy);
  ll dir = (dx < 0 || dy < 0) ? -1LL : 1LL;

  vector<int> moving_indices;
  moving_indices.push_back(index_to_move);

  // find the obsctacle
  vector<int> possible_obstacles;
  REPL(i, windows.size()) {
    if (dir * bound < dir * windows[i].bound_of(-dx, -dy)) 
      possible_obstacles.push_back(i);
  }

  while (dx + dy != 0) {
    ll min_kissing_distance = llabs(dx + dy); // always positive

    if (dx > 0 && bound + min_kissing_distance >= screen_x) {
      min_kissing_distance = screen_x - bound - 1;
    } else if (dx < 0 && bound - min_kissing_distance < 0) {
      min_kissing_distance = bound;
    } else if (dy > 0 && bound + min_kissing_distance >= screen_y) {
      min_kissing_distance = screen_y - bound - 1;
    } else if (dy < 0 && bound - min_kissing_distance < 0) {
      min_kissing_distance = bound;
    }

    int moving_index = -1;
    int obsctacle_index = -1;

    REPL(i, moving_indices.size()) {
      REPL(j, possible_obstacles.size()) {
        win_t m = windows[moving_indices[i]];
        win_t o = windows[possible_obstacles[j]];
        if (!m.extend(dx, dy).overlap(o)) continue;

        ll kissing_distance = dir * (o.bound_of(-dx, -dy) - m.bound_of(dx, dy)) - 1;
        if (kissing_distance < min_kissing_distance) {
          min_kissing_distance = kissing_distance;
          moving_index = i;
          obsctacle_index = j;
        }
      }
    }

    if (min_kissing_distance == 0 && obsctacle_index == -1) return moved;
    
    EACH(moving_indices, idx) 
      ((dx != 0) ? windows[*idx].x : windows[*idx].y) += dir * min_kissing_distance;
    bound += dir * min_kissing_distance;
    moved += dir * min_kissing_distance;
    
    if ((dx > 0 && bound == screen_x - 1) || (dy > 0 && bound == screen_y - 1) ||
        (dx < 0 && bound == 0) || (dy < 0 && bound == 0)) return moved;

    if (obsctacle_index != -1) {
      bound = dir * max(dir * bound, dir * windows[possible_obstacles[obsctacle_index]].bound_of(dx, dy));
      moving_indices.push_back(possible_obstacles[obsctacle_index]);
      possible_obstacles.erase(possible_obstacles.begin() + obsctacle_index);
    }

    (dx != 0 ? dx : dy) -= dir * min_kissing_distance;
  }
  return moved;
}

int main()
{
  vector<win_t> windows;

  cin >> screen_x >> screen_y;
  REPL(i, 256) {
    string cmd;
    ll x, y, w, h, dx, dy;
    cin >> cmd >> x >> y;
    if (!cin) break;

    if (cmd == "OPEN") {
      cin >> w >> h;
      win_t target(x, y, w, h);
      if (!target.valid() || !open_window(windows, target)) {
        printf("Command %d: %s - window does not fit\n", i + 1, cmd.c_str());
      }
    } else if (cmd == "CLOSE") {
      if (!close_window(windows, x, y)) {
        printf("Command %d: %s - no window at given position\n", i + 1, cmd.c_str());
      }
    } else if (cmd == "RESIZE") {
      cin >> w >> h;
      int j = find_window(windows, x, y);
      if (j == -1) {
        printf("Command %d: %s - no window at given position\n", i + 1, cmd.c_str());
      } else {
        win_t target = windows[j];
        target.w = w; target.h = h;
        if (!target.valid() || has_overlap(windows, target, j)) {
          printf("Command %d: %s - window does not fit\n", i + 1, cmd.c_str());
        } else {
          windows[j].w = w;
          windows[j].h = h;
        }
      }
    } else if (cmd == "MOVE") {
      cin >> dx >> dy;
      int j = find_window(windows, x, y);
      if (j == -1) {
        printf("Command %d: %s - no window at given position\n", i + 1, cmd.c_str());
      } else if (dx + dy != 0) {
        ll actual_move = move_window(windows, j, dx, dy);

        if (actual_move != dx + dy) {
          printf("Command %d: %s - moved %lld instead of %lld\n", i + 1, cmd.c_str(), llabs(actual_move), llabs(dx + dy));
        }
      } // dx, dy == 0
    }
  }

  printf("%lu window(s):\n", windows.size());
  EACH(windows, w) printf("%lld %lld %lld %lld\n", w->x, w->y, w->w, w->h);
}