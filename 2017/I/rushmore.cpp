#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;

bool trans[26][26];

bool match(char *u, char *v) {
  if (strlen(u) != strlen(v)) return false;
  for (int i = 0; i < strlen(u); i++) {
    if (!trans[u[i] - 'a'][v[i] - 'a']) return false;
  }
  return true;
}

int main() {
  int m, n;
  scanf("%d %d", &m, &n);
  for (int i = 0; i < m; i++) {
    char f[2], t[2];
    scanf("%s %s", f, t);
    trans[*f - 'a'][*t - 'a'] = true;
  }
  for (int i = 0; i < 26; i++) trans[i][i] = true;

  // warshall
  for (int k = 0; k < 26; k++) {
    for (int i = 0; i < 26; i++) {
      for (int j = 0; j < 26; j++)
        trans[i][j] |= (trans[i][k] && trans[k][j]);
    }
  }

  for (int i = 0; i < n; i++) {
    char word1[51], word2[51];
    scanf("%s %s", word1, word2);
    puts(match(word1, word2) ? "yes" : "no");
  }
}