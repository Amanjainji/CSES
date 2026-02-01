#include <bits/stdc++.h>
using namespace std;

class Node {
public:
    long long sum;
    long long pref;
    long long suff;
    long long ans;

    Node() {
        sum = pref = suff = ans = 0;
    }

    Node(long long val) {
        sum = val;
        pref = max(0LL, val);
        suff = max(0LL, val);
        ans = max(0LL, val);
    }
};

const int MAXN = 200005;
long long a[MAXN];
Node seg[4 * MAXN];

Node merge(Node left, Node right) {
    Node res;
    res.sum = left.sum + right.sum;
    res.pref = max(left.pref, left.sum + right.pref);
    res.suff = max(right.suff, right.sum + left.suff);
    res.ans = max({left.ans, right.ans, left.suff + right.pref});
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

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }

    build(0, 0, n - 1);

    while (m--) {
        int k;
        long long x;
        cin >> k >> x;
        // convert to 0-based index
        update(0, 0, n - 1, k - 1, x);
        
        // The answer is always the max subarray sum of the entire range (Root Node)
        cout << seg[0].ans << "\n";
    }

    return 0;
}