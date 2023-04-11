#include <bits/stdc++.h>
using namespace std;

// UnionFind  ex.)UnionFind uf(N);  という風に定義する
// coding: https://youtu.be/TdR816rqc3s?t=726
// comment: https://youtu.be/TdR816rqc3s?t=6822
struct UnionFind {
    vector<int> d;
    UnionFind(int n=0): d(n,-1) {}  //コンストラクタ(n:頂点数)
    int find(int x) {
        if (d[x] < 0) return x;
        return d[x] = find(d[x]);
    }
    bool unite(int x, int y) {
        x = find(x); y = find(y);
        if (x == y) return false;
        if (d[x] > d[y]) swap(x,y);
        d[x] += d[y];
        d[y] = x;
        return true;
    }
    bool same(int x, int y) { return find(x) == find(y);}
    int size(int x) { return -d[find(x)];}
};

int N;

struct place{
    int x, y;
    place(){}
    place(int _x, int _y): x(_x), y(_y) {}
}; 

vector<place> places;

double distance(int i, int j){
    return sqrt(pow(places[i].x - places[j].x, 2) + pow(places[i].y - places[j].y, 2));
}

void inOutput(){
    cout << N << endl;
    for (int i = 0; i < N; i++){
        cout << places[i].x << " " << places[i].y << endl;
    }
    return;
}

struct TSP{
    vector<int> path;

    TSP(){
        path.resize(N, -1);
    }

    void insertion(){
        path[0] = 1;
        path[1] = 0;
        for(int i = 2; i < N; i++){
            int best = 0;
            double bestDist = 1e9;
            int now = 0;
            while(path[now] != 0){
                assert(now != -1);
                double dist = distance(i, now) + distance(i, path[now]) - distance(now, path[now]);
                if(dist < bestDist){
                    bestDist = dist;
                    best = now;
                }
                now = path[now];
            }
            path[i] = path[best];
            path[best] = i;
        }
        return;
    }

    void kraskal(){
        vector<pair<double, pair<int, int>>> edges;
        for(int i = 0; i < N; i++){
            for(int j = i + 1; j < N; j++){
                edges.push_back(make_pair(distance(i, j), make_pair(i, j)));
            }
        }
        sort(edges.begin(), edges.end());
        UnionFind uf(N);
        vector<vector<int>> graph(N);
        for(int i = 0; i < edges.size(); i++){
            int u = edges[i].second.first;
            int v = edges[i].second.second;
            if(uf.find(u) != uf.find(v)){
                uf.unite(u, v);
                graph[u].push_back(v);
                graph[v].push_back(u);
            }
        }

        vector<bool> used(N, false);
        vector<int> order;

        function<void(int)> dfs = [&](int now){
            used[now] = true;
            order.push_back(now);
            for(int i = 0; i < graph[now].size(); i++){
                int next = graph[now][i];
                if(used[next]) continue;
                dfs(next);
            }
        };

        dfs(0);

        for(int i = 0; i < N; i++){
            if(i < N - 1) path[order[i]] = order[i + 1];
            else path[order[i]] = order[0];
        }

        return;
    }

    bool check(){
        set<int> st;
        for(int i = 0; i < N; i++){
            if(path[i] == -1){
                return false;
            }
            st.insert(path[i]);
        }
        if(st.size() != N){
            return false;
        }
        return true;
    }

    double calcAllDist(){
        assert(check());
        double sum = 0;
        for(int i = 0; i < N; i++){
            sum += distance(i, path[i]);
        }
        return sum;
    }

    void output(){
        double pathDist = calcAllDist();
        for (int i = 0; i < N; i++){
            cout << i + 1 << " " << path[i] + 1 << endl;
        }
        return;
    }
};

int main(){
    cin >> N;
    places.resize(N);
    for(int i = 0; i < N; i++){
        int x, y;
        cin >> x >> y;
        places[i] = place(x, y);
    }
    TSP tsp1, tsp2;
    tsp1.insertion();
    tsp2.kraskal();

    inOutput();
    tsp1.output();
    tsp2.output();


    return 0;
}