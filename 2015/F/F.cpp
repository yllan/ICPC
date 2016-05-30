#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <queue>
#include <stack>
#include <set>
#include <map>
#include <tuple>
#include <algorithm>
#include <utility>
#include <bitset>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <cstring>

using namespace std;

#define REP(i,n) for((i)=0;(i)<(int)(n);(i)++)
#define REPD(i,n) for(int (i)=0;(i)<(int)(n);(i)++)
#define snuke(c,itr) for(__typeof((c).begin()) itr=(c).begin();itr!=(c).end();itr++)

typedef long long ll;
#define INF (1ll<<60)

#define MAX_KB_SIZE 100

int r, c;
char keyboard[MAX_KB_SIZE][MAX_KB_SIZE];
enum {
  N = 0,
  E,
  W,
  S
};
int displace[4][MAX_KB_SIZE][MAX_KB_SIZE];

void find_displace(int direction) {
  int di[4] = {-1, 0, 0, 1};
  int dj[4] = {0, 1, -1, 0};

  for (int i = 0; i < r; i++) {
    for (int j = 0; j < c; j++) {
      int ni = i + di[direction], nj = j + dj[direction]; 
      for (;
           ni >= 0 && ni < r && nj >= 0 && nj < c && keyboard[i][j] == keyboard[ni][nj];
           ni = ni + di[direction], nj = nj + dj[direction]) {
      }
      displace[direction][i][j] = (ni < 0 || ni >= r || nj < 0 || nj >= c) ? 0 : (ni - i) + (nj - j);
    }
  }
}

typedef tuple<int, int, int> node;
int dist[MAX_KB_SIZE][MAX_KB_SIZE][10010];


int main()
{
  string text;
  cin >> r >> c;
  REPD(i, r) cin >> keyboard[i];
  REPD(d, 4) find_displace(d);

  cin >> text;
  text.push_back('*');
  
  deque<node> q;
  q.push_back(make_tuple(0, 0, 0));
  dist[0][0][0] = 1;

  while (!q.empty()) {
    node n = q.front();
    q.pop_front();

    int i = get<0>(n);
    int j = get<1>(n);
    int k = get<2>(n);

    REPD(d, 4) {
      int ni = i + ((d == N || d == S) ? displace[d][i][j] : 0);
      int nj = j + ((d == N || d == S) ? 0 : displace[d][i][j]);

      if (dist[ni][nj][k] == 0) {
        dist[ni][nj][k] = dist[i][j][k] + 1;
        q.push_back(make_tuple(ni, nj, k));
      }
    }
    if (k < text.size() && keyboard[i][j] == text[k]) {
      if (k == text.size() - 1) {
        cout << dist[i][j][k] << endl;
        break;
      }
      if (dist[i][j][k + 1] == 0) {
        dist[i][j][k + 1] = dist[i][j][k] + 1;
        q.push_back(make_tuple(i, j, k + 1));
      }
    }
  }
 
  return 0;
}