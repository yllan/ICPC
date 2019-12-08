#include <string>
#include <iostream>
#include <cassert>
#include <tuple>
#include <unordered_map>
#include <chrono>
#include <vector>

using namespace std;

/**
 *  suffix(n, h): F(n) has suffix == pattern.substr(0, h)
 *    - len(F(n)) < h: false
 *    - len(F(n)) >= h && len(F(n - 2)) < h: suffix(n - 1, pattern.substr(0, h - len(F(n - 2)))) && F(n - 2) == pattern.substr(h - len(F(n - 2)))
 *    - len(F(n - 2)) >= h: suffix(n - 2, h)
 *
 *  prefix(n, t): F(n) has prefix == pattern.substr(pattern.size() - t, t);
 */

typedef unsigned long long ull;
#define MAX_F 90

unordered_map<ull, bool> mem;
ull len_f[MAX_F]; // the length of F[n]. (n > 90 will exceed the max of ull)
void init()
{
  len_f[0] = 1;
  len_f[1] = 1;
  for (int i = 2; i < MAX_F; i++)
    len_f[i] = len_f[i - 1] + len_f[i - 2];
}

bool prefix(int n, string &pat, int begin, int len)
{
  /* Case: len < 2, easy */
  if (len == 0) return true;
  if (len == 1 && pat[begin] == '0') return (n == 0); // pat is only "0". only F(0) begins with 0
  if (len == 1 && pat[begin] == '1') return (n > 0);  // pat is only "1". n > 0 all begins with 1

  /* Case: len >= 2, n < 2 */
  if (n == 0) return false;
  if (n == 1) return false;

  /* Case: len >= 2, n >= 2 */
  if (n > 0 && pat[begin] == '0' ) return false;

  if (n < MAX_F && len > len_f[n]) return false; // F[n] is too small to contain pat.substr(begin, len)

  while (n >= MAX_F || len_f[n] >= len) n--; // find the max n s.t. len_f[n] < len

  ull sig = ((((ull)begin * 1000000ull) + (ull)len) * 1000ull + (ull)n) * 2ull;
  auto it = mem.find(sig);
  if (it != mem.end()) {
    return it->second;
  } else {
    bool ans = prefix(n, pat, begin, len_f[n]) && prefix(n - 1, pat, begin + len_f[n], len - len_f[n]);
    mem[sig] = ans;
    return ans;
  }
}

bool suffix(int n, string &pat, int begin, int len)
{
  /* Case: len < 2, easy */
  if (len == 0) return true;
  if (len == 1 && pat[begin] == '0') return (n % 2 == 0); // only F(even) ends with 0
  if (len == 1 && pat[begin] == '1') return (n % 2 == 1); // only F(odd) ends with 0

  /* Case: len >= 2, n < 2 */
  if (n == 0) return false;
  if (n == 1) return false;

  /* Case: len >= 2, n >= 2 */
  if (n < MAX_F && len > len_f[n]) return false; // F[n] is too small to contain pat.substr(begin, len)
  if (pat[begin + len - 1] == '0' && n % 2 == 1) return false;
  if (pat[begin + len - 1] == '1' && n % 2 == 0) return false;

  while (n >= MAX_F) n -= 2;

  if (len_f[n - 2] > len) {
    return suffix(n - 2, pat, begin, len);
  } else {
    ull sig = ((((ull)begin * 1000000ull) + (ull)len) * 1000ull + (ull)n) * 2ull + 1ull;
    auto it = mem.find(sig);
    if (it != mem.end()) {
      return it->second;
    } else {
      bool ans = suffix(n - 1, pat, begin, len - len_f[n - 2]) && suffix(n - 2, pat, begin + len - len_f[n - 2], len_f[n - 2]);
      mem[sig] = ans;
      return ans;
    }
  }
}


int main(int argc, char const *argv[])
{
  int n, c = 1;
  string pat;
  init();

  while (cin >> n >> pat) {
    ull ans[n + 1];
    int m;
    mem.clear();
    ans[0] = (pat == "0" ? 1 : 0);
    ans[1] = (pat == "1" ? 1 : 0);
    for (m = 2; len_f[m] < pat.size(); m++) ans[m] = 0;

    bool calced[2] = { false, false };
    ull sub[2];

    for (; m <= n; m++) {
      ans[m] = ans[m - 1] + ans[m - 2];
      ull sub_answer = 0;
      // auto t1 = chrono::high_resolution_clock::now();
      // proper decomposition
      if (calced[m % 2]) {
        sub_answer = sub[m % 2];
      } else {
        for (int prefix_len = 1; prefix_len < pat.size(); prefix_len++) {
          sub_answer += suffix(m - 1, pat, 0, prefix_len) && prefix(m - 2, pat, prefix_len, pat.size() - prefix_len) ? 1 : 0;
        }
        if (len_f[m - 2] >= pat.size()) {
          sub[m % 2] = sub_answer;
          calced[m % 2] = true;
        }
      }
      // auto t2 = chrono::high_resolution_clock::now();
      // cerr << "Timing for " << m << "=" << sub_answer << ": " << chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << endl;
      ans[m] += sub_answer;
    }
    cout << "Case " << c++ << ": " << ans[n] << endl;
  }
  return 0;
}