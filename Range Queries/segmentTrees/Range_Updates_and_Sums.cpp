#include <bits/stdc++.h>
using namespace std;

const int MAXN = 200005;

int a[MAXN];
long long seg[4 * MAXN];
long long lazy_add[4 * MAXN];
long long lazy_set[4 * MAXN];
bool has_set[4 * MAXN]; // Flag to check if there is a pending set update

// Push updates to children
void push(int ind, int low, int high) {
    int mid = (low + high) / 2;

    // 1. Handle SET updates first (they override everything)
    if (has_set[ind]) {
        long long val = lazy_set[ind];

        // Apply to Left Child
        has_set[2 * ind + 1] = true;
        lazy_set[2 * ind + 1] = val;
        lazy_add[2 * ind + 1] = 0; // Set wipes out previous Adds
        seg[2 * ind + 1] = val * (mid - low + 1);

        // Apply to Right Child
        has_set[2 * ind + 2] = true;
        lazy_set[2 * ind + 2] = val;
        lazy_add[2 * ind + 2] = 0; // Set wipes out previous Adds
        seg[2 * ind + 2] = val * (high - mid);

        // Reset current node's set status
        has_set[ind] = false;
        lazy_set[ind] = 0;
    }

    // 2. Handle ADD updates
    if (lazy_add[ind] != 0) {
        long long val = lazy_add[ind];

        // Apply to Left Child
        if (has_set[2 * ind + 1]) {
            // If child has a pending SET, just add to that set value
            lazy_set[2 * ind + 1] += val;
        } else {
            // Otherwise, accumulate in lazy_add
            lazy_add[2 * ind + 1] += val;
        }
        seg[2 * ind + 1] += val * (mid - low + 1);

        // Apply to Right Child
        if (has_set[2 * ind + 2]) {
            lazy_set[2 * ind + 2] += val;
        } else {
            lazy_add[2 * ind + 2] += val;
        }
        seg[2 * ind + 2] += val * (high - mid);

        // Reset current node's add status
        lazy_add[ind] = 0;
    }
}

// Build the tree
void build(int ind, int low, int high) {
    if (low == high) {
        seg[ind] = a[low];
        return;
    }
    int mid = (low + high) / 2;
    build(2 * ind + 1, low, mid);
    build(2 * ind + 2, mid + 1, high);
    seg[ind] = seg[2 * ind + 1] + seg[2 * ind + 2];
}

void update_add(int ind, int low, int high, int l, int r, int val) {
    if (low > r || high < l) return;

    if (low >= l && high <= r) {
        if (has_set[ind]) {
            lazy_set[ind] += val;
        } else {
            lazy_add[ind] += val;
        }
        seg[ind] += (long long)val * (high - low + 1);
        return;
    }

    push(ind, low, high);
    int mid = (low + high) / 2;
    update_add(2 * ind + 1, low, mid, l, r, val);
    update_add(2 * ind + 2, mid + 1, high, l, r, val);
    seg[ind] = seg[2 * ind + 1] + seg[2 * ind + 2];
}

void update_set(int ind, int low, int high, int l, int r, int val) {
    if (low > r || high < l) return;

    if (low >= l && high <= r) {
        has_set[ind] = true;
        lazy_set[ind] = val;
        lazy_add[ind] = 0; // Clear any pending adds
        seg[ind] = (long long)val * (high - low + 1);
        return;
    }

    push(ind, low, high);
    int mid = (low + high) / 2;
    update_set(2 * ind + 1, low, mid, l, r, val);
    update_set(2 * ind + 2, mid + 1, high, l, r, val);
    seg[ind] = seg[2 * ind + 1] + seg[2 * ind + 2];
}

long long query(int ind, int low, int high, int l, int r) {
    if (low > r || high < l) return 0;

    if (low >= l && high <= r) {
        return seg[ind];
    }

    push(ind, low, high);
    int mid = (low + high) / 2;
    return query(2 * ind + 1, low, mid, l, r) + 
           query(2 * ind + 2, mid + 1, high, l, r);
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
        int type;
        cin >> type;
        if (type == 1) {
            int l, r, x;
            cin >> l >> r >> x;
            update_add(0, 0, n - 1, l - 1, r - 1, x);
        } else if (type == 2) {
            int l, r, x;
            cin >> l >> r >> x;
            update_set(0, 0, n - 1, l - 1, r - 1, x);
        } else {
            int l, r;
            cin >> l >> r;
            cout << query(0, 0, n - 1, l - 1, r - 1) << "\n";
        }
    }

    return 0;
}