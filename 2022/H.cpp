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
    if (closing == 0) {
      tablets.emplace_back(0, closing, opening, i + 1);
    } else if (opening > 0 && opening - closing >= 0) {
      tablets.emplace_back(1, closing, opening, i + 1);
    } else {
      tablets.emplace_back(2, closing, opening, i + 1);
    }
  }

  sort(tablets.begin(), tablets.end());

  int64_t balance = 0;
  for (auto [t, closing, opening, pos]: tablets) {
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
