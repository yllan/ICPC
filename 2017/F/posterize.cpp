#include <iostream>
#include <algorithm>
#include <climits>
#include <vector>
#include <tuple>
using namespace std;

typedef long long ll;

#define REP(i, n)  for (int (i) = 0; (i) < (int)(n); (i)++)
#define INF  (1ll << 60)

ll e(int v1, int v2, vector<pair<int, ll>> &h) {
  ll ans = 0;
  for (auto r : h) {
    if (r.first <= v1) {
      continue;
    } else if (r.first >= v2) {
      break;
    } else {
      ll d = min(abs(r.first - v1), abs(v2 - r.first));
      ans += r.second * d * d;
    }
  }
  return ans;
}

ll optimize(int v, int left, vector<pair<int, ll>> &h) {
  if (left == 0) {
    return e(v, 1024, h);
  } else {
    ll best_value = INF;
    for (int next_v = v + 1; next_v < 256 - (left - 1); next_v++) {
      ll current_value = e(v, next_v, h) + optimize(next_v, left - 1, h);
      best_value = min(best_value, current_value);
    }
    return best_value;
  }
}

ll solve(vector<pair<int, ll>> &h, int k) {
  ll best_value = INF;
  for (int v = h[0].first; v < min(256 - (k - 1), h.back().first + 1); v++) {
    ll current_value = e(-1024, v, h) + optimize(v, k - 1, h);
    best_value = min(best_value, current_value);
  }
  return best_value;
}

ll dp(vector<pair<int, ll>> &h, int k) {
  ll m[k + 1][256];
  ll em[256][256];

  REP(i, k) REP(j, 256) m[i][j] = INF;
  REP(i, 256) m[0][i] = e(i, 1024, h);
  REP(i, 256) for (int j = i + 1; j < 256; j++) em[i][j] = e(i, j, h);

  for (int kk = 1; kk < k; kk++) {
    for (int v = 0; v < 256; v++) {
      ll best_value = INF;
      for (int next_v = v + 1; next_v < 256; next_v++) {
        ll current_value = em[v][next_v] + m[kk - 1][next_v];
        best_value = min(best_value, current_value);
      }
      m[kk][v] = best_value;
    }
  }
  ll best_value = LLONG_MAX;
  REP(v, 256) {
    ll current_value = e(-1024, v, h) + m[k - 1][v];
    best_value = min(best_value, current_value);
  }
  return best_value;
}

int main() {
  int d, k;
  vector<pair<int, ll>> histogram;
  cin >> d >> k;

  for (int i = 0; i < d; i++) {
    int r, p;
    cin >> r >> p;
    histogram.push_back(make_pair(r, p));
  }
  cout << dp(histogram, k) << endl;
}