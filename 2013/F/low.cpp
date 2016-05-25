#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;
typedef long long ll;

int N, K;

bool can(vector<ll> b, ll d) {
  // b sorted ascending
  int last;
  
  // first pair, b[0], b[1]
  if (b[1] - b[0] > d) return false;
  last = 0;
  
  for (int i = 1; i < N; i++) {
    bool found = false;
    for (int j = last + 2; j <= i * 2 * K; j++) {
      if (b[j + 1] - b[j] <= d) {
        found = true;
        last = j;
        break;
      }
    }
    if (!found) return false;
  }
  
  return true;
}

int main()
{
  vector<ll> b;
  cin >> N >> K;
  for (int i = 0; i < 2 * N * K; i++) {
    ll p;
    cin >> p;
    b.push_back(p);
  }
  sort(b.begin(), b.end());
  ll upper = b[2 * N * K - 1] - b[0];
  ll lower = -1ll;
  do {
    ll mid = (upper + lower) / 2ll;
    // printf("%lld %lld %lld\n", lower, mid, upper);
    if (can(b, mid)) {
      upper = mid;
    } else {
      lower = mid;
    }
  } while (upper - lower > 1ll);
  cout << upper << endl;
}