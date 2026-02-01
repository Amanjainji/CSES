#include <iostream>
#include <vector>

using namespace std;

const int MAX_VAL = 1e9;

// l_child and r_child store indices in the 'seg' vector
struct Node {
    int sum;
    int l_child;
    int r_child;
};

const int MAX_NODES = 12000000;
Node seg[MAX_NODES];
int cnt = 1; // Allocator counter, start from 1 (0 is null)

// Update function: adds val to index idx
// node_idx: current node index in 'seg' array
// l, r: current range covered by this node
void update(int node_idx, int l, int r, int idx, int val) {
    if (l == r) {
        seg[node_idx].sum += val;
        return;
    }

    int mid = l + (r - l) / 2;
    
    if (idx <= mid) {
        if (seg[node_idx].l_child == 0) {
            seg[node_idx].l_child = ++cnt;
        }
        update(seg[node_idx].l_child, l, mid, idx, val);
    } else {
        if (seg[node_idx].r_child == 0) {
            seg[node_idx].r_child = ++cnt;
        }
        update(seg[node_idx].r_child, mid + 1, r, idx, val);
    }

    int left_sum = (seg[node_idx].l_child == 0) ? 0 : seg[seg[node_idx].l_child].sum;
    int right_sum = (seg[node_idx].r_child == 0) ? 0 : seg[seg[node_idx].r_child].sum;
    seg[node_idx].sum = left_sum + right_sum;
}

int query(int node_idx, int l, int r, int q_l, int q_r) {
    // If current node doesn't exist or range is invalid or no overlap
    if (node_idx == 0 || l > q_r || r < q_l) {
        return 0;
    }

    // complete overlap
    if (l >= q_l && r <= q_r) {
        return seg[node_idx].sum;
    }

    int mid = l + (r - l) / 2;
    return query(seg[node_idx].l_child, l, mid, q_l, q_r) +
           query(seg[node_idx].r_child, mid + 1, r, q_l, q_r);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, q;
    if (!(cin >> n >> q)) return 0;

    vector<int> p(n + 1);
    
    // Root is always node 1, covering range [1, 10^9]
    int root = 1;

    for (int i = 1; i <= n; i++) {
        cin >> p[i];
        update(root, 1, MAX_VAL, p[i], 1);
    }

    // Process queries
    for (int i = 0; i < q; i++) {
        char type;
        cin >> type;
        if (type == '!') {
            int k, x;
            cin >> k >> x;
            
            // Remove old salary
            update(root, 1, MAX_VAL, p[k], -1);
            
            // Update local array
            p[k] = x;
            
            // Add new salary
            update(root, 1, MAX_VAL, p[k], 1);
        } else {
            int a, b;
            cin >> a >> b;
            cout << query(root, 1, MAX_VAL, a, b) << "\n";
        }
    }

    return 0;
}