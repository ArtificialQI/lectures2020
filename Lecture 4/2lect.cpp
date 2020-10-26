#include <vector>
#include <random>
#include <iostream>
#include <algorithm>

using namespace std;

class Rndm {
    static mt19937 gen;
    poisson_distribution<> distrib;

 public:
    Rndm(int mean) : distrib(mean) {}
    int operator()() {
        return distrib(gen);
    }
};

mt19937 Rndm::gen(5);

vector<int> generate_vector(int mean, size_t size) {
    vector<int> vec(size);
    generate(vec.begin(), vec.end(), Rndm(1000));
    return vec;
}

int main() {
    Rndm gen(1000);

    for (size_t i = 0; i < 10; ++i) {
        cout << gen() << ' ';
    }
    cout << endl;
    return 0;
}

