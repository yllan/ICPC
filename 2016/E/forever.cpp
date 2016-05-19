#include<iostream>
#include<algorithm>
#include<set>
#include<vector>
#include<string>

using namespace std;

typedef long long ll;
// template <typename T>
// inline std::ostream& operator<< (std::ostream& os, const std::vector<T>& v) {
//   std::copy (v.begin(), v.end(), std::ostream_iterator<T>(os, " "));
//   return os;
// }

vector<ll> digits(ll x, ll base)
{
  vector<ll> d;
  do {
    d.push_back(x % base);
    x /= base;
  } while (x > 0LL);
  reverse(d.begin(), d.end());
  return d;
}

ll eval(vector<ll> &d, ll base) 
{
  ll v = 0ll;
  for (int i = 0; i < d.size(); i++) v = v * base + d[i];
  return v;
}

// true if a ≥ b
bool ge(vector<ll> &a, vector<ll> &b)
{
  if (a.size() > b.size()) return true;
  if (a.size() < b.size()) return false;
  for (int i = 0; i < a.size(); i++) {
    if (a[i] < b[i]) return false;
    if (a[i] > b[i]) return true;
  }
  return true;
}

// find the max b s.t. digits(y, b) ≥ m
ll find_max_base(vector<ll> &m, ll y)
{
  ll lower = 10ll;
  ll upper = y + 1LL;
  // invariant: digits(y, lower) ≥ m, digits(y, upper) < m 
  
  while (lower + 1 < upper) {
    ll mid = (lower + upper) / 2ll;
    vector<ll> t = digits(y, mid); 
    if (ge(t, m)) {
      lower = mid;
    } else {
      upper = mid;
    }
  }
  return lower;
}

bool in_deci(vector<ll> &a)
{
  for (int i = 0; i < a.size(); i++) {
    if (a[i] > 9) return false;
  }
  return true;
}

ll solve(ll y, ll l)
{
  // simple case
  if (l == 10LL) return y;
  if (l == y) return 10LL;
  
  vector<ll> lb = digits(l, 10LL);
  vector<ll> magnitude = lb;
  for (int i = 1; i < magnitude.size(); i++) magnitude[i] = 0;

  while (1) {
    ll b = find_max_base(magnitude, y);
    while (1) {
      vector<ll> d = digits(y, b);
      if (d[1] > 9) break;
      if (ge(d, lb) && in_deci(d)) {
        return b;
      }
      b--;  
    }    
   
    // increase magnitude
    if (magnitude[0] == 9) {
      magnitude[0] = 1;
      magnitude.push_back(0);
    } else {
      magnitude[0]++;
    }
  }
  
  // for (ll b = y; b >= 10; b--) {
  //   vector<ll> t = digits(y, b);
  //   if (in_deci(t) && ge(t, lb)) return b;
  // }
  return -1LL;
}

int main()
{
  int T = 1;
  // cin >> T; 
  
  for (int case_t = 1; case_t <= T; case_t++) {
    ll y, l;
    cin >> y >> l;
    ll ans = solve(y, l);
    cout << ans << endl;
    
  }
}