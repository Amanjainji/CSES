#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const int MAXN = 200005;
int seg[4 * MAXN];
int h[MAXN];

// Build the segment tree with max values
void build(int node, int low, int high) {
    if (low == high) {
        seg[node] = h[low];
        return;
    }
    int mid = (low + high) / 2;
    build(2 * node, low, mid);
    build(2 * node + 1, mid + 1, high);
    seg[node] = max(seg[2 * node], seg[2 * node + 1]);
}

// Update the hotel capacity at a specific index
void update(int node, int low, int high, int idx, int val) {
    if (low == high) {
        h[low] -= val;
        seg[node] = h[low];
        return;
    }
    int mid = (low + high) / 2;
    if (idx <= mid)
        update(2 * node, low, mid, idx, val);
    else
        update(2 * node + 1, mid + 1, high, idx, val);
    seg[node] = max(seg[2 * node], seg[2 * node + 1]);
}

// Find the first index with value >= val
int query(int node, int low, int high, int val) {
    if (seg[node] < val) return 0; // No hotel in this range fits
    if (low == high) return low;
    
    int mid = (low + high) / 2;
    // Always try the left child first to find the first available hotel
    if (seg[2 * node] >= val)
        return query(2 * node, low, mid, val);
    else
        return query(2 * node + 1, mid + 1, high, val);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    for (int i = 1; i <= n; i++) {
        cin >> h[i];
    }

    build(1, 1, n);

    for (int i = 0; i < m; i++) {
        int rooms_needed;
        cin >> rooms_needed;
        
        int hotel_idx = query(1, 1, n, rooms_needed);
        cout << hotel_idx << " ";
        
        if (hotel_idx != 0) {   //as indexing starts from 1
            update(1, 1, n, hotel_idx, rooms_needed);
        }
    }
    cout << endl;

    return 0;
}