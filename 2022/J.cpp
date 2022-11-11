#include<bits/stdc++.h>
using namespace std;

#ifdef LOCAL
#include<yl/debug.h>
#else
#define debug(...) 0
#endif

using node=tuple<string, int, int, int>;

int main() {
  int m, n, q;
  cin >> m >> n >> q;

  vector<node> nodes(n);
  
  int streamCount = 0;
  unordered_map<int, int> fromNode;
  unordered_map<int, int> toNode;

  for (int i = 0; i < n; i++) {
    auto &[type, x, y, z] = nodes[i];
    cin >> type >> x >> y >> z;
    x--, y--, z--;
    if (type == "S") { // split
      toNode[x] = i;
      fromNode[y] = i;
      fromNode[z] = i;
    } else { // merge
      toNode[x] = i;
      toNode[y] = i;
      fromNode[z] = i;
    }
    streamCount = max({streamCount, x, y, z});
  }
  streamCount++;

  vector<int> streamSize(streamCount);
  vector<bool> visited(streamCount);
  streamSize[0] = m;
  visited[0] = true;
  queue<int> que;
  que.push(0);
  auto explore = [&](int i) {
    visited[i] = true;
    if (toNode.find(i) == toNode.end()) return;
    auto &[type, x, y, z] = nodes[toNode[i]];
    if (type == "S" || (visited[x] && visited[y])) que.push(i);
  };

  while (!que.empty()) {
    int i = que.front(); que.pop();
    int sz = streamSize[i];
    if (toNode.find(i) == toNode.end()) break;
    auto &[type, x, y, z] = nodes[toNode[i]];
    if (type == "S") {
      streamSize[y] = (sz + 1) / 2;
      streamSize[z] = sz / 2;
      explore(y);
      explore(z);
    } else {
      streamSize[z] = streamSize[y] + streamSize[x];
      explore(z);
    }
  }

  for (; q > 0; q--) {
    int qx, qk;
    cin >> qx >> qk;
    qx--;

    while (qx != 0) {
      // debug(qx, qk);
      if (qk > streamSize[qx]) break;
      auto &[type, x, y, z] = nodes[fromNode[qx]];
      if (type == "S") {
        if (qx == y) {
          qk = qk * 2 - 1;
        } else { // qx == z
          qk *= 2;
        }
        qx = x;
      } else {
        // x1, y1, x2, y2, .... xM, yM, ....
        int commonLength = min(streamSize[x], streamSize[y]);
        if (qk <= commonLength * 2) {
          qx = qk % 2 == 1 ? x : y;
          qk = (qk + 1) / 2;
        } else { 
          qk -= commonLength;
          qx = streamSize[x] == commonLength ? y : x;
        }
      }
    }
    if (qk > streamSize[qx]) {
      cout << "none" << endl;
    } else {
      cout << qk << endl;
    }
  }
}
