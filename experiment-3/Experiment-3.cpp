#include <bits/stdc++.h>
using namespace std;
int main() {
    int m;
    cin >> m;
    vector<char> arr(m);
    for(int i = 0; i < m; i++) {
        cin >> arr[i];
    }
    unordered_map<int, int> mp;
    int sum = 0, maxLen = 0;
    mp[0]=-1;
    for(int i = 0; i < m; i++) {
        if(arr[i] == 'P')
            sum += 1;
        else if(arr[i] == 'A')
            sum -= 1;

        if(mp.find(sum) != mp.end()) {
            maxLen = max(maxLen, i - mp[sum]);
        } else {
            mp[sum] = i;
        }
    }
    cout <<"Ans="<< maxLen << endl;
    return 0;
}
