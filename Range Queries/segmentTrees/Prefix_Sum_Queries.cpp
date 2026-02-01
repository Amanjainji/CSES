#include <bits/stdc++.h>
using namespace std;

class Node {
public:
    long long sum;
    long long pref;

    Node() {
        sum = 0;
        pref = 0; // Empty prefix sum is 0
    }

    // Constructor for leaf nodes
    Node(long long val) {
        sum = val;
        pref = max(0LL, val); // Max prefix is either the value or 0 (empty)
    }
};

const int MAXN = 200005;
long long a[MAXN];
Node seg[4 * MAXN];

// The Merge Logic
Node merge(Node left, Node right) {
    Node res;
    res.sum = left.sum + right.sum;
    res.pref = max(left.pref, left.sum + right.pref);
    return res;
}

void build(int ind, int low, int high) {
    if (low == high) {
        seg[ind] = Node(a[low]);
        return;
    }
    int mid = (low + high) / 2;
    build(2 * ind + 1, low, mid);
    build(2 * ind + 2, mid + 1, high);
    seg[ind] = merge(seg[2 * ind + 1], seg[2 * ind + 2]);
}

void update(int ind, int low, int high, int pos, int val) {
    if (low == high) {
        seg[ind] = Node(val);
        return;
    }
    int mid = (low + high) / 2;
    if (pos <= mid)
        update(2 * ind + 1, low, mid, pos, val);
    else
        update(2 * ind + 2, mid + 1, high, pos, val);

    seg[ind] = merge(seg[2 * ind + 1], seg[2 * ind + 2]);
}

// Query function returns a Node so we can merge the results properly
Node query(int ind, int low, int high, int l, int r) {
    if (low >= l && high <= r) {
        return seg[ind];
    }

    int mid = (low + high) / 2;
    
    // If range is completely on the left
    if (r <= mid) return query(2 * ind + 1, low, mid, l, r);
    
    // If range is completely on the right
    if (l > mid) return query(2 * ind + 2, mid + 1, high, l, r);

    // If range splits, merge both sides
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
            int k, u;
            cin >> k >> u;
            update(0, 0, n - 1, k - 1, u);
        } else { // Query
            int l, r;
            cin >> l >> r;
            // Print the max prefix sum found in the range [l, r]
            cout << query(0, 0, n - 1, l - 1, r - 1).pref << "\n";
        }
    }

    return 0;
}