#include<iostream>
#include<algorithm>
#include<vector>
#include<tuple>

using namespace std;

typedef long long ll;
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;

bool mless(pll a, pll b) {
  if (a.second < b.second) return true;
  if (a.second > b.second) return false;
  return (a.first < b.first);
}

int main()
{
  int n;
  cin >> n;
  vector<pii> pos;
  vector<pii> zero;
  vector<pii> neg;
  for (int i = 0; i < n; i++) {
    ll a, b;
    cin >> a >> b;
    if (a == b) {
      zero.push_back(make_pair(a, b));
    } else if (a < b) {
      pos.push_back(make_pair(a, b));
    } else {
      neg.push_back(make_pair(a, b));
    }
  }
  sort(zero.begin(), zero.end());
  sort(pos.begin(), pos.end());
  sort(neg.begin(), neg.end(), mless);
  // reverse((neg).begin(), (neg).end());
  
  ll free_space = 0ll;
  ll need = 0ll;
  for (int i = 0; i < pos.size(); i++) {
    pll disk = pos[i];
    cerr << disk.first << " +> " << disk.second << endl;
    if (free_space < disk.first) {
      ll delta = disk.first - free_space;
      need += delta;
      free_space += delta;
    }
    free_space += disk.second - disk.first;
    cerr << "free: " << free_space << endl;
  }
  
  if (zero.size() > 0) { // choose the max one.
    pll disk = zero[zero.size() - 1];
    cerr << disk.first << " => " << disk.second << endl;
    if (free_space < disk.first) {
      ll delta = disk.first - free_space;
      need += delta;
      free_space += delta;
    }
    cerr << "free: " << free_space << endl;
  }
  
  for (int i = neg.size() - 1; i >= 0; i--) {
  // for (int i = 0; i < neg.size(); i++) {
    pll disk = neg[i];
    cerr << disk.first << " -> " << disk.second << endl;
    if (free_space < disk.first) {
      ll delta = disk.first - free_space;
      need += delta;
      free_space += delta;
    }
    free_space += disk.second - disk.first;
  }
  cout << need << endl;
}