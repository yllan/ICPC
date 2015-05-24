#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <deque>
#include <queue>
#include <stack>
#include <set>
#include <map>
#include <algorithm>
#include <functional>
#include <utility>
#include <bitset>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <cstring>
#include <iomanip> 

using namespace std;

#define FOREACH(c,itr) for(__typeof((c).begin()) itr=(c).begin();itr!=(c).end();itr++)
#define INF (1ll<<60)

int n;
long double w, u, v, t1, t2;
long double time_to_pass_a_lane;


typedef pair<long double, long double> interval_t;

inline bool non_empty(interval_t i)
{
  return i.first < i.second;
}

interval_t intersect(interval_t a, interval_t b)
{
  return make_pair(max(a.first, b.first), min(a.second, b.second));
}

vector<interval_t> pass_lane(vector<interval_t> &gaps, vector<interval_t> &time_intervals)
{
  vector<interval_t> result;

  // assert: gaps are increasing and non-overlapping. 
  // assert: time_intervals are increasing and non-overlapping.
  
  int skip = 0;
  FOREACH(time_intervals, t) {
    for (int idx = skip; idx < gaps.size(); idx++) {
      interval_t g = gaps[idx];
      if (g.first > t->second + time_to_pass_a_lane) break;
      interval_t next = intersect(g, make_pair(t->first, t->second + time_to_pass_a_lane));
      next.second -= time_to_pass_a_lane;
      if (non_empty(next)) result.push_back(next);
      skip = idx;
    }
  }

  return result;
}

int main()
{
  scanf("%d %Lf %Lf %Lf %Lf %Lf", &n, &w, &u, &v, &t1, &t2);
  time_to_pass_a_lane = w / v;

  vector<interval_t> valid_begin_intervals;
  valid_begin_intervals.push_back(make_pair(t1, t2));


  for (int i = 0; i < n; i++) {
    long double ferry_arrived_time = time_to_pass_a_lane * i; 

    char direction[2];
    int m_i;
    scanf("%s %d", direction, &m_i);

    // read the ships position, then normalize to time arriving to origin.
    vector<interval_t> ships;
    for(int j = 0; j < m_i; j++) {
      long double len, pos;
      scanf("%Lf %Lf", &len, &pos);
      pos *= (*direction == 'E') ? -1.0l : 1.0l;
      ships.push_back(make_pair((pos / u) - ferry_arrived_time, ((pos + len) / u) - ferry_arrived_time));
    }
    if (*direction == 'E') reverse(ships.begin(), ships.end());
    
    if (ships.size() == 0) continue;

    // calculate the gaps between ships
    vector<interval_t> gaps;
    FOREACH(ships, s) {
      interval_t gap = (s == ships.begin()) ? make_pair((long double)-INF, s->first) : make_pair(prev(s)->second, s->first);
      if (non_empty(gap)) gaps.push_back(gap);
    }
    gaps.push_back(make_pair(ships.back().second, INF));

    valid_begin_intervals = pass_lane(gaps, valid_begin_intervals);   
  }

  long double max_interval = 0ll;
  FOREACH(valid_begin_intervals, interval) 
    max_interval = max(max_interval, interval->second - interval->first);
  
  printf("%.8Lf\n", max_interval);
  return 0;
}