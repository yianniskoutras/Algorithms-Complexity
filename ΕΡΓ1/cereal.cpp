#include <iostream>
#include <algorithm>

using namespace std;

//this function will help me check if the median is possible or not
bool possible(int arr[], int& N, int& K,
          int& median)
{
    int pre[N];
    for (int i = 0; i < N; i++) {
        if (arr[i] >= median)
            pre[i] = 1;
        else
            pre[i] = -1;

        if (i > 0)
            pre[i] += pre[i - 1];
    }


    int mx = pre[K - 1];

  
    int mn = 0;

    for (int i = K; i < N; i++) {
        mn = min(mn, pre[i - K]);
        mx = max(mx, pre[i] - mn);
    }
    return (mx>0);


}


int maxMedian(int arr[], int N, int K)
{

    int l = 1, r = N + 1;    // l = left boundary , r = right boundary

    int mx_median = -1;

    while (l <= r) {
        int mid = (l + r) / 2;
        if (possible(arr, N, K, mid)) {
            mx_median = mid;
            l = mid + 1;
        }
        else
            r = mid - 1;
    }
    return mx_median;
}


int main() {
    int N, K;
    cin >> N >> K;
    int arr[N];
    int count = 0;
    int elements;
    while(cin >> elements) {
        arr[count] = elements;

        char c;
        cin.get(c);

        if (c != ' ')
            break; // Exit the loop if the character is not a space

        count++;
    }


    cout << maxMedian(arr, N, K) << endl;

    return 0;
}