class DisjointSet {
    vector<int> parent, rank;
    
public:
    DisjointSet(int n) {
        parent.resize(n);
        rank.resize(n, 0);
        
        for(int i = 0; i < n; i++)
            parent[i] = i;
    }
    
    int findParent(int node) {
        if(parent[node] == node)
            return node;
        return parent[node] = findParent(parent[node]);
    }
    
    void unionSet(int u, int v) {
        u = findParent(u);
        v = findParent(v);
        
        if(u == v) return;
        
        if(rank[u] < rank[v])
            parent[u] = v;
        else if(rank[v] < rank[u])
            parent[v] = u;
        else {
            parent[v] = u;
            rank[u]++;
        }
    }
};
class Solution {
public:
   int kruskalsMST(int V, vector<vector<int>>& edges) {
        
        
        sort(edges.begin(), edges.end(), [](vector<int> &a, vector<int> &b){
            return a[2] < b[2];
        });
        
        DisjointSet ds(V);
        int mstSum = 0;
        
        
        for(auto &edge : edges) {
            int u = edge[0];
            int v = edge[1];
            int wt = edge[2];
            
           
            if(ds.findParent(u) != ds.findParent(v)) {
                mstSum += wt;
                ds.unionSet(u, v);
            }
        }
        
        return mstSum;
    }
};
