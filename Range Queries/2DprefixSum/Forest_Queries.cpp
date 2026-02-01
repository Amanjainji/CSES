#include <bits/stdc++.h>
using namespace std;

int dp[1005][1005];

int main() {
    ios::sync_with_stdio(0); cin.tie(0);
    int n, q;
    cin >> n >> q;

    // 1. Build 2D Prefix Sum Array
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            char c; cin >> c;
            int val = (c == '*') ? 1 : 0;
            // Formula: Current + Top + Left - TopLeft (overlap)
            dp[i][j] = val + dp[i-1][j] + dp[i][j-1] - dp[i-1][j-1];
        }
    }

    // 2. Answer Queries
    while (q--) {
        int y1, x1, y2, x2;
        cin >> y1 >> x1 >> y2 >> x2;
        // Formula: BottomRight - Top - Left + TopLeft
        cout << dp[y2][x2] - dp[y1-1][x2] - dp[y2][x1-1] + dp[y1-1][x1-1] << "\n";
    }
}