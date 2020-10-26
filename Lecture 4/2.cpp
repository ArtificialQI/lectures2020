/** Задача: заполнить контейнер случайными целыми числами **/

#include <random>
#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <unordered_map>
#include <functional>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

class RndmInt {
    static mt19937 gen;  // Standard mersenne_twister_engine
    poisson_distribution<> distrib;

 public:
    RndmInt(int mean) : distrib(mean) {}
    int operator()() {
        return distrib(gen);
    }
};

constexpr int genseed = 5;
mt19937 RndmInt::gen(genseed);


/** Measures function execution time. Returns averaged time in seconds **/
template<typename Container>
double measure(size_t niter, size_t nitems, function<Container(size_t)> fcn) {
    Container ctr;

    auto begin = chrono::high_resolution_clock::now();
    for (size_t i = 0; i < niter; ++i) {
        ctr = fcn(nitems);
    }
    cout << "Container size: " << sizeof(ctr) << '\n';
    auto end = chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::nanoseconds>(end - begin).count() * 1e-9 / niter;
}

map<int, size_t> generate_map(size_t nitems) {
    map<int, size_t> m;
    RndmInt rndm(10000);
    for (size_t i = 0; i < nitems; ++i) {
        ++m[rndm()];
    }
    return m;
}

unordered_map<int, size_t> generate_unordered_map(size_t nitems) {
    unordered_map<int, size_t> m;
    RndmInt rndm(10000);
    for (size_t i = 0; i < nitems; ++i) {
        ++m[rndm()];
    }
    return m;
}

vector<int> generate_vector(size_t size) {
    vector<int> v(size);
    RndmInt rndm(10000);
    generate(v.begin(), v.end(), rndm);
    return v;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        cerr << "Usage: ./a.out [logfile]" << endl;
        return 1;
    }
    const string logfile = argv[1];

    map<size_t, size_t> nitems{
        {100, 1e5},
        {1e3, 1e4},
        {1e4, 1e3},
        {1e5, 100},
        {1e6, 10},
        {1e7, 1}
    };

    ofstream ofile(logfile, ofstream::out);
    if (!ofile.good()) {
        cerr << "Can't create log file" << endl;
        return 1;
    }
    stringstream ss;

    for (auto [nitems, iters] : nitems) {
        ofile << nitems << ' ';
        cout << nitems << ' ' << iters << ":\n";
        auto time1 = measure<vector<int>>(10, nitems, generate_vector);
        auto time2 = measure<map<int, size_t>>(10, nitems, generate_map);
        auto time3 = measure<unordered_map<int, size_t>>(10, nitems, generate_unordered_map);
        ss.str("");
        ss << scientific << setprecision(3)
           << time1 << ' '
           << time2 << ' '
           << time3 << ' '
           << '\n';
        cout << ss.str();
        ofile << ss.str();
    }
}
