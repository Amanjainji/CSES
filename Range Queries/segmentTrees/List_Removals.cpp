#include <iostream>
#include <vector>

using namespace std;

const int MAXN = 200005;
int tree[4 * MAXN];
int arr[MAXN];

// Build the tree where each leaf is 1 (representing one active element)
void build(int node, int start, int end) {
    if (start == end) {
        tree[node] = 1;
        return;
    }
    int mid = (start + end) / 2;
    build(2 * node, start, mid);
    build(2 * node + 1, mid + 1, end);
    tree[node] = tree[2 * node] + tree[2 * node + 1];
}

// Find the k-th active position and mark it as 0 (removed)
int query_and_update(int node, int start, int end, int k) {
    if (start == end) {
        tree[node] = 0; // Mark as removed
        return start;   // Return the original index
    }
    
    int mid = (start + end) / 2;
    int left_count = tree[2 * node];
    int res;
    
    if (k <= left_count) {
        res = query_and_update(2 * node, start, mid, k);
    } else {
        // If the k-th element is in the right child, 
        // we skip all elements in the left child
        res = query_and_update(2 * node + 1, mid + 1, end, k - left_count);
    }
    
    tree[node] = tree[2 * node] + tree[2 * node + 1]; // Update parent sum
    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    for (int i = 1; i <= n; i++) {
        cin >> arr[i];
    }

    build(1, 1, n);

    for (int i = 0; i < n; i++) {
        int p;
        cin >> p;
        int original_idx = query_and_update(1, 1, n, p);
        cout << arr[original_idx] <<" ";
    }
    cout << endl;

    return 0;
}