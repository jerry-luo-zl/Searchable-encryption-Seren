#include "Core/SSEClient.h"
#include "TimeRecord.h"
#include <chrono>
#include <iostream>
#include <sys/time.h>
#include <vector>
using namespace std;
using namespace chrono;

vector<int> inds;
vector<int> res;

int main() {
  SSEClient client;

  // 进行测试的 kv-pairs 数目
  vector<int> benchmark({100, 1000, 10000, 100000});

  for (int i = 0; i < benchmark.size(); i++) {
    cout << endl;
    cout << "============== " << benchmark[i] << " ==============" << endl;

    for (int j = 0; j < benchmark[i]; ++j) {
      inds.emplace_back(j);
    }

    // insert test
    T0 = high_resolution_clock::now();
    client.update(INS, "test", inds);
    T1 = high_resolution_clock::now();
    cout << "Insert time : " << duration_cast<microseconds>(T1 - T0).count()
         << endl;

    // delete test
    for (int j = 0; j < benchmark[i] / 2; ++j) {
      inds.pop_back();
    }

    T3 = high_resolution_clock::now();
    client.update(DEL, "test", inds);
    T4 = high_resolution_clock::now();
    cout << "Delete time : " << duration_cast<microseconds>(T4 - T3).count()
         << endl;

    T5 = high_resolution_clock::now();
    client.search(res, "test");
    T6 = high_resolution_clock::now();
    cout << "search time : " << duration_cast<microseconds>(T6 - T5).count()
         << endl;

    // cout << "search results : ";
    // for (auto r : res) {
    //     cout << r << " ";
    // }
  }
  return 0;
}
