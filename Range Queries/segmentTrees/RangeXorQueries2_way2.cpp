#include <bits/stdc++.h>
using namespace std;

const int MAX_N = 2e5;

int n, q, arr[MAX_N + 1], prefix[MAX_N + 1], a, b;

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	cin >> n >> q;
	// arr and prefix use 1-based indexing
	for (int i = 1; i <= n; ++i){
        cin >> arr[i]; 
    }
	// Build prefix XOR array
	prefix[1] = arr[1];
	for (int i = 2; i <= n; i++){
        prefix[i] = prefix[i - 1] ^ arr[i]; 
    }

	while (q--) {
		cin >> a >> b;
		cout << (prefix[a - 1] ^ prefix[b]) << endl;
	}
}