#include <iostream>
#include <algorithm>
#include <deque>
#include <vector>
#include <set>

struct tile {
    int price;
    int height;
    int pos;
};

// h, p ≤ 10^9 can fit in 32bit int
using namespace std;

struct CompareHeight {
    bool operator()(const tile& a, const tile& b) const {
        return a.height < b.height;
    }
};

bool price_lt(tile a, tile b) {
    return a.price < b.price;
}

int main() {
    int n;
    cin >> n;

    deque<tile> front_row(n);
    deque<tile> back_row(n);

    // input
    for (int i = 0; i < n; i++) cin >> back_row[i].price;
    for (int i = 0; i < n; i++) cin >> back_row[i].height;
    for (int i = 0; i < n; i++) back_row[i].pos = i + 1;
    for (int i = 0; i < n; i++) cin >> front_row[i].price;
    for (int i = 0; i < n; i++) cin >> front_row[i].height;
    for (int i = 0; i < n; i++) front_row[i].pos = i + 1;

    // sort
    sort(front_row.begin(), front_row.end(), price_lt);
    sort(back_row.begin(), back_row.end(), price_lt);

    multiset<tile, CompareHeight> front_segment;
    multiset<tile, CompareHeight> back_segment;

    vector<int> front_pos;
    vector<int> back_pos;

    while (true) {
        if (front_segment.empty()) {
            if (front_row.empty()) break; // finish
            do {
                front_segment.insert(front_row.front());
                front_row.pop_front();
            } while (!front_row.empty() && front_row.front().price == front_segment.begin()->price);
        }
        if (back_segment.empty()) {
            if (back_row.empty()) break; // finish
            do {
                back_segment.insert(back_row.front());
                back_row.pop_front();
            } while (!back_row.empty() && back_row.front().price == back_segment.begin()->price);
        }
        if (back_segment.size() > front_segment.size()) {
            while (!front_segment.empty()) {
                auto fit = front_segment.begin();
                auto bit = back_segment.upper_bound(*fit);
                if (bit != back_segment.end()) {
                    front_pos.push_back(fit->pos);
                    back_pos.push_back(bit->pos);
                    front_segment.erase(fit);
                    back_segment.erase(bit);
                } else {
                    cout << "impossible" << endl;
                    return 0;
                }
            }
        } else {
            while (!back_segment.empty()) {
                auto bit = back_segment.begin();
                auto fit = front_segment.lower_bound(*bit);
                if (fit == front_segment.begin()) {
                    cout << "impossible" << endl;
                    return 0;
                } else {
                    fit--;
                    front_pos.push_back(fit->pos);
                    back_pos.push_back(bit->pos);
                    front_segment.erase(fit);
                    back_segment.erase(bit);
                }
            }
        }
    }

    for (auto t : back_pos) cout << t << " ";
    cout << endl;
    for (auto t : front_pos) cout << t << " ";
    cout << endl;
}