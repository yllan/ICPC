#include<cstdio>
#include<iostream>
#include<vector>

using namespace std;

#define REPI(i,n) for(int (i)=0;(i)<(int)(n);(i)++)

int n = 0;
const char *digits[10][7] = {
  {
    " ** ",
    "*  *",
    "*  *",
    "    ",
    "*  *",
    "*  *",
    " ** "
  },
  {
    "    ",
    "   *",
    "   *",
    "    ",
    "   *",
    "   *",
    "    "
  },
  {
    " ** ",
    "   *",
    "   *",
    " ** ",
    "*   ",
    "*   ",
    " ** "
  },
  {
    " ** ",
    "   *",
    "   *",
    " ** ",
    "   *",
    "   *",
    " ** "
  },
  {
    "    ",
    "*  *",
    "*  *",
    " ** ",
    "   *",
    "   *",
    "    "
  },
  {
    " ** ",
    "*   ",
    "*   ",
    " ** ",
    "   *",
    "   *",
    " ** "
  },
  {
    " ** ",
    "*   ",
    "*   ",
    " ** ",
    "*  *",
    "*  *",
    " ** "
  },
  {
    " ** ",
    "   *",
    "   *",
    "    ",
    "   *",
    "   *",
    "    "
  },
  {
    " ** ",
    "*  *",
    "*  *",
    " ** ",
    "*  *",
    "*  *",
    " ** "
  },
  {
    " ** ",
    "*  *",
    "*  *",
    " ** ",
    "   *",
    "   *",
    " ** "
  }
};
const char *seg[7] = {
  " **   **     **   ** ",
  "*  * *  *   *  * *  *",
  "*  * *  * # *  * *  *",
  " **   **     **   ** ",
  "*  * *  * # *  * *  *",
  "*  * *  *   *  * *  *",
  " **   **     **   ** "
};

char clock_for[3000][7][22];
char clocks[101][7][22];
char answer[7][22];
char accum[7][22];

#define NON_SEG   '.'
#define NON_DET   '?'
#define WORKING   'W'
#define ALWAYSOFF '0'
#define ALWAYSON  '1'

#define PURE_OFF 1
#define PURE_ON 2
#define MIXED_ON_OFF 3

bool is_seg(int x, int y) {
  return seg[x][y] == '*';
}

bool is_colon(int x, int y) {
  return seg[x][y] == '#';
}

int next_minute(int t) {
  int mm = t % 100;
  int hh = t / 100;
  mm++;
  if (mm >= 60) {
    mm = 0;
    hh++;
  }
  if (hh >= 24) {
    hh = 0;
  }
  return hh * 100 + mm;
}

void render(int t, char c[7][22])
{
  REPI(i, 7) REPI(j, 21) c[i][j] = '.';
  int h1 = t / 1000;
  int h2 = (t / 100) % 10;
  int m1 = (t / 10) % 10;
  int m2 = t % 10;
  
  if (h1 > 0) {
    REPI(i, 7) REPI(j, 4) c[i][0 + j] = digits[h1][i][j] == '*' ? 'X' : '.';
  }
  REPI(i, 7) REPI(j, 4) c[i][5 + j] = digits[h2][i][j] == '*' ? 'X' : '.';
  REPI(i, 7) REPI(j, 4) c[i][12 + j] = digits[m1][i][j] == '*' ? 'X' : '.';
  REPI(i, 7) REPI(j, 4) c[i][17 + j] = digits[m2][i][j] == '*' ? 'X' : '.';
  REPI(i, 7) REPI(j, 21) if (is_colon(i, j)) c[i][j] = 'X';
  REPI(i, 7) c[i][21] = '\0';
}

bool fit(int start_time)
{
  int current = start_time;
  REPI(k, n) {
    REPI(i, 7) REPI(j, 21) 
      if (answer[i][j] == WORKING && clocks[k][i][j] != clock_for[current][i][j]) {
        // c[i][j] = '@';
        // fprintf(stderr, "start: %d, min: %d, (%d, %d)\n", start_time, k, i, j);
        // REPI(_i, 7) fprintf(stderr, "%s\n", clocks[k][_i]);
        // fprintf(stderr, "-------\n");
        // REPI(_i, 7) fprintf(stderr, "%s\n", c[_i]);
        // fprintf(stderr, "==============\n");
        return false;
      }
    current = next_minute(current);
  }
  return true;
}

bool exist_differ_at(int start_time, char should, int i, int j)
{
  int current = start_time;
  REPI(k, n) {
    if (clock_for[current][i][j] != should) return true;
    current = next_minute(current);
  }
  return false;
}

bool solve()
{
  // check the appear of lights
  REPI(i, 7) REPI(j, 21) accum[i][j] = 0;
  REPI(i, 7) {
    REPI(j, 21) answer[i][j] = (is_seg(i, j) || is_colon(i, j)) ? NON_DET : NON_SEG;
    answer[i][21] = '\0';
  }
  
  REPI(k, n) REPI(i, 7) REPI(j, 21)
    accum[i][j] |= (clocks[k][i][j] == '.') ? PURE_OFF : PURE_ON;

  REPI(i, 7) REPI(j, 21) 
    if (is_colon(i, j)) {
      if (accum[i][j] == PURE_OFF) answer[i][j] = ALWAYSOFF;
      else if (accum[i][j] == MIXED_ON_OFF) return false;
    } else if (is_seg(i, j) && accum[i][j] == MIXED_ON_OFF) {
      answer[i][j] = WORKING;
    }
  
  vector<int> valid_start_time;
  int t = 0;
  do {
    if (fit(t)) valid_start_time.push_back(t);
    t = next_minute(t);
  } while (t != 0);
  
  if (valid_start_time.size() == 0) return false;
  
  REPI(i, 7) REPI(j, 21) {
    if (!is_seg(i, j)) continue;
    if (accum[i][j] == PURE_ON || accum[i][j] == PURE_OFF) {
      char should_be = accum[i][j] == PURE_ON ? 'X' : '.';
      
      bool all_has_mixed = true;
      REPI(_t, valid_start_time.size()) {
        if (!exist_differ_at(valid_start_time[_t], should_be, i, j)) {
          all_has_mixed = false;
          break;
        }
      }
      if (all_has_mixed) {
        answer[i][j] = (accum[i][j] == PURE_ON) ? '1' : '0';
      }
    }
  }

  return true;
}

int main()
{
  // precompute
  int t = 0;
  REPI(_i, 24 * 60) {
    render(t, clock_for[t]);
    t = next_minute(t);
  }
  
  cin >> n;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < 7; j++)
      cin >> clocks[i][j];
  }
  
  if (solve()) 
    REPI(i, 7) puts(answer[i]);
  else
    puts("impossible");
}