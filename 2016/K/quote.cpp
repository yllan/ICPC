#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

bool q(vector<int> qs, int k)
{
  if (qs.size() == 0) return false;
  if (k == 1) {
    int qq = 0;
    for (int i = 0; i < qs.size(); i++) qq += qs[i];
    return qq > 0 && (qq % 2 == 0);
  }
  qs[0] -= k; qs[qs.size() - 1] -= k;
  if (qs[0] < 0 || qs[qs.size() - 1] < 0) return false;
  
  vector<int> subq;
  for (int i = 0; i < qs.size(); i++)
    if (qs[i] > 0) subq.push_back(qs[i]);
  return q(subq, k - 1);
}

int solve(vector<int> qs) {
  int qq = 0;
  for (int i = 0; i < qs.size(); i++) qq += qs[i];
  
  for (int k = min(qs[0], qs[qs.size() - 1]); k > 0; k--) 
    if (q(qs, k) && (k != 1 || qq == 2)) 
      return k;
  return -1;
}

int main()
{
  int n;
  vector<int> quotes;
  
  cin >> n;
  for (int i = 0; i < n; i++) {
    int a;
    cin >> a;
    quotes.push_back(a);
  } 
  
  int ans = solve(quotes);
  if (ans <= 0) {
    cout << "no quotation" << endl;
  } else {
    cout << ans << endl;
  }
}