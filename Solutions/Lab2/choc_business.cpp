#include <iostream>
#include <vector>
#include <climits>

using namespace std;

int maxProfit(vector<int>& costs, int k) {
    int n = costs.size();

    if (n == 0 || k == 0) {
        return 0;
    }

    vector<int> profit(k + 1, 0);
    vector<int> prevDiff(k + 1, INT_MIN);

    
    for (int j = 0; j < n; j++) {
        for (int i = 1; i <= k; i++) {
            prevDiff[i] = max(prevDiff[i], profit[i - 1] - costs[j]);
            profit[i] = max(profit[i], costs[j] + prevDiff[i]);
        }
    }

    return profit[k];
}

int main() {
    int n, k;
    cin >> n >> k;

    vector<int> costs(n);
    for (int i = 0; i < n; i++) {
        cin >> costs[i];
    }

    cout << maxProfit(costs, k) << endl;

    return 0;
}
