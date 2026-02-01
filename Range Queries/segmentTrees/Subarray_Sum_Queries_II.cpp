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

Node query(int ind, int low, int high, int l, int h) {
    //complete overlap
    if (low>=l && high<=h) {
        return seg[ind];
    }
    //no overlap
    if (low>h || high<l) {
        return Node();
    }

    //partial overlap
    int mid = (low + high) / 2;
    Node left=query(2 * ind + 1, low, mid, l, h);
    Node right=query(2 * ind + 2, mid+1, high, l, h);
    
    return merge(left,right);
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
        int a,b;
        cin >> a >> b;
        cout << query(0,0,n-1,a-1,b-1).ans << "\n";
    }

    return 0;
}