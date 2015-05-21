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

#define REP(i,n) for((i)=0;(i)<(int)(n);(i)++)
#define REPD(i,n) for(int (i)=0;(i)<(int)(n);(i)++)
#define snuke(c,itr) for(__typeof((c).begin()) itr=(c).begin();itr!=(c).end();itr++)

typedef long long ll;
typedef long double ld;
#define INF (1ll<<60)

int n, s;

typedef struct {
    ld r;
    ld x;
    ld y;
    ld z;

    ld volume() {
        return (r * r * r * M_PI * 4.0) / 3.0;
    };

    ld volume(ld bz) {
        ld lower = -r;
        ld upper = min(max(bz - z, -r), r);
#define INTEGRAL(t)  (r * r * t - t * t * t / 3.0)
        return M_PI * (INTEGRAL(upper) - INTEGRAL(lower));
#undef INTEGRAL
    }
} hole_t;

ld cheese_volume_at(vector<hole_t> &holes, ld z)
{
    ld v = 100000.0L * 100000.0L * z;
    snuke(holes, h) v -= h->volume(z);
    return v;
}

ld approximate(ld lower, ld upper, ld target, vector<hole_t> &holes)
{
    ld mid = (lower + upper) / 2.0;
    if (upper - lower < 0.0000000001) return mid;
    ld mid_v = cheese_volume_at(holes, mid);
    // cout << "lower:" << lower << ", upper:" << upper << ", mid:" << mid << ", mid_v:" << mid_v << endl;
    if (target < mid_v) {
        return approximate(lower, mid, target, holes);
    } else if (target > mid_v) {
        return approximate(mid, upper, target, holes);
    } else {
        return mid;
    }
}

void cut(ld v, int s, vector<hole_t> &holes)
{
    ld prev_z = 0;
    for (int i = 1; i < s; i++) {
        ld target = v * i / s;
        // cout << "target:" << target << ", prev_z:" << prev_z << endl;
        ld z = approximate(prev_z, 100 * 1000, target, holes);
        cout << setprecision(14) << (z - prev_z) / 1000.0 << endl;
        prev_z = z;
    }
    cout << setprecision(14) << 100.0 - prev_z / 1000.0 << endl;
}

int main()
{
    cin >> n >> s;
    vector<hole_t> holes;
    
    REPD(i, n) {
        hole_t h;
        cin >> h.r >> h.x >> h.y >> h.z;
        holes.push_back(h);
    }

    ld volume = 100000.0L * 100000.0L * 100000.0L;
    snuke(holes, h) volume -= h->volume();
    // cout << setprecision(14) << volume / 1000000000.0L << endl;
    
    cut(volume, s, holes);

    return 0;
}