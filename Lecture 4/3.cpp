/** Задача: отсортировать вектор строк **/

#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <cctype>
#include <chrono>
#include <set>

using namespace std;

void clean_word(string& s) {
    s.erase(remove_if(s.begin(), s.end(),
        [](unsigned char item) {return ispunct(item);}), s.end());
    for_each(s.begin(), s.end(), [](char& ch) {ch = tolower(ch);});
}

void make_unique_and_shuffle(vector<string>& corpus) {
    set<string> s(corpus.begin(), corpus.end());
    corpus = vector<string>{s.begin(), s.end()};
    random_shuffle(corpus.begin(), corpus.end());
}

vector<string> read_words(const string& path) {
    ifstream ifile(path, ifstream::in);
    if (!ifile.good()) {
        cerr << "Can't load file" << endl;
        return {};
    }
    vector<string> words{
        istream_iterator<string>(ifile),
        istream_iterator<string>()
    };
    
    words.erase(remove_if(words.begin(), words.end(),
        [](const string& item){
            return item.size() < 4 || item.empty();
        }), words.end());
    for_each(words.begin(), words.end(), clean_word);
    
    make_unique_and_shuffle(words);

    return words;
}

double measure(size_t niter, vector<string> corpus, function<void(vector<string>&)> fcn) {
    auto begin = chrono::high_resolution_clock::now();
    for (size_t i = 0; i < niter; ++i) {
        fcn(corpus);
    }
    auto end = chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::nanoseconds>(end - begin).count() * 1e-9 / niter;
}

void quick_sort(vector<string>& corpus) {
    sort(corpus.begin(), corpus.end());
}

void stab_sort(vector<string>& corpus) {
    stable_sort(corpus.begin(), corpus.end());
}

void sort_via_set(vector<string>& corpus) {
    set<string> s(corpus.begin(), corpus.end());
    corpus = vector<string>{s.begin(), s.end()};
}

void sort_with_copy(vector<string>& corpus) {
    sort(corpus.begin(), corpus.end(),
        [](string& a, string& b) {return b < a;});
}

int main() {
    auto words = read_words("shakespeare.txt");
    cout << words.size() << " words" << endl;
    copy(words.begin(), words.begin()+10, ostream_iterator<string>(cout, ", "));
    cout << endl;
    auto time1 = measure(10, words, quick_sort);
    auto time2 = measure(10, words, stab_sort);
    auto time3 = measure(10, words, sort_via_set);
    auto time4 = measure(10, words, sort_with_copy);
    cout << scientific << setprecision(3)
         << time1 << ' '
         << time2 << ' '
         << time3 << ' '
         << time4 << ' '
         << endl;
    return 0;
}
