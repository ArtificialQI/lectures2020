#include <vector>
#include <iostream>
#include <chrono>
#include <functional>
#include <iomanip>
#include <numeric>
#include <map>
#include <fstream>
#include <sstream>

using namespace std;

double mearuse(size_t niter, size_t size, function<vector<size_t>(size_t)> fcn) {
    vector<size_t> vec;
    auto begin = chrono::high_resolution_clock::now();
    for (size_t iteration = 0; iteration < niter; ++iteration) {
        vec = fcn(size);
    }
    auto end = chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::nanoseconds>(end - begin).count() * 1e-9 / niter;
}

vector<size_t> build_vector1(size_t n) {
    vector<size_t> vec;
    for (size_t i = 0; i < n; ++i) {
        vec.insert(vec.begin(), i);
    }
    return vec;
}

vector<size_t> build_vector2(size_t n) {
    vector<size_t> vec;
    vec.reserve(n);
    while (n-- > 0) {
        vec.push_back(n);
    }
    return vec;
}

vector<size_t> build_vector3(size_t n) {
    vector<size_t> vec(n);
    iota(vec.rbegin(), vec.rend(), 0);
    return vec;
}

int main(int argc, char** argv) {
    map<size_t, size_t> config {
        {100, 1e4},
        {1e3, 1e3},
        {1e4, 1e2},
        {1e5, 3},
        {2e5, 1}
    };

    ofstream ofile("1.txt", ofstream::out);

    if (!ofile.good()) {
        cerr << "Can't open file" << endl;
        return 1;
    }

    stringstream ss;

    for (auto [size, niter] : config) {
        ss.str("");
        const double time1 = mearuse(niter, size, build_vector1);
        const double time2 = mearuse(niter, size, build_vector2);
        const double time3 = mearuse(niter, size, build_vector3);
        ss << setw(8) << size << " "
           << scientific << setprecision(3)
           << time1 << " " << time2 << " " << time3 << '\n';
        ofile << ss.str();
        cout << ss.str();
    }
    return 0;
}
