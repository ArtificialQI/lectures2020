/** Задача: заполнить массив натуральными числами от N-1 до 0 **/

#include <iostream>
#include <vector>
#include <numeric>
#include <chrono>
#include <functional>
#include <iomanip>
#include <map>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

vector<int> build_vector1(size_t n) {
    vector<int> vec;
    for (size_t i = 0; i < n; ++i) {
        vec.insert(vec.begin(), i);
    }
    return vec;
}

vector<int> build_vector2(size_t n) {
    vector<int> vec;
    for (int i = n-1; i >= 0; --i) {
        vec.push_back(i);
    }
    return vec;
}

vector<int> build_vector3(size_t n) {
    vector<int> vec(n);
    int i = vec.size();
    for (auto& item : vec) {
        item = --i;
    }
    return vec;
}

vector<int> build_vector4(size_t n) {
    vector<int> vec(n);
    iota(vec.rbegin(), vec.rend(), 0);
    return vec;
}

/** Measures function execution time. Returns averaged time in seconds **/
double measure(size_t niter, size_t nitems, function<vector<int>(size_t n)> fcn) {
    vector<int> vec;

    auto begin = chrono::high_resolution_clock::now();
    for (size_t i = 0; i < niter; ++i) {
        vec = fcn(nitems);
    }
    auto end = chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::nanoseconds>(end - begin).count() * 1e-9 / niter;
}


int main(int argc, char** argv) {
    if (argc < 2) {
        cerr << "Usage: ./a.out [logfile]" << endl;
        return 1;
    }
    const string logfile = argv[1];

    ofstream ofile(logfile, ofstream::out);
    if (!ofile.good()) {
        cerr << "Can't create log file" << endl;
        return 1;
    }
    stringstream ss;

    map<size_t, size_t> nitems{
        {100, 1e5},
        {200, 5e4},
        {500, 2e4},
        {1e3, 1e4},
        {2e3, 5e3},
        {5e3, 2e3},
        {1e4, 1e3},
        {5e4, 10},
        {1e5, 3},
        // {2e5, 3},
        // {1e7, 1}
    };

    for (auto [nitems, iters] : nitems) {
        cout << setw(8) << nitems << ' ' << setw(7) << iters << ' ';
        auto time1 = measure(iters, nitems, build_vector1);
        auto time2 = measure(iters, nitems, build_vector2);
        auto time3 = measure(iters, nitems, build_vector3);
        auto time4 = measure(iters, nitems, build_vector4);
        ss.str("");
        ss << scientific << setprecision(3)
           << time1 << ' '
           << time2 << ' '
           << time3 << ' '
           << time4;
        cout << ss.str() << endl;
        ofile << nitems << ' ' << ss.str() << '\n';
    }

    return 0;
}
