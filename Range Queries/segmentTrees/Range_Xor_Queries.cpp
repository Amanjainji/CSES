#include <bits/stdc++.h>
using namespace std;

int a[200005];
long long seg[4 * 200005];    //always 4n capacity

// Build the segment tree
void build(int ind, int low, int high) {
    if (low == high) {
        seg[ind] = a[low];
        return;
    }

    int mid = (low + high) / 2;
    build(2 * ind + 1, low, mid);
    build(2 * ind + 2, mid + 1, high);

    seg[ind] = seg[2 * ind + 1] ^ seg[2 * ind + 2];
}

long long query(int ind, int low, int high, int l, int r) {
    // complete overlap
    if (low >= l && high <= r) {
        return seg[ind];
    }

    // no overlap
    if (high < l || low > r) {
        return 0;
    }

    // partial overlap
    int mid = (low + high) / 2;
    long long left = query(2 * ind + 1, low, mid, l, r);
    long long right = query(2 * ind + 2, mid + 1, high, l, r);

    return left ^ right;
}

int main() {
    int n;
    cin >> n;
    int q;
    cin >> q;

    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }

    build(0, 0, n - 1);

    while (q--) {
        int l, r;
        cin >> l >> r;
        cout << query(0, 0, n - 1, l-1, r-1) << endl;
    }

    return 0;
}
