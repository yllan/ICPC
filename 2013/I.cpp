#include <cstdio>
#include <climits>
#include <cstdlib>
#include <cstring>

#include <algorithm> 
#include <stack>
#include <ctime>

#define MAX 500

int d[MAX][MAX];
int m, n;

struct IndexValue
{
  int idx;
  int v;
  IndexValue(int anIdx, int aV) : 
    idx(anIdx), v(aV) {}
};

bool allZero()
{
  for (int i = 0; i < m; i++)
    for (int j = 0; j < n; j++)
      if (d[i][j] != 0) return false;
  return true;
}

long long maxVolume(int topArea, int depth, int poolArea)
{
  long long numer = (long long)topArea * depth;
  long long denom = poolArea - topArea;
  long long delta =  numer / denom;
  if (delta * denom == numer) delta--;
  return topArea * (depth + delta);
}

long long popDescendingSeq(std::stack<IndexValue> *q, int min_depth, int w, int last_idx, int h_bound) 
{
  long long max = 0;
  while (!q->empty() && q->top().v >= min_depth) {

    IndexValue prev_iv = q->top();
    q->pop();

    int range_begin = q->empty() ? 0 : q->top().idx + 1;
    int range_end = last_idx;
    int h = std::min(range_end - range_begin + 1, h_bound);
    int topArea = w * h;
    int depth = prev_iv.v;
    long long volume = maxVolume(topArea, depth, m * n);
    max = std::max(max, volume);
  }
  return max;
}

long long solve(int a, int b)
{
  if (allZero()) return 0;
  
  int min_between_rows[MAX];
  long long max = 0;

  int large_bound = std::max(a, b);
  int small_bound = std::min(a, b);

  // Monotone increasing stack.
  std::stack<IndexValue> q;

  // Enumerate 1 ≤ row_begin ≤ row_end ≤ m.
  for (int row_begin = 0; row_begin < m; row_begin++) {
    memcpy(min_between_rows, d[row_begin], n * sizeof(int));

    for (int row_end = row_begin; row_end < m; row_end++) {
      int row_count = row_end - row_begin + 1;
      int h_bound = row_count <= small_bound ? large_bound : small_bound;
      if (row_count > large_bound) break;

      // Accumulate the minimum
      for (int c = 0; c < n; c++)
        min_between_rows[c] = std::min(d[row_end][c], min_between_rows[c]);

      for (int c = 0; c < n; c++) {
        int min_depth = min_between_rows[c];

        // Violate the monotone property, fix it!
        if (!q.empty() && q.top().v >= min_depth) 
          max = std::max(max, popDescendingSeq(&q, min_depth, row_count, c - 1, h_bound));

        q.push(IndexValue(c, min_depth));
      }
      max = std::max(max, popDescendingSeq(&q, -1, row_count, n - 1, h_bound));
    }
  }

  return max;
}

int main()
{
  int a, b;

  scanf("%d %d %d %d", &a, &b, &m, &n);
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      scanf("%d", &d[i][j]);
    }
  }

  long long solution = solve(a, b);
  printf("%lld\n", solution);
  return 0;
}