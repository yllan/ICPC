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

using namespace std;

#define REP(i,n) for((i)=0;(i)<(int)(n);(i)++)
#define REPD(i,n) for(int (i)=0;(i)<(int)(n);(i)++)
#define snuke(c,itr) for(__typeof((c).begin()) itr=(c).begin();itr!=(c).end();itr++)

typedef long long ll;
#define INF (1ll<<60)

// Let's try the professional template! It's copied from infamous rng_58.

int r, c;
char keyboard[60][60];
string text;

ll dist[3000][3000];

inline int node_of(int x, int y) {
	return x * c + y;
}

inline int row_of(int n) {
	return n / c;
}

inline int column_of(int n) {
	return n % c;
}

void construct_distance() 
{
	REPD(i, r * c) REPD(j, r * c) dist[i][j] = (i == j) ? 0 : INF;

	int dx[] = {0, 1, 0, -1};
	int dy[] = {1, 0, -1, 0};
	
	REPD(i, r) REPD(j, c) {
		char key = keyboard[i][j];
		REPD(d, 4) {
			int ni = i, nj = j;
			while (0 <= ni && ni < r && 0 <= nj && nj < c) {
				if (keyboard[ni][nj] != key) {
					dist[node_of(i, j)][node_of(ni, nj)] = 1;
					break;
				}
				ni += dx[d];
				nj += dy[d];
			}
		}
	}
}

void warshall_floyd()
{
	REPD(k, r * c) REPD(i, r * c) REPD(j, r * c)
		if (dist[i][k] + dist[k][j] < dist[i][j])
			dist[i][j] = dist[i][k] + dist[k][j];
}

int main()
{
	cin >> r >> c;
	REPD(i, r) cin >> keyboard[i];
	cin >> text;

	text.push_back('*');
	int length = text.length();

	// shortest path
	construct_distance();
	warshall_floyd();

	// REPD(i, r * c) REPD(j, r * c) 
	// 	if (dist[i][j] < INF && dist[i][j] > 0) 
	// 		printf("(%d,%d) -> [%d,%d] = %lld\n", row_of(i), column_of(i), row_of(j), column_of(j), dist[i][j]);

	map<int, ll> current;

	map<char, vector<int> > nodes_of_key;
	REPD(i, r) REPD(j, c) nodes_of_key[keyboard[i][j]].push_back(node_of(i, j));

	current[0] = 0LL;

	snuke(text, k) {
		map<int, ll> next;
		snuke(current, it) snuke(nodes_of_key[*k], jt) {
			int current_idx = it->first;
			int next_idx = *jt;
			ll current_dist = it->second;
			next[next_idx] = min(next.find(next_idx) == next.end() ? INF : next[next_idx],
								 current_dist + dist[current_idx][next_idx]);
		}
		current = next;
	}

	ll min_keystroke = INF;
	snuke(current, it) min_keystroke = min(min_keystroke, it->second);
	cout << (min_keystroke + length) << endl; 
}