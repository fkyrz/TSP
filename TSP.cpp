#include <bits/stdc++.h>
using namespace std;

struct xorshift128{
    const unsigned int ini_x = 123456789, ini_y = 362436069, ini_z = 521288629, ini_w = 88675123;
    unsigned int x, y, z, w;
    
    xorshift128() {}

    // シードによってx,y,z,wを初期化 ... initialize x,y,z,w by seed
    void set_seed(unsigned int seed){
        x = ini_x, y = ini_y, z = ini_z, w = ini_w ^ seed;
    }

    unsigned int randint(){
        unsigned int t = x ^ (x << 11);
        x = y;
        y = z;
        z = w;
        return w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
    }

    // [0,r)の範囲の整数で乱数発生 ... generate random integer in [0,r)
    unsigned int randint(unsigned int r){
        assert(r != 0);
        return randint() % r;
    }

    // [l,r)の範囲の整数で乱数発生 ... generate random integer in [l,r)
    unsigned int randint(unsigned int l, unsigned int r){
        assert(r > l);
        return l + randint(r-l);
    }

    // 長さnの順列をランダムに生成し、その前k個分を出力する ... generate a random permutation of size n, and return the first k
    vector<int> randperm(int n, int k){
        assert(k >= 0 && k <= n);
        vector<int> ret(n);
        for(int i = 0; i < n; i++){
            ret[i] = i;
        }
        for(int i = 0; i < k; i++){
            swap(ret[i], ret[randint(i, n)]);
        }
        return vector<int>(ret.begin(), ret.begin() + k);
    }

    // [0,1]の範囲の実数で乱数発生 ... generate random real number in [0,1]
    double uniform(){
        return double(randint()) * 2.3283064370807974e-10;
    }

    // [0,r]の範囲の実数で乱数発生 ... generate random real number in [0,r]
    double uniform(double r){
        assert(r >= 0.0);
        return uniform() * r;
    }

    // [l,r]の範囲の実数で乱数発生 ... generate random real number in [l,r]
    double uniform(double l, double r){
        assert(r >= l);
        return l + uniform(r - l);
    }
};

xorshift128 Random;

const int64_t CYCLES_PER_SEC = 2800000000;

struct Timer {
	int64_t start;
	Timer() { reset(); }
	void reset() { start = getCycle(); }
	void plus(double a) { start -= (a * CYCLES_PER_SEC); }
	inline double get() { return (double)(getCycle() - start) / CYCLES_PER_SEC; }
	inline int64_t getCycle() {
		uint32_t low, high;
		__asm__ volatile ("rdtsc" : "=a" (low), "=d" (high));
		return ((int64_t)low) | ((int64_t)high << 32);
	}
};

Timer timer;

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

    bool twoOpt(){
        int s1 = Random.uniform(N);
        int s2 = Random.uniform(N);
        if(s1 == s2) return false;
        if(path[s1] == s2) return false;
        if(path[s2] == s1) return false;
        int t1 = path[s1];
        int t2 = path[s2];
        double nowDistance = distance(s1, t1) + distance(s2, t2);
        double newDistance = distance(s1, t2) + distance(s2, t1);
        if(newDistance < nowDistance){
            path[s1] = t2;
            path[s2] = t1;
        }
        return true;
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
    ios::sync_with_stdio(0);
	cin.tie(0);
	timer.reset();
	double TIMELIMIT = 2.0;
	random_device rnd;     // 非決定的な乱数生成器
	unsigned long long sd = (unsigned long long)rnd(); 
	Random.set_seed(sd);

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

    // while(timer.get() < TIMELIMIT){
    //     tsp1.twoOpt();
    //     tsp2.twoOpt();
    // }
    inOutput();
    tsp1.output();
    tsp2.output();


    return 0;
}