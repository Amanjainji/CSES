#include <bits/stdc++.h>
using namespace std;

const int MAXN = 1005;

// bit[i][j] stores the sums for the 2D Fenwick Tree
int bit[MAXN][MAXN];
// grid[i][j] stores the current state (1 for tree, 0 for empty)
int grid[MAXN][MAXN];
int n;

// Update function for 2D BIT
// Adds 'val' to element at (y, x)
void update(int y, int x, int val) {
    // Nested loop: outer for rows (y), inner for columns (x)
    for (int i = y; i <= n; i += i & -i) {
        for (int j = x; j <= n; j += j & -j) {
            bit[i][j] += val;
        }
    }
}

// Query function for 2D BIT
// Returns sum of rectangle from (1, 1) to (y, x)
int query(int y, int x) {
    int sum = 0;
    for (int i = y; i > 0; i -= i & -i) {
        for (int j = x; j > 0; j -= j & -j) {
            sum += bit[i][j];
        }
    }
    return sum;
}

// Helper for range sum query using inclusion-exclusion
int query_rect(int y1, int x1, int y2, int x2) {
    return query(y2, x2) 
         - query(y1 - 1, x2) 
         - query(y2, x1 - 1) 
         + query(y1 - 1, x1 - 1);
}

int main() {
    // Optimize I/O operations
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int q;
    if (!(cin >> n >> q)) return 0;

    // Read initial grid
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            char c;
            cin >> c;
            if (c == '*') {
                grid[i][j] = 1;
                update(i, j, 1);
            } else {
                grid[i][j] = 0;
            }
        }
    }

    while (q--) {
        int type;
        cin >> type;
        
        if (type == 1) {
            // Update Operation: Toggle tree/empty
            int y, x;
            cin >> y >> x;
            
            if (grid[y][x] == 1) {
                // Remove tree: Add -1
                update(y, x, -1);
                grid[y][x] = 0;
            } else {
                // Add tree: Add +1
                update(y, x, 1);
                grid[y][x] = 1;
            }
        } else {
            // Query Operation: Count trees in rectangle
            int y1, x1, y2, x2;
            cin >> y1 >> x1 >> y2 >> x2;
            cout << query_rect(y1, x1, y2, x2) << "\n";
        }
    }

    return 0;
}