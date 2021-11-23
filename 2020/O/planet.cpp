#include<iostream>
#include<tuple>
#include<vector>
#include<algorithm>

using namespace std;

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n;
    cin >> n;
    vector<pair<int, int>> planet[2];
    vector<pair<int, int>> sig[2];
    for (int t=0; t<2; t++) {
        for (int i=0; i<n; i++) {
            double lon, lat;
            cin >> lon >> lat;
            planet[t].emplace_back((lat + 180) * 10000.0 + 0.1, (lon + 90) * 10000.0 + 0.1);
        }
        sort(planet[t].begin(), planet[t].end());
        for (int i=0; i<n; i++) {
            sig[t].emplace_back(planet[t][i].second, (planet[t][i].first + 3600000 - planet[t][(i+1)%n].first) % 3600000);
        }
    }
    int i = -1, j = -1;
    while (i < n - 1 && j < n - 1) {
        int k = 1;
        while (k <= n && sig[0][(i+k) % n] == sig[1][(j+k) % n]) k++;
        if (k > n) {
            cout << "Same\n";
            return 0;
        }
        if (sig[0][(i+k) % n] < sig[1][(j+k)%n]) i += k; else j += k;
    }
    cout << "Different\n";
}