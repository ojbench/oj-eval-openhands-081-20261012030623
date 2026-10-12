#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <cstring>
using namespace std;

const int MAXN = 3005;

class MaxFlowSolver {
private:
    int n;
    vector<vector<int>> adj;
    vector<vector<bool>> used;
    
    bool find_path(int s, int t, vector<bool>& visited) {
        if (s == t) return true;
        visited[s] = true;
        
        for (size_t i = 0; i < adj[s].size(); i++) {
            int v = adj[s][i];
            if (!visited[v] && !used[s][i]) {
                used[s][i] = true;
                if (find_path(v, t, visited)) {
                    return true;
                }
                used[s][i] = false;
            }
        }
        return false;
    }
    
    int max_flow(int s, int t) {
        int flow = 0;
        // Since each node has degree ≤ 3 and capacity = 1, max flow is at most 3
        for (int attempt = 0; attempt < 3; attempt++) {
            vector<bool> visited(n, false);
            if (find_path(s, t, visited)) {
                flow++;
            } else {
                break;
            }
        }
        return flow;
    }
    
public:
    MaxFlowSolver(int n) : n(n) {
        adj.resize(n);
        used.resize(n);
    }
    
    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
        used[u].push_back(false);
        used[v].push_back(false);
    }
    
    long long solve_all_pairs() {
        long long total = 0;
        
        for (int s = 0; s < n; s++) {
            for (int t = s + 1; t < n; t++) {
                // Reset used flags for this pair
                for (int i = 0; i < n; i++) {
                    fill(used[i].begin(), used[i].end(), false);
                }
                
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
    
    MaxFlowSolver solver(n);
    
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