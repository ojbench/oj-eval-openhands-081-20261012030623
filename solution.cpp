#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <cstring>
using namespace std;

const int MAXN = 3005;
const int MAXM = 4505 * 2;

struct Edge {
    int to, cap, rev;
    Edge(int t, int c, int r) : to(t), cap(c), rev(r) {}
};

class OptimizedDinic {
private:
    vector<Edge> graph[MAXN];
    int level[MAXN];
    int iter[MAXN];
    int n;
    
    void bfs(int s) {
        memset(level, -1, sizeof(level));
        queue<int> q;
        level[s] = 0;
        q.push(s);
        
        while (!q.empty()) {
            int v = q.front(); q.pop();
            for (size_t i = 0; i < graph[v].size(); i++) {
                Edge &e = graph[v][i];
                if (e.cap > 0 && level[e.to] < 0) {
                    level[e.to] = level[v] + 1;
                    q.push(e.to);
                }
            }
        }
    }
    
    int dfs(int v, int t, int f) {
        if (v == t) return f;
        for (int &i = iter[v]; i < (int)graph[v].size(); i++) {
            Edge &e = graph[v][i];
            if (e.cap > 0 && level[v] < level[e.to]) {
                int d = dfs(e.to, t, min(f, e.cap));
                if (d > 0) {
                    e.cap -= d;
                    graph[e.to][e.rev].cap += d;
                    return d;
                }
            }
        }
        return 0;
    }
    
public:
    OptimizedDinic(int n) : n(n) {}
    
    void add_edge(int from, int to, int cap) {
        graph[from].push_back(Edge(to, cap, graph[to].size()));
        graph[to].push_back(Edge(from, 0, graph[from].size() - 1));
    }
    
    int max_flow(int s, int t) {
        int flow = 0;
        while (true) {
            bfs(s);
            if (level[t] < 0) return flow;
            memset(iter, 0, sizeof(iter));
            while (true) {
                int f = dfs(s, t, 1);
                if (f == 0) break;
                flow += f;
                if (flow >= 3) return flow; // Early exit, max flow is at most 3
            }
        }
    }
    
    void clear() {
        for (int i = 0; i < n; i++) {
            graph[i].clear();
        }
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n, m;
    cin >> n >> m;
    
    vector<pair<int, int>> edges;
    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;
        a--; b--; // Convert to 0-based
        edges.push_back({a, b});
    }
    
    long long total = 0;
    
    // Process pairs in batches to reduce memory allocation overhead
    for (int s = 0; s < n; s++) {
        for (int t = s + 1; t < n; t++) {
            OptimizedDinic dinic(n);
            
            // Build flow network
            for (auto& edge : edges) {
                dinic.add_edge(edge.first, edge.second, 1);
                dinic.add_edge(edge.second, edge.first, 1);
            }
            
            int flow = dinic.max_flow(s, t);
            total += flow;
        }
    }
    
    cout << total << endl;
    
    return 0;
}