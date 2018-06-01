#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <utility>

using namespace std;

struct Node {
  uint idx;
  uint in;
  uint low;
  bool ap;
  bool open;
  set<Node*> nbor;

  Node(uint idx) : idx(idx), in(0), low(0), ap(false), open(false) {}
};

vector<Node*>             NODEVEC;
vector<set<Node*>>        BLOCKVEC;
list<pair<Node*, Node*>>  STACK;
uint                      TIMESTAMP;

void getInput() {
  uint numnode, numedge, x, y;

  cin >> numnode >> numedge;

  NODEVEC.reserve(numnode);

  for(uint i = 0; i < numnode; ++i) NODEVEC.push_back(new Node(i));

  for(uint i = 0; i < numedge; ++i) {
    cin >> x >> y;
    NODEVEC[x]->nbor.insert(NODEVEC[y]);
    NODEVEC[y]->nbor.insert(NODEVEC[x]);
  }
}

void cleanStack(pair<Node*, Node*> edge) {
  set<Node*> nodeset;
  while(true) {
    auto e = STACK.back();
    STACK.pop_back();
    nodeset.insert(e.first);
    nodeset.insert(e.second);
    if(e.first == edge.first and e.second == edge.second) break;
  }
  if(nodeset.size()) BLOCKVEC.push_back(nodeset);
}

void cleanStack() {
  set<Node*> nodeset;
  while(!STACK.empty()) {
    auto e = STACK.back();
    STACK.pop_back();
    nodeset.insert(e.first);
    nodeset.insert(e.second);
  }
  if(nodeset.size()) BLOCKVEC.push_back(nodeset);
}

void findBlocks(Node* v, Node* p, uint stamp) {
  uint cnt = 0;
  if(v->open) return;
  v->open = true;
  v->low = v->in = TIMESTAMP++;
  if(v->nbor.empty()) BLOCKVEC.push_back({v});
  for(Node* w : v->nbor) {
    if(w == p) continue;
    if(!w->open) {
      cnt++;
      STACK.push_back(make_pair(v, w));
      findBlocks(w, v, stamp);
      v->low = min(w->low, v->low);
      if((v->in > stamp and v->in <= w->low) or (cnt > 1 and !v->in)) {
        cleanStack(make_pair(v, w));
//        printf("%d [%d %d] [%d %d]\n", cnt, v->idx, v->in, w->idx, w->low);
        v->ap = true;
      }
    }
    else if(w->in < v->low) {
      v->low = w->in;
      STACK.push_back(make_pair(v, w));
    }
  }
}

void solve() {
  vector<Node*> result;
  Node* pick = nullptr;
  for(auto block : BLOCKVEC) {
    uint  cnt  = 0;
    for(auto node : block) {
      if(node->ap) cnt++;
      else pick = node;
    }
    if(cnt == 1) result.push_back(pick);
  }
  cout << result.size() << endl;
  for(uint i = 0; i < result.size(); i++)
    cout << result[i]->idx << (i < result.size()-1 ? " " : "");
  cout << endl;
  for(auto node : NODEVEC) {
    delete node;
  NODEVEC.clear();
  }
}

void debug() {
  int i = 0;
  for(auto nodes : BLOCKVEC) {
    printf("%2d:", i++);
    for(auto node : nodes) {
      printf(" [%2d |%d]", node->idx, node->ap);
    }
    cout << endl;
  }
}

int main() {
  getInput();
  findBlocks(NODEVEC[0], NODEVEC[0], TIMESTAMP);
  cleanStack();
//  debug();
  solve();
  return 0;
}
