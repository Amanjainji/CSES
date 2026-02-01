#include <bits/stdc++.h>
using namespace std;

const int MAXN = 200005;
int a[MAXN];
vector<int> seg[4 * MAXN];

// Build: Merge sorted vectors from children
void build(int node, int low, int high) {
    if (low == high) {
        seg[node].push_back(a[low]);
        return;
    }
    int mid = (low + high) / 2;
    build(2 * node, low, mid);
    build(2 * node + 1, mid + 1, high);

    // Standard merge operation (like Merge Sort)
    // Merges two sorted arrays into a new sorted array
    merge(seg[2 * node].begin(), seg[2 * node].end(),
          seg[2 * node + 1].begin(), seg[2 * node + 1].end(),
          back_inserter(seg[node]));
}

int query(int node, int low, int high, int l, int r, int k1, int k2) {
    // 1. no overlap
    if (high < l || low > r) {
        return 0;
    }

    // 2. complete overlap
    if (l <= low && high <= r) {
        // upper_bound returns iterator to first element > k2
        // lower_bound returns iterator to first element >= k1
        // The distance between them is the count of elements in [k1, k2]
        auto ub = upper_bound(seg[node].begin(), seg[node].end(), k2);
        auto lb = lower_bound(seg[node].begin(), seg[node].end(), k1);
        return ub - lb;
    }

    // 3. Partial overlap
    int mid = (low + high) / 2;
    return query(2 * node, low, mid, l, r, k1, k2) +
           query(2 * node + 1, mid + 1, high, l, r, k1, k2);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, q;
    if (!(cin >> n >> q)) return 0;

    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }

    build(1, 0, n - 1);

    while (q--) {
        int l, r, c, d;
        cin >> l >> r >> c >> d;
        // Convert 1-based index to 0-based
        cout << query(1, 0, n - 1, l - 1, r - 1, c, d) << "\n";
    }

    return 0;
}

// #include <bits/stdc++.h>
// using namespace std;

// const int MAXN = 200005;

// // Fenwick Tree (Binary Indexed Tree)
// int bit[MAXN];
// int max_val;

// void update(int idx, int val) {
//     for (; idx <= max_val; idx += idx & -idx)
//         bit[idx] += val;
// }

// int query(int idx) {
//     int sum = 0;
//     for (; idx > 0; idx -= idx & -idx)
//         sum += bit[idx];
//     return sum;
// }

// // Struct to store query parts
// struct QueryPart {
//     int id;      // Original query index
//     int c_rank;  // Compressed lower bound value
//     int d_rank;  // Compressed upper bound value
//     int sign;    // +1 for R, -1 for L-1
// };

// int a[MAXN];
// int ans[MAXN];
// vector<QueryPart> queries_at[MAXN]; // Buckets to store queries at specific indices

// int main() {
//     ios_base::sync_with_stdio(false);
//     cin.tie(NULL);

//     int n, q;
//     if (!(cin >> n >> q)) return 0;

//     // 1. Read Input and Coordinate Compress Values
//     // We need coordinate compression because values of a[i] might be up to 10^9
//     vector<int> vals;
//     for (int i = 1; i <= n; i++) {
//         cin >> a[i];
//         vals.push_back(a[i]);
//     }
    
//     // Sort and remove duplicates to map values to ranks 1..N
//     sort(vals.begin(), vals.end());
//     vals.erase(unique(vals.begin(), vals.end()), vals.end());
//     max_val = vals.size();

//     auto get_rank = [&](int v) {
//         return lower_bound(vals.begin(), vals.end(), v) - vals.begin() + 1;
//     };

//     // 2. Process Queries Offline
//     for (int i = 0; i < q; i++) {
//         int l, r, c, d;
//         cin >> l >> r >> c >> d;
        
//         // Find compressed ranks for the value range [c, d]
//         // c_rank: first index >= c
//         // d_rank: last index <= d (which is first index > d, minus 1)
//         int c_rank = lower_bound(vals.begin(), vals.end(), c) - vals.begin() + 1;
//         int d_rank = upper_bound(vals.begin(), vals.end(), d) - vals.begin(); 
        
//         // If the range is invalid (c > max element or d < min element), answer is 0
//         if (c_rank > d_rank) {
//             ans[i] = 0;
//             continue;
//         }

//         // Store two events: 
//         // 1. At L-1: subtract the count found so far
//         if (l > 1) {
//             queries_at[l - 1].push_back({i, c_rank, d_rank, -1});
//         }
//         // 2. At R: add the count found so far
//         queries_at[r].push_back({i, c_rank, d_rank, 1});
//     }

//     for (int i = 1; i <= n; i++) {
//         // Add current element a[i] to the BIT
//         int rank = get_rank(a[i]);
//         update(rank, 1);

//         // Process all queries that end or start here
//         for (auto &qp : queries_at[i]) {
//             int count_in_range = query(qp.d_rank) - query(qp.c_rank - 1);
//             ans[qp.id] += qp.sign * count_in_range;
//         }
//     }

//     // 4. Output results
//     for (int i = 0; i < q; i++) {
//         cout << ans[i] << "\n";
//     }

//     return 0;
// }