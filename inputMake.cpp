#include<bits/stdc++.h>
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

int main(){
    random_device rnd;     // 非決定的な乱数生成器
	unsigned long long sd = (unsigned long long)rnd(); 
	Random.set_seed(sd);
    int N = 50;
    int maxHeight = 600, maxWidth = 800;
    cout << N << endl;
    for(int i = 0; i < N; i++){
        cout << Random.randint(maxWidth) << " " << Random.randint(maxHeight) << endl;
    }


    return 0;
}