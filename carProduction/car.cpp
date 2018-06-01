#include <iostream>

using namespace std;

class Node {
  
  public:
  
  bool              flag;
  unsigned int      size;
  unsigned int*     list;
  unsigned long int time, tsum;
  
  Node() : flag(false), size(0), list(nullptr), time(0), tsum(0) {}
  
  ~Node() { clean(); }
  
  void clean() {
    if(size) delete [] list;
  }
  
  void setNode(unsigned int s, unsigned long int t) {
    clean();
    size = s;
    time = t;
    if(size) {
      tsum = 0;
      list = new unsigned int[size];
    }
    else tsum = time;
  }
  
};

Node* NODE;
unsigned int SIZE;

void load() {
  
  cin >> SIZE;
  
  NODE = new Node[SIZE];
  unsigned long int* time = new unsigned long int[SIZE];
  
  for(unsigned int i = 0; i < SIZE; ++i) cin >> time[i];
  
  for(unsigned int i = 0; i < SIZE; ++i) {
    unsigned int s;
    cin >> s;
    NODE[i].setNode(s, time[i]);
    for(unsigned int j = 0; j < s; ++j) cin >> NODE[i].list[j];
  }
  
  delete [] time;
  
}

unsigned long int dfs(Node& x) {
  
  if(x.tsum > 0) return x.tsum;
  if(x.flag) return 0;

  x.flag = true;

  unsigned long int m = 0;
  unsigned long int n = 0;

  for(unsigned int i = 0; i < x.size; ++i) {
    n = dfs(NODE[x.list[i]]);
    if(n < 1) return 0;
    m = max(m, n);
  }

  x.tsum = x.time + m;
  x.flag = false;

  return x.tsum;

}

int main() {
  
  load();
  
  bool flag = true;
  unsigned long int m = 0;
  unsigned long int n = 0;
  
  for(unsigned int i = 0; i < SIZE; ++i) {
    n = dfs(NODE[i]);
    if(n < 1) {
      flag = false;
      break;
    }
    m = max(m, n);
  }
  
  if(flag) {
    cout << m << endl;
    for(unsigned int i = 0; i < SIZE; ++i) cout << (i ? " " : "") << NODE[i].tsum - NODE[i].time;
  }
  else cout << "No solution.";
  
  delete [] NODE;

  return 0;
  
}















































