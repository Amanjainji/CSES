#include <bits/stdc++.h>
using namespace std;

// Use long long to prevent overflow (values can reach ~2e14)
long long seg[4 * 200005];
long long lazy[4 * 200005];
int a[200005];

// Build segment tree
void build(int ind, int low, int high) {
    if (low == high) {
        seg[ind] = a[low];
        return;
    }

    int mid = (low + high) >> 1;
    build(2 * ind + 1, low, mid);
    build(2 * ind + 2, mid + 1, high);

    seg[ind] = seg[2 * ind + 1] + seg[2 * ind + 2];
}

// Range update: add val to range [l, r]
void rangeUpdate(int ind, int low, int high, int l, int r, int val) {

    // Step 1: resolve pending lazy update
    if (lazy[ind] != 0) {
        // Apply pending updates to the current node
        seg[ind] += (long long)(high - low + 1) * lazy[ind];

        // Push lazy value to children if not a leaf
        if (low != high) {
            lazy[2 * ind + 1] += lazy[ind];
            lazy[2 * ind + 2] += lazy[ind];
        }
        lazy[ind] = 0;
    }

    // Step 2: no overlap
    if (r < low || l > high)
        return;

    // Step 3: complete overlap
    if (low >= l && high <= r) {
        // CORRECTION 1: Update sum by (range size * val)
        seg[ind] += (long long)(high - low + 1) * val;

        if (low != high) {
            lazy[2 * ind + 1] += val;
            lazy[2 * ind + 2] += val;
        }
        return;
    }

    // Step 4: partial overlap
    int mid = (low + high) >> 1;
    rangeUpdate(2 * ind + 1, low, mid, l, r, val);
    rangeUpdate(2 * ind + 2, mid + 1, high, l, r, val);

    seg[ind] = seg[2 * ind + 1] + seg[2 * ind + 2];
}

long long query(int ind, int low, int high, int k) {

    // Must resolve lazy even in query to ensure leaf gets updated
    if (lazy[ind] != 0) {
        seg[ind] += (long long)(high - low + 1) * lazy[ind];

        if (low != high) {
            lazy[2 * ind + 1] += lazy[ind];
            lazy[2 * ind + 2] += lazy[ind];
        }
        lazy[ind] = 0;
    }

    if (low == high) {
        return seg[ind];
    }

    int mid = (low + high) >> 1;
    
    if (k <= mid) 
        return query(2 * ind + 1, low, mid, k);
    else 
        return query(2 * ind + 2, mid + 1, high, k);
}

int main() {
    // Optimization for faster I/O (Required for large inputs in CSES)
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin >> n;
    int q;
    cin >> q;
    for (int i = 0; i < n; i++)
        cin >> a[i];

    build(0, 0, n - 1);

    while (q--) {
        int type;
        cin >> type;

        if (type == 1) {
            int l, r, val;
            cin >> l >> r >> val;
            // Convert 1-based input to 0-based indexing
            rangeUpdate(0, 0, n - 1, l - 1, r - 1, val);
        } else {
            int k;
            cin >> k;
            // Convert 1-based input to 0-based indexing
            cout << query(0, 0, n - 1, k - 1) << "\n";
        }
    }
    return 0;
}