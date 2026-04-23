class Solution {
  public:
    vector<int> maxOfSubarrays(vector<int>& arr, int k) {
        vector<int> output;
        priority_queue<pair<int,int>> priority;
        
        int n = arr.size();
        int i = 0;
        for(i=0;i<k;i++)
            priority.push({arr[i],i});
        
        while(i<n+1)
        {
            while(!priority.empty() && priority.top().second < i-k)
            priority.pop();
            
            output.push_back(priority.top().first);
            priority.push({arr[i],i});
            i++;
        }
        
    return output;
    }
};
