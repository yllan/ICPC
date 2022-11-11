#include<iostream>
#include<vector>
#include<algorithm>
#include<string>
#include<iomanip>
#include<map>
using namespace std;

using point=pair<int, int>;

struct Trie {
  map<int, Trie*> children;
  int count = 0;
  point pos;
};

void insert(Trie *t, point pos, vector<int> &path) {
  for (int p: path) {
    t->count++;
    if (t->children.find(p) == t->children.end()) {
      t->children[p] = new Trie();
    } 
    t = t->children[p];
    t->pos = pos;
  }
}

int dist(point p, point o) {
  int x = p.first - o.first;
  int y = p.second - o.second;
  int d = max(abs(x), abs(y));
  if (d == 0) return 1;
  if (y == -d) {
    if (x == -d) return (d * 2 + 1) * (d * 2 + 1);
    return (x + d) + (d * 2 - 1) * (d * 2 - 1);
  } else if (x == d) {
    return (y + d) + (d * 2 - 1) * (d * 2 - 1) + 2 * d;
  } else if (y == d) {
    return (d - x) + (d * 2 - 1) * (d * 2 - 1) + 4 * d;
  } else {
    return (d - y) + (d * 2 - 1) * (d * 2 - 1) + 6 * d;
  }
}

int main() {
  int dx, dy;
  cin >> dx >> dy;
  vector<string> grid(dy);
  for (auto &row: grid) cin >> row;

  vector<point> markers;
  for (int y = 0; y < dy; y++) for (int x = 0; x < dx; x++) 
    if (grid[y][x] == 'X') markers.emplace_back(x, y);
  
  Trie *t = new Trie();
  for (int y = 0; y < dy; y++) for (int x = 0; x < dx; x++) {
    vector<int> path;
    for (auto m: markers) 
      path.push_back(dist(m, {x, y}) - 1);
    sort(path.begin(), path.end());
    insert(t, {x, y}, path);
  }

  double total = 0;
  int maxStep = -1;
  vector<vector<int>> steps(dy, vector<int>(dx));
  function<void(Trie *)> traverse = [&](Trie *t) {
    int prev_k = -1;
    for (auto &[k, tt]: t->children) {
      if (tt->count <= 1) {
        int cost = (k == t->children.rbegin()->first && prev_k >= 0) ? prev_k : k;
        total += cost;
        maxStep = max(maxStep, cost);
        steps[dy - 1 - tt->pos.second][tt->pos.first] = cost;
      } else {
        traverse(tt);
      }
      prev_k = k;
    }
  };
  traverse(t);

  cout << fixed << setprecision(10) << total / (dy * dx) << endl;
  cout << maxStep << endl;
  bool first = true;
  for (int y = 0; y < dy; y++) for (int x = 0; x < dx; x++) 
    if (steps[y][x] == maxStep) {
      if (!first) cout << ' ';
      cout << '(' << x+1 << ',' << y+1 << ')';
      first = false;
    }
  cout << endl;
}
