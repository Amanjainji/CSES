#include <bits/stdc++.h>
using namespace std;

const int INF = 2e9 + 7;

class Node {
public:
    int min_down; // Stores min(p[i] - i)
    int min_up;   // Stores min(p[i] + i)

    Node() {
        min_down = INF;
        min_up = INF;
    }

    Node(int val, int idx) {
        // idx is the 0-based index (0 to n-1)
        // So distance is |x - i|.
        // Left: p[i] - i + x  -> store p[i] - i
        // Right: p[i] + i - x -> store p[i] + i
        min_down = val - idx;
        min_up = val + idx;
    }
};

const int MAXN = 200005;
int a[MAXN];
Node seg[4 * MAXN];

// Merge is simple: just take the min of both children for both types
Node merge(Node left, Node right) {
    Node res;
    res.min_down = min(left.min_down, right.min_down);
    res.min_up = min(left.min_up, right.min_up);
    return res;
}

void build(int ind, int low, int high) {
    if (low == high) {
        seg[ind] = Node(a[low], low);
        return;
    }
    int mid = (low + high) / 2;
    build(2 * ind + 1, low, mid);
    build(2 * ind + 2, mid + 1, high);
    seg[ind] = merge(seg[2 * ind + 1], seg[2 * ind + 2]);
}

void update(int ind, int low, int high, int pos, int val) {
    if (low == high) {
        seg[ind] = Node(val, low);
        return;
    }
    int mid = (low + high) / 2;
    if (pos <= mid)
        update(2 * ind + 1, low, mid, pos, val);
    else
        update(2 * ind + 2, mid + 1, high, pos, val);

    seg[ind] = merge(seg[2 * ind + 1], seg[2 * ind + 2]);
}

Node query(int ind, int low, int high, int l, int r) {
    if (low >= l && high <= r) {
        return seg[ind];
    }
    if (high < l || low > r) {
        return Node(); 
    }

    int mid = (low + high) / 2;
    Node left = query(2 * ind + 1, low, mid, l, r);
    Node right = query(2 * ind + 2, mid + 1, high, l, r);
    return merge(left, right);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, q;
    if (!(cin >> n >> q)) return 0;

    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }

    build(0, 0, n - 1);

    while (q--) {
        int t;
        cin >> t;
        if (t == 1) { // Update
            int k, x;
            cin >> k >> x;
            update(0, 0, n - 1, k - 1, x);
        } else { // Query
            int k;
            cin >> k;
            int x = k - 1; // 0-based index of customer
            
            // 1. Find min cost from left (including x itself)
            // Range [0, x]: Cost = (p[i] - i) + x
            int left_min = query(0, 0, n - 1, 0, x).min_down + x;

            // 2. Find min cost from right (including x itself)
            // Range [x, n-1]: Cost = (p[i] + i) - x
            int right_min = query(0, 0, n - 1, x, n - 1).min_up - x;

            cout << min(left_min, right_min) << "\n";
        }
    }

    return 0;
}