#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <cstring>
using namespace std;

const int MAXN = 3005;

class GlobalMinCut {
private:
    int n;
    vector<vector<int>> adj;
    vector<pair<int, int>> edges;
    vector<bool> in_component;
    
    // Stoer-Wagner algorithm for global minimum cut
    int stoer_wagner() {
        vector<bool> merged(n, false);
        int min_cut = n; // Initialize with large value
        
        while (true) {
            vector<int> weight(n, 0);
            vector<bool> added(n, false);
            int prev = -1;
            
            for (int i = 0; i < n; i++) {
                if (merged[i]) continue;
                
                // Find most tightly connected vertex
                int sel = -1;
                for (int j = 0; j < n; j++) {
                    if (!merged[j] && !added[j] && (sel == -1 || weight[j] > weight[sel])) {
                        sel = j;
                    }
                }
                
                if (added[sel]) break;
                added[sel] = true;
                
                if (i == n - 1) {
                    // Last vertex added, update min cut
                    min_cut = min(min_cut, weight[sel]);
                    break;
                }
                
                prev = sel;
                // Update weights
                for (int j = 0; j < n; j++) {
                    if (!added[j]) {
                        weight[j] += adj[sel][j];
                    }
                }
            }
            
            if (prev == -1) break;
            
            // Merge prev and last vertex
            merged[prev] = true;
            for (int i = 0; i < n; i++) {
                adj[i][0] += adj[prev][i]; // Merge into first vertex
            }
        }
        
        return min_cut;
    }
    
    // Simple connectivity check using BFS
    bool are_connected(int u, int v) {
        vector<bool> visited(n, false);
        queue<int> q;
        q.push(u);
        visited[u] = true;
        
        while (!q.empty()) {
            int curr = q.front(); q.pop();
            if (curr == v) return true;
            
            for (int next : adj_list[curr]) {
                if (!visited[next]) {
                    visited[next] = true;
                    q.push(next);
                }
            }
        }
        return false;
    }
    
    // Count edge-disjoint paths using BFS-based approach
    int count_disjoint_paths_fast(int s, int t) {
        if (!are_connected(s, t)) return 0;
        
        // Since degree <= 3, we can use a simplified approach
        int flow = 0;
        vector<vector<bool>> edge_used(n);
        
        for (int i = 0; i < n; i++) {
            edge_used[i].resize(adj_list[i].size(), false);
        }
        
        // Try to find up to 3 paths
        for (int attempt = 0; attempt < 3; attempt++) {
            vector<int> parent(n, -1);
            vector<int> edge_idx(n, -1);
            queue<int> q;
            q.push(s);
            parent[s] = s;
            
            bool found = false;
            while (!q.empty() && !found) {
                int u = q.front(); q.pop();
                
                for (size_t i = 0; i < adj_list[u].size(); i++) {
                    int v = adj_list[u][i];
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
            
            if (!found) break;
            
            // Mark edges as used
            int current = t;
            while (current != s) {
                int prev = parent[current];
                int idx = edge_idx[current];
                edge_used[prev][idx] = true;
                
                // Mark reverse edge
                for (size_t i = 0; i < adj_list[current].size(); i++) {
                    if (adj_list[current][i] == prev) {
                        edge_used[current][i] = true;
                        break;
                    }
                }
                
                current = prev;
            }
            
            flow++;
        }
        
        return flow;
    }
    
    vector<vector<int>> adj_list;
    
public:
    GlobalMinCut(int n) : n(n) {
        adj.assign(n, vector<int>(n, 0));
        adj_list.resize(n);
        in_component.resize(n, false);
    }
    
    void add_edge(int u, int v) {
        adj[u][v] = 1;
        adj[v][u] = 1;
        adj_list[u].push_back(v);
        adj_list[v].push_back(u);
        edges.push_back({u, v});
    }
    
    long long solve_all_pairs() {
        long long total = 0;
        
        // For each pair, use the fast disjoint paths counting
        for (int s = 0; s < n; s++) {
            for (int t = s + 1; t < n; t++) {
                int flow = count_disjoint_paths_fast(s, t);
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
    
    GlobalMinCut solver(n);
    
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