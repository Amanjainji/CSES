/*
Here is a breakdown of the solution logic for **Polynomial Queries**.

### 1. The Core Difficulty

The problem asks us to add an arithmetic progression  to a range .

* At index L, we add 1
* At index L+1, we add 2
* At index I, we add 1+(i-L)

The difficulty is that the value added depends on **relative position**.

* If you split the range  into two children, the left child sees  but the right child sees something like  depending on where the split happened.
* Tracking this "start value" for every node is tedious and prone to bugs.

### 2. The Mathematical Trick (Global Indexing)

Instead of thinking relatively, let's rewrite the update formula in terms of the **absolute index** `i`.

We need to add:
    {Value}(i) = 1 + (i - L)
    {Value}(i) = i + (1 - L)


This looks like a linear equation: y = A.i + B


**Why is this brilliant?**
Because  and  are constants calculated once at the start. They do **not** change as we push them down the tree. The formula  is valid for *any* index  in the range, whether it's in the left child or the right child.

### 3. How the Segment Tree Works

We maintain two lazy values for every node:

1. `lz_A`: The accumulated  values.
2. `lz_B`: The accumulated  values.

When we want to know the **Total Sum** contributed by these updates for a node covering range `[low, high]`, we calculate:


This splits into two parts:

1. A x {Sum of indices low....high)
2. B x {Count of numbers in range})

#### Helper Function: `sum_indices`

Calculates the sum of integers from `low` to `high`:
    sum = (L + R) * (R - L + 1) / 2;


*/

// ### Solution Code

#include <bits/stdc++.h>
using namespace std;

const int MAXN = 200005;

long long seg[4 * MAXN];
long long lz_A[4 * MAXN]; // Coefficient for 'i'
long long lz_B[4 * MAXN]; // Constant term

// Helper to calculate sum of indices from L to R: L + (L+1) + ... + R
long long sum_indices(long long L, long long R) {
    return (L + R) * (R - L + 1) / 2;
}

// Push lazy updates to children
void push(int ind, int low, int high) {
    if (lz_A[ind] == 0 && lz_B[ind] == 0) return;

    int mid = (low + high) / 2;
    long long A = lz_A[ind];
    long long B = lz_B[ind];

    // --- Apply to Left Child ---
    seg[2 * ind + 1] += A * sum_indices(low, mid) + B * (mid - low + 1);
    lz_A[2 * ind + 1] += A;
    lz_B[2 * ind + 1] += B;

    // --- Apply to Right Child ---
    seg[2 * ind + 2] += A * sum_indices(mid + 1, high) + B * (high - mid);
    lz_A[2 * ind + 2] += A;
    lz_B[2 * ind + 2] += B;

    // --- Reset Current --- 
    lz_A[ind] = 0;
    lz_B[ind] = 0;
}

// Range Update: Add (valA * i + valB) to all i in [l, r]
void update(int ind, int low, int high, int l, int r, long long valA, long long valB) {
    if (low > r || high < l) return;

    if (low >= l && high <= r) {
        seg[ind] += valA * sum_indices(low, high) + valB * (high - low + 1);
        lz_A[ind] += valA;
        lz_B[ind] += valB;
        return;
    }

    push(ind, low, high);
    int mid = (low + high) / 2;
    update(2 * ind + 1, low, mid, l, r, valA, valB);
    update(2 * ind + 2, mid + 1, high, l, r, valA, valB);

    seg[ind] = seg[2 * ind + 1] + seg[2 * ind + 2];
}

long long query(int ind, int low, int high, int l, int r) {
    if (low > r || high < l) return 0;
    
    if (low >= l && high <= r) {
        return seg[ind];
    }

    push(ind, low, high);
    int mid = (low + high) / 2;
    return query(2 * ind + 1, low, mid, l, r) + query(2 * ind + 2, mid + 1, high, l, r);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, q;
    if (!(cin >> n >> q)) return 0;

    // We can just use the update function to build the initial array
    // This simplifies the code significantly.
    for (int i = 0; i < n; i++) {
        int x;
        cin >> x;
        // Update index i with value x: 0*i + x
        // We use 1-based indexing for the tree to match problem statement easily
        update(0, 1, n, i + 1, i + 1, 0, x);
    }

    while (q--) {
        int type;
        cin >> type;
        if (type == 1) {
            int l, r;
            cin >> l >> r;
            // Add sequence 1, 2, 3... to range [l, r]
            // Value at index i is: 1 + (i - l) = i + (1 - l)
            // So A = 1, B = 1 - l
            update(0, 1, n, l, r, 1, 1 - l);
        } else {
            int l, r;
            cin >> l >> r;
            cout << query(0, 1, n, l, r) << "\n";
        }
    }

    return 0;
}

// This problem (**Polynomial Queries**) asks us to perform range updates where the added values form an **Arithmetic Progression (1, 2, 3, ...)** and handle standard range sum queries.

// The Insight: "Global" Linear Functions

// Normally, handling Arithmetic Progressions in segment trees is hard because the term `1` depends on where the query started.
// However, notice that for an update on range [L, R], the value added to index i (where L <= i <= R) is:
//{Value}(i) = 1 + (i - L)
//{Value}(i) = i + (1 - L)
//This is a linear function of the index i:f(i) = A i + B
//Where:A = 1 (Coefficient of i), B = 1 - L (Constant part)

// Because this function is defined on the **absolute index**  (not relative to the current node), we can push these  and  values down the tree without modifying them. If a node covers range , it simply applies the formula  to its range.

// ### Lazy Propagation Logic

// We maintain two lazy values per node:

// 1. `lz_A`: Accumulates the coefficient of .
// 2. `lz_B`: Accumulates the constant term.