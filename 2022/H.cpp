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
    int type = closing == 0 ? 0 : (opening == 0 ? 2: 1); // ((( first, )))((( middle, ))) last
    tablets.emplace_back(type, closing, opening, i + 1);
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