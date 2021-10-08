#include<bits/stdc++.h>

using namespace std;
long long n, k;

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL); cout.tie(NULL);
    cin >> n >> k;
    vector<pair<long long, long long>> ops(n + 10);
    for (int i = 0; i < k; i++) {
        string o;
        int x1, x2;
        cin >> o >> x1 >> x2;
        if (o == "R") {
            ops[x1].first++;
            ops[x2 + 1].first--;
        } else if (o == "D") {
            ops[x1].first--;
            ops[x2 + 1].first++;
        } else if (o == "H") {
            if (x2 - x1 <= 1) {
                ops[x1].first++;
                ops[x2 + 1].first--;
            } else if ((x2 - x1) % 2 == 0) { // x1 ... x1 + k ... x2
                long long k = (x2 - x1) / 2;
                ops[x1].second++;
                ops[x1 + k].first += 2;
                ops[x1 + k].second -= 2;
                ops[x2 + 1].first--;
                ops[x2 + 1].second++;
            } else { // x1 ... x1 + k, x1 + k + 1, ... x2
                long long k = (x2 - x1) / 2;
                ops[x1].second++;
                ops[x1 + k].first++;
                ops[x1 + k].second--;
                ops[x1 + k + 1].first++;
                ops[x1 + k + 1].second--;
                ops[x2 + 1].first--;
                ops[x2 + 1].second++;
            }
        } else if (o == "V") {
            if (x2 - x1 <= 1) {
                ops[x1].first--;
                ops[x2 + 1].first++;
            } else if ((x2 - x1) % 2 == 0) { // x1 ... x1 + k ... x2
                long long k = (x2 - x1) / 2;
                ops[x1].second--;
                ops[x1 + k].first -= 2;
                ops[x1 + k].second += 2;
                ops[x2 + 1].first++;
                ops[x2 + 1].second--;
            } else { // x1 ... x1 + k, x1 + k + 1, ... x2
                long long k = (x2 - x1) / 2;
                ops[x1].second--;
                ops[x1 + k].first--;
                ops[x1 + k].second++;
                ops[x1 + k + 1].first--;
                ops[x1 + k + 1].second++;
                ops[x2 + 1].first++;
                ops[x2 + 1].second--;
            }
        }
    }

    pair<long long, long long> st(0, 0);

    for (int i=1; i<=n; i++) {
        st.first += ops[i].first;
        st.second += ops[i].second;
        st.first += st.second;
        cout << st.first << endl;
    }
}