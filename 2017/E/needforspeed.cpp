#include<iostream>
#include<algorithm>
#include<set>
#include<vector>
#include<string>
#include<cstdio>
#include<cmath>

using namespace std;

int main() {
  int n, t;
  int d[1001], s[1001];

  scanf("%d %d", &n, &t);
  for (int i = 0; i < n; i++) {
    scanf("%d %d", &d[i], &s[i]);
  }

  double lower = -(*min_element(s, s + n));
  double upper = 10000000;
  double error = 0.000000001;

  while (fabs(lower - upper) >= error) {
    double c = (lower + upper) / 2.0;
    double tt = 0;
    for (int i = 0; i < n; i++) {
      tt += ((double)d[i]) / (s[i] + c);
    }
    double e = t - tt;
    //fprintf(stderr, "c=%lf, tt=%lf, e=%lf\n", c, tt, e);
    if (fabs(e) < error) {
      break;
    } else if (e > 0) { // t > tt
      upper = c;
    } else { // t < tt
      lower = c;
    }
  }
  printf("%.9lf\n", (lower + upper) / 2.0);
  return 0;
}