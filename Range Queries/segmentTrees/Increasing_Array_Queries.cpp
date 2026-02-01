#include <bits/stdc++.h>
using namespace std;

const int MAXN = 200005;

// User variables
int a[MAXN];
long long seg[4 * MAXN]; 
long long lazy[4 * MAXN]; // Added for Lazy Propagation

// Helper to calculate prefix sums of the original array
long long pref[MAXN];

// Push lazy updates to children
void push(int ind, int low, int high) {
    if (lazy[ind] != 0) {
        int mid = (low + high) / 2;
        
        // Apply to left child
        lazy[2 * ind + 1] = lazy[ind];
        seg[2 * ind + 1] = lazy[ind] * (mid - low + 1);
        
        // Apply to right child
        lazy[2 * ind + 2] = lazy[ind];
        seg[2 * ind + 2] = lazy[ind] * (high - mid);
        
        // Reset current lazy
        lazy[ind] = 0;
    }
}

// Range Set Update: set values in range [l, r] to val
void update(int ind, int low, int high, int l, int r, int val) {
    //no overlap
    if (low > r || high < l) return;

    // Complete overlap
    if (low >= l && high <= r) {
        lazy[ind] = val;
        seg[ind] = (long long)val * (high - low + 1);
        return;
    }

    push(ind, low, high); // Push before going down
    int mid = (low + high) / 2;
    update(2 * ind + 1, low, mid, l, r, val);
    update(2 * ind + 2, mid + 1, high, l, r, val);

    seg[ind] = seg[2 * ind + 1] + seg[2 * ind + 2];
}

long long query(int ind, int low, int high, int l, int r) {
    if (low > r || high < l) return 0;

    if (low >= l && high <= r) {
        return seg[ind];
    }

    push(ind, low, high); // Push before going down
    int mid = (low + high) / 2;
    long long left = query(2 * ind + 1, low, mid, l, r);
    long long right = query(2 * ind + 2, mid + 1, high, l, r);

    return left + right;
}

// Store queries to process offline: queries[L] -> {R, query_index
vector<pair<int, int>> queries[MAXN];
long long answers[MAXN];

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, q;
    if (!(cin >> n >> q)) return 0;

    // 1-based prefix sum for O(1) original sum calculation
    pref[0] = 0;
    for (int i = 0; i < n; i++) {
        cin >> a[i];
        pref[i + 1] = pref[i] + a[i];
    }

    for (int i = 0; i < q; i++) {
        int l, r;
        cin >> l >> r;
        // Store queries by left index (0-based)
        queries[l - 1].push_back({r - 1, i});
    }

    // Monotonic stack to find the next element >= a[i]
    stack<int> s;

    // Process from right to left
    for (int i = n - 1; i >= 0; i--) {
        // Pop elements that are smaller than current a[i]
        // because a[i] will cover/flatten them
        while (!s.empty() && a[s.top()] < a[i]) {
            s.pop();
        }

        // Determine the range [i, limit] that a[i] flattens
        int limit = s.empty() ? n - 1 : s.top() - 1;
        
        // Set the range [i, limit] to value a[i] in the segment tree
        update(0, 0, n - 1, i, limit, a[i]);
        s.push(i);

        // Answer all queries starting at this index i
        for (auto &qry : queries[i]) {
            int r = qry.first;
            int idx = qry.second;
            
            long long current_sum = query(0, 0, n - 1, i, r);
            long long original_sum = pref[r + 1] - pref[i];
            
            answers[idx] = current_sum - original_sum;  //The problem asks for the "number of operations," which is Sum(Modified Array) - Sum(Original Array).The Segment Tree tracks the Sum(Modified Array).The pref array lets us calculate Sum(Original Array) in O(1) time.
        }
    }

    for (int i = 0; i < q; i++) {
        cout << answers[i] << endl;
    }

    return 0;
}