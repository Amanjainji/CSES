// #include <bits/stdc++.h>
// using namespace std;

// struct Node {
//     int mx;                 // maximum height in this segment
//     vector<int> inc;        // increasing (visible) heights
// };

// int n, q;
// vector<int> h;
// vector<Node> seg;

// /* Merge two segment tree nodes */
// Node mergeNode(const Node &L, const Node &R) {
//     if (L.inc.empty()) return R;
//     if (R.inc.empty()) return L;

//     Node res;
//     res.mx = max(L.mx, R.mx);

//     // start with left visible buildings
//     res.inc = L.inc;

//     // add only those from right which are strictly higher
//     int last = res.inc.back();
//     for (int x : R.inc) {
//         if (x > last) {
//             res.inc.push_back(x);
//             last = x;
//         }
//     }
//     return res;
// }

// /* Build segment tree */
// void build(int idx, int l, int r) {
//     if (l == r) {
//         seg[idx].mx = h[l];
//         seg[idx].inc = {h[l]};
//         return;
//     }
//     int mid = (l + r) / 2;
//     build(2 * idx, l, mid);
//     build(2 * idx + 1, mid + 1, r);
//     seg[idx] = mergeNode(seg[2 * idx], seg[2 * idx + 1]);
// }

// /* Query segment tree */
// Node query(int idx, int l, int r, int ql, int qr) {
//     if (r < ql || l > qr) {
//         return Node{0, {}};
//     }
//     if (ql <= l && r <= qr) {
//         return seg[idx];
//     }
//     int mid = (l + r) / 2;
//     Node left = query(2 * idx, l, mid, ql, qr);
//     Node right = query(2 * idx + 1, mid + 1, r, ql, qr);
//     return mergeNode(left, right);
// }

// int main() {
//     ios::sync_with_stdio(false);
//     cin.tie(NULL);

//     cin >> n >> q;
//     h.resize(n + 1);
//     for (int i = 1; i <= n; i++) {
//         cin >> h[i];
//     }

//     seg.resize(4 * n);
//     build(1, 1, n);

//     while (q--) {
//         int a, b;
//         cin >> a >> b;
//         Node ans = query(1, 1, n, a, b);
//         cout << ans.inc.size() << "\n";
//     }

//     return 0;
// }
#include <bits/stdc++.h>
using namespace std;

struct Node {
    int mx;     // maximum height in this segment
    int count;  // pre-calculated: how many visible in Right Child if blocked by Left Child
};

int n, q;
vector<int> h;
vector<Node> seg;

/* Helper function: Calculates how many buildings in the range [l, r] (rooted at idx)
   are visible given that they must be strictly taller than 'limit'.
   Complexity: O(log N)
*/
int calc(int idx, int l, int r, int limit) {
    // Optimization: If the max height here is <= limit, nothing is visible.
    if (seg[idx].mx <= limit) return 0;

    // Base Case: Leaf node
    if (l == r) return seg[idx].mx > limit;

    int mid = (l + r) / 2;

    // Logic: 
    // If the Left Child's max is blocked by 'limit', then the whole Left side is hidden.
    // We only need to check the Right side with the same limit.
    if (seg[2 * idx].mx <= limit) {
        return calc(2 * idx + 1, mid + 1, r, limit);
    }
    
    // If the Left Child is NOT fully blocked:
    // 1. Recurse into Left to find visible buildings there.
    // 2. The Right side is now blocked by the Left Child's max (seg[2*idx].mx).
    //    We already calculated this answer in seg[idx].count
    return calc(2 * idx, l, mid, limit) + seg[idx].count;
}

/* Build segment tree */
void build(int idx, int l, int r) {
    if (l == r) {
        seg[idx].mx = h[l];
        seg[idx].count = 0; // Leaf has no right child
        return;
    }
    int mid = (l + r) / 2;
    build(2 * idx, l, mid);
    build(2 * idx + 1, mid + 1, r);

    // Standard max update
    seg[idx].mx = max(seg[2 * idx].mx, seg[2 * idx + 1].mx);

    // KEY STEP: Pre-calculate the 'count' for this node.
    // "How many in the Right Child are visible if they are blocked by the Left Child's max?"
    seg[idx].count = calc(2 * idx + 1, mid + 1, r, seg[2 * idx].mx);
}

/* Query segment tree */
// We pass 'limit' by reference to update the "max height seen so far" as we move left-to-right
int query(int idx, int l, int r, int ql, int qr, int &limit) {
    if (r < ql || l > qr) {
        return 0;
    }
    
    // Complete overlap
    if (ql <= l && r <= qr) {
        int ans = calc(idx, l, r, limit);
        // Important: Update the limit for the next segments to the right
        limit = max(limit, seg[idx].mx);
        return ans;
    }

    int mid = (l + r) / 2;
    int left_ans = query(2 * idx, l, mid, ql, qr, limit);
    int right_ans = query(2 * idx + 1, mid + 1, r, ql, qr, limit);
    
    return left_ans + right_ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    if (!(cin >> n >> q)) return 0;
    
    h.resize(n + 1);
    for (int i = 1; i <= n; i++) {
        cin >> h[i];
    }

    seg.resize(4 * n + 1);
    build(1, 1, n);

    while (q--) {
        int a, b;
        cin >> a >> b;
        int current_limit = 0; // Initially, nothing blocks our view (height 0)
        cout << query(1, 1, n, a, b, current_limit) << "\n";
    }

    return 0;
}