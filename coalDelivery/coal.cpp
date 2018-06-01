#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>
#include <queue>
#include <list>
#include <set>
#include <map>

using namespace std;

set<int>              srce;
set<int>              sink;
vector<set<int>>      edge;
vector<map<int, int>> zone;

vector<set<int>>      open_edge;

void debug() {
  /* SHOW INPUT */
  printf("\nSOURCE:\t");
  for(int i : srce) printf("%2d ", i);
  printf("\n\nSINK  :\t");
  for(int i : sink) printf("%2d ", i);
  printf("\n\nEDGE  :\t");
  for(int i = 0; i < (int)edge.size(); ++i) {
    printf("%2d > ", i);
    for(int j : edge[i])
      printf("%2d ", j);
    printf("\n\t");
  }

  /* SHOW ZONE */
  printf("\nZONE  :");
  for(uint i = 0; i < zone.size(); ++i) {
    printf("\t%2d > ", i);
    for(auto z : zone[i])
      printf("[%2d|%2d] ", z.first, z.second);
    printf("\n");
  }
  printf("\n");
}

void getInput() {
  int num_node, num_edge, num_sink;
  cin >> num_node >> num_edge >> num_sink;

  for(int i = 0; i < 2; ++i)
    for(int j = 0; j < num_sink; ++j) {
      int node;
      cin >> node;
      i ? sink.insert(node) : srce.insert(node);
    }

  for(int i = 0; i < num_node; ++i) {
    edge.push_back(set<int>());
    edge[i].insert(i);
  }

  for(int i = 0; i < (int)num_edge; ++i) {
    int node_a, node_b;
    cin >> node_a >> node_b;
    edge[node_a].insert(node_b);
  }

  open_edge = edge;
}

void initZone() {
  map<int, int> node;
  for(int i : srce)
    node[i] = -2;
  zone.push_back(node);
}

void buildZone() {
  bool disc = false;

  set<int> sink_set = sink;
  for(int s : srce)
    sink_set.erase(s);

  if(zone.empty()) {
    initZone();
    if(sink_set.empty()) return;
  }

  for(uint i = zone.size()-1; i < zone.size(); ++i) {
    disc = false;
    auto nodes = zone[i];
    for(auto n : zone[i]) {
      if(n.second != -2) nodes[n.first] = n.first;
      for(int nbr : edge[n.first]) {
        if(zone[i].find(nbr) == zone[i].end()) nodes[nbr] = -2;
        sink_set.erase(nbr);
        disc |= open_edge[n.first].erase(nbr);
      }
    }
    zone.push_back(nodes);
    if(sink_set.empty() || !disc) return;
  }
}

void addQue(bool way, int zone_idx, int node_idx, queue<vector<pair<int, int>>>& que, set<pair<bool, pair<int, int>>>& closed) {
  if(zone_idx < 0 || zone_idx >= (int)zone.size()) return;
  pair<int, int> node(zone_idx, node_idx);
  pair<bool, pair<int, int>> ext_node(way, node);
  if(closed.find(ext_node) != closed.end()) return;
  closed.insert(ext_node);
  auto path = que.front();
  path.push_back(node);
  que.push(path);
}

void buildPath(vector<pair<int, int>>& path) {
  zone[0][path.front().second] = -1;

  for(uint i = 1; i < path.size(); ++i) {
    auto prev = path[i-1];
    auto curr = path[i];
    // is it forward edge
    if(prev.first < curr.first)
      zone[curr.first][curr.second] = prev.second;
    // does prev node has parent
    else if(zone[prev.first][prev.second] == curr.second)
      zone[prev.first][prev.second] = -2;
  }
  //debug();
}

uint findPath() {
  uint cnt = 0;
  for(auto n : zone.front()) {
    if(n.second != -2) continue;

    queue<vector<pair<int, int>>> que;
    vector<pair<int, int>> v;
    pair<int, int> p(0, n.first);
    v.push_back(p);
    que.push(v);

    set<pair<bool, pair<int, int>>> closed;
    closed.insert(pair<bool, pair<int, int>>(1, p));

    while(!que.empty()) {
      auto path = que.front();
      auto node = path.back();

      //for(auto n : path) cout << n.first << ":" << n.second << " ";
      //cout << " | " << closed.size() << endl;

      // check node is unmarked
      if(zone[node.first][node.second] == -2) {
        // check if node is not in last zone
        if(node.first != (int)zone.size()-1) {
          for(auto nbr : edge[node.second])
          addQue(1, node.first+1, nbr, que, closed);
        }
        // if node is in last zone, check if it is sink
        else if(sink.find(node.second) != sink.end()) {
          buildPath(path);
          cnt++;
          break;
        }
      }
      else {
        auto prev_node = path[path.size()-2];
        addQue(0, node.first-1, zone[node.first][node.second], que, closed);
        // check if rolled back from prev_node, meaning we can also go forward
        if(prev_node.first > node.first) {
          for(auto nbr : edge[node.second])
            if(nbr != prev_node.second) addQue(1, node.first+1, nbr, que, closed);
        }
      }
      que.pop();
    }
  }
  return cnt;
}

bool solve() {
  uint cnt = 0;
  while(true) {
    uint tmp = findPath();
    cnt += tmp;
    if(cnt == sink.size() || zone.size() > edge.size()) break;
    buildZone();
  }
  return (cnt == sink.size());
}

void printPath(int node) {
  list<int> path;
  path.push_front(node);
  for(int i = (int)zone.size()-1; i > 0; --i) {
    node = zone[i][node];
    path.push_front(node);
  }
  for(auto itr = path.begin(); itr != path.end(); ++itr)
    cout << *(--itr) << (++itr == path.end() ? "\n" : " ");
}

void printResult() {
  cout << zone.size()-1 << endl;
  for(auto node : zone.back()) {
    if(node.second != -2) printPath(node.first);
  }
}

int main() {

  getInput();
  buildZone();
  if(solve()) printResult();
  else cout << -1 << endl;
  //debug();

  return 0;
}
