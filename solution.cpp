#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <cstring>
using namespace std;

const int MAXN = 3005;

class EfficientMaxFlow {
private:
    int n;
    vector<vector<int>> adj;
    
    // BFS to find shortest path and return distance
    int bfs_distance(int s, int t) {
        vector<int> dist(n, -1);
        queue<int> q;
        dist[s] = 0;
        q.push(s);
        
        while (!q.empty()) {
            int u = q.front(); q.pop();
            if (u == t) return dist[u];
            
            for (int v : adj[u]) {
                if (dist[v] == -1) {
                    dist[v] = dist[u] + 1;
                    q.push(v);
                }
            }
        }
        return -1; // No path
    }
    
    // Check if there are k edge-disjoint paths from s to t
    bool has_k_disjoint_paths(int s, int t, int k) {
        if (k == 0) return true;
        if (k == 1) return bfs_distance(s, t) != -1;
        
        // For k >= 2, we need to check more carefully
        // Since degree <= 3, k can be at most 3
        
        vector<vector<bool>> edge_used(n);
        for (int i = 0; i < n; i++) {
            edge_used[i].resize(adj[i].size(), false);
        }
        
        for (int path = 0; path < k; path++) {
            vector<int> parent(n, -1);
            vector<int> edge_idx(n, -1);
            queue<int> q;
            q.push(s);
            parent[s] = s;
            
            bool found = false;
            while (!q.empty() && !found) {
                int u = q.front(); q.pop();
                
                for (size_t i = 0; i < adj[u].size(); i++) {
                    int v = adj[u][i];
                    if (parent[v] == -1 && !edge_used[u][i]) {
                        parent[v] = u;
                        edge_idx[v] = i;
                        if (v == t) {
                            found = true;
                            break;
                        }
                        q.push(v);
                    }
                }
            }
            
            if (!found) return false;
            
            // Mark edges on this path as used
            int current = t;
            while (current != s) {
                int prev = parent[current];
                int idx = edge_idx[current];
                edge_used[prev][idx] = true;
                
                // Mark reverse edge as used too
                for (size_t i = 0; i < adj[current].size(); i++) {
                    if (adj[current][i] == prev) {
                        edge_used[current][i] = true;
                        break;
                    }
                }
                
                current = prev;
            }
        }
        
        return true;
    }
    
    int max_flow(int s, int t) {
        // Since degree <= 3 and capacity = 1, max flow is at most 3
        for (int k = 3; k >= 0; k--) {
            if (has_k_disjoint_paths(s, t, k)) {
                return k;
            }
        }
        return 0;
    }
    
public:
    EfficientMaxFlow(int n) : n(n) {
        adj.resize(n);
    }
    
    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    
    long long solve_all_pairs() {
        long long total = 0;
        
        for (int s = 0; s < n; s++) {
            for (int t = s + 1; t < n; t++) {
                int flow = max_flow(s, t);
                total += flow;
            }
        }
        
        return total;
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n, m;
    cin >> n >> m;
    
    EfficientMaxFlow solver(n);
    
    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;
        a--; b--; // Convert to 0-based
        solver.add_edge(a, b);
    }
    
    long long result = solver.solve_all_pairs();
    cout << result << endl;
    
    return 0;
}