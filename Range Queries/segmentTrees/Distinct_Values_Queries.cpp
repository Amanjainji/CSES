#include <bits/stdc++.h>
using namespace std;

const int MAXN = 200005;
int a[MAXN];
int seg[4 * MAXN]; // Segment tree array
int last_pos[MAXN]; // Tracks the last index of a value
int n, q;

// 1. Update function: Point Update
// Sets the value at 'idx' to 'val' (either 0 or 1)
void update(int node, int start, int end, int idx, int val) {
    if (start == end) {
        seg[node] = val;
        return;
    }
    int mid = (start + end) / 2;
    if (idx <= mid)
        update(2 * node, start, mid, idx, val);
    else
        update(2 * node + 1, mid + 1, end, idx, val);
    
    seg[node] = seg[2 * node] + seg[2 * node + 1];
}

// 2. Query function: Range Sum
// Returns the sum in range [l, r]
int query(int node, int start, int end, int l, int r) {
    if (r < start || end < l) return 0; // Out of range
    if (l <= start && end <= r) return seg[node]; // Complete overlap

    int mid = (start + end) / 2;
    int p1 = query(2 * node, start, mid, l, r);
    int p2 = query(2 * node + 1, mid + 1, end, l, r);
    return p1 + p2;
}

struct Query {
    int l, r, id;
};

// Sort queries by Right endpoint
bool compareQueries(const Query& a, const Query& b) {
    return a.r < b.r;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    if (!(cin >> n >> q)) return 0;

    // Coordinate Compression Logic
    // We map the raw values to range [0, n] to use them as indices in last_pos
    vector<int> raw_vals;
    vector<int> original_a(n);
    for (int i = 0; i < n; i++) {
        cin >> original_a[i];
        raw_vals.push_back(original_a[i]);
    }

    sort(raw_vals.begin(), raw_vals.end());
    raw_vals.erase(unique(raw_vals.begin(), raw_vals.end()), raw_vals.end());

    for (int i = 0; i < n; i++) {
        // Replace value with its rank (0 to distinct_count-1)
        a[i] = lower_bound(raw_vals.begin(), raw_vals.end(), original_a[i]) - raw_vals.begin();
    }

    // Initialize last_pos with -1
    memset(last_pos, -1, sizeof(last_pos));

    // Store queries
    vector<Query> queries(q);
    for (int i = 0; i < q; i++) {
        cin >> queries[i].l >> queries[i].r;
        queries[i].l--; // Convert to 0-based
        queries[i].r--; // Convert to 0-based
        queries[i].id = i;
    }

    sort(queries.begin(), queries.end(), compareQueries);

    vector<int> ans(q);
    int q_idx = 0;

    // 3. Sweep Line
    for (int i = 0; i < n; i++) {
        int val = a[i];

        // If seen before, turn off the previous occurrence
        if (last_pos[val] != -1) {
            update(1, 0, n - 1, last_pos[val], 0);
        }

        // Turn on the current occurrence
        update(1, 0, n - 1, i, 1);
        last_pos[val] = i;

        // Answer all queries ending at this position
        while (q_idx < q && queries[q_idx].r == i) {
            ans[queries[q_idx].id] = query(1, 0, n - 1, queries[q_idx].l, queries[q_idx].r);
            q_idx++;
        }
    }

    for (int i = 0; i < q; i++) {
        cout << ans[i] << endl;
    }

    return 0;
}

// #include <bits/stdc++.h>
// using namespace std;

// int a[200005];
// unordered_set<int> seg[4 * 200005];

// // Build the segment seg
// void build(int ind, int low, int high) {
//     if (low == high) {
//         unordered_set<int> st;
//         st.insert(a[low]);
//         seg[ind] = st;
//         return;
//     }

//     int mid = (low + high) / 2;
//     build(2 * ind + 1, low, mid);
//     build(2 * ind + 2, mid + 1, high);

//     unordered_set<int> st;
//     for(auto i: seg[2*ind+1]){
//         st.insert(i);
//     }
//     for(auto i: seg[2*ind+2]){
//         st.insert(i);
//     }
//     seg[ind] = st;
// }

// unordered_set<int> query(int ind, int low, int high, int l, int r) {
//     // complete overlap
//     if (low >= l && high <= r) {
//         return seg[ind];
//     }

//     // no overlap
//     if (high < l || low > r) {
//         return {};
//     }

//     // partial overlap
//     int mid = (low + high) / 2;
//     unordered_set<int> left = query(2 * ind + 1, low, mid, l, r);
//     unordered_set<int> right = query(2 * ind + 2, mid + 1, high, l, r);

//     unordered_set<int> st;
//     for(auto i: left){
//         st.insert(i);
//     }
//     for(auto i: right){
//         st.insert(i);
//     }
//     return st;
// }

// int main() {
//     int n;
//     cin >> n;
//     int q;
//     cin >> q;

//     for (int i = 0; i < n; i++) {
//         cin >> a[i];
//     }

//     build(0, 0, n - 1);

//     while (q--) {
//         int l, r;
//         cin >> l >> r;
//         cout << query(0, 0, n - 1, l-1, r-1).size() << endl;
//     }

//     return 0;
// }

