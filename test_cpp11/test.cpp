#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

void test_lower_bound() {
  //map<int, int, greater<int>> m = {{3,1}, {2,2}, {7,3}};
  map<int, int> m = {{1,1}, {3,1}, {2,2}, {7,3}};
  for (auto& it: m) {
    printf("%d -> %d\n", it.first, it.second);
  }
  for (int k=0; k<9; k++) {
    auto it = m.lower_bound(k);
    printf("%d: lower bound: ", k);
    if(it != m.end())
      printf("%d. ", it->first);
    else
      printf("nil. ");
    printf("upper bound: ");
    it = m.upper_bound(k);
    if(it != m.end())
      printf("%d\n", it->first);
	else
      printf("nil\n");
  }
}

int main(int argc, char *argv[]) {
  test_lower_bound();
  return 0;
}
