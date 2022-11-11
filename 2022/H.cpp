#include<vector>
#include<algorithm>
#include<iostream>
#include<tuple>
using namespace std;

int main() {
  int n;
  cin >> n;
  vector<tuple<int, int, int, int>> tablets; // type, closing, opening, position
  for (int i = 0; i < n; i++) {
    string s;
    cin >> s;

    int opening = 0, closing = 0;
    for (char c: s) {
      if (c == '(') {
        opening++;
      } else if (c == ')') {
        if (opening > 0) {
          opening--;
        } else {
          closing++;
        }
      }
    }
    if (opening - closing >= 0) {
      tablets.emplace_back(0, closing, opening, i + 1);
    } else {
      tablets.emplace_back(1, -opening, closing, i + 1);
    }
  }

  sort(tablets.begin(), tablets.end());
  int64_t balance = 0;
  for (auto [t, a, b, pos]: tablets) {
    int opening = t ? -a : b;
    int closing = t ? b : a;
    balance -= closing;
    if (balance < 0) break;
    balance += opening;
  }
  if (balance != 0) {
    cout << "impossible\n";
  } else {
    for (int i = 0; i < n; i++) cout << get<3>(tablets[i]) << '\n';
  }
}
