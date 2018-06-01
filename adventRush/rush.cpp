#include <iostream>
#include <cmath>

using namespace std;

struct Tuple {
  
  long int p, m;
  
  Tuple(long int p, long int m) : p(p), m(m) {}
   
};

struct Road {
  
  long int idx, x, y, len;
  
  Road(long int idx, long int x, long int y, long int len) : 
  idx(idx), x(min(x,y)), y(max(x,y)), len(len) {}
  
};

struct RoadNode {
  
  Road* road;
  RoadNode* next;
  
  RoadNode(Road* road, RoadNode* next) : road(road), next(next) {}
  
};

struct RoadList {
  
  RoadNode* beg;
  RoadNode* end;
  
  RoadList() : beg(nullptr), end(nullptr) {}
  
  ~RoadList() {
    while(beg != nullptr) {
      RoadNode* tmp = beg;
      beg = beg->next;
      delete tmp;
    }
  }
  
  void add(Road* road) {
    RoadNode* tmp = new RoadNode(road, nullptr);
    if(beg == nullptr)
      beg = end = tmp;
    else {
      end->next = tmp;
      end = tmp;
    }
  }
  
  Road* pop() {
    RoadNode* tmp = beg;
    Road* r = beg->road;
    beg = beg->next;
    delete tmp;
    return r;
  }
  
  bool isEmpty() {
    return beg == nullptr;
  }
  
};

struct IntNode {

  long int      val;
  IntNode* next;
  
  IntNode(long int val, IntNode* next) : val(val), next(next) {}

};

struct IntList {
  
  IntNode* beg;
  IntNode* end;
  
  IntList() : beg(nullptr), end(nullptr) {}
  
  ~IntList() {
    while(beg != nullptr) {
      IntNode* tmp = beg;
      beg = beg->next;
      delete tmp;
    }
  }
  
  void add(long int val) {
    IntNode* tmp = new IntNode(val, nullptr);
    if(beg == nullptr)
      beg = end = tmp;
    else {
      end->next = tmp;
      end = tmp;
    }
  }
  
  long int pop() {
    IntNode* tmp = beg;
    long int i = beg->val;
    beg = beg->next;
    delete tmp;
    return i;
  }
  
  bool isEmpty() {
    return beg == nullptr;
  }
  
};

struct Square {
  
  bool     vis;
  long int      pls, mns;
  IntNode* nbr;
  
  Square() : vis(0), pls(1), mns(0), nbr(nullptr) {}
  
  ~Square() {
    while(nbr != nullptr) {
      IntNode* tmp = nbr;
      nbr = nbr->next;
      delete tmp;
    }
  }
  
  void add(long int n) {
    nbr = new IntNode(n, nbr);
  }
  
};

struct Set {
  
  long int set;
  long int rank;
  
  Set(long int set) : set(set), rank(0) {}
  
};

struct Heap {
  
  long int size, cnt;
  Road**   heap;
  
  Heap(long int size) : size(size), cnt(0) {
    heap = new Road*[size];
  }
  
  ~Heap() {
    delete [] heap;
  }
  
  void pushBack(Road* r) {
    heap[cnt++] = r;
    if(cnt == size)
      for(long int i = cnt/2; i >= 0; --i)
        bubleDown(i);
  }
  
  void swap(long int i, long int j) {
    Road* t = heap[i];
    heap[i] = heap[j];
    heap[j] = t;
  }
  
  void bubleUp(long int i) {

    if(!i) return;
    while(i > 0) {
      long int p = (i+1)/2-1;
      if(heap[p]->len <= heap[i]->len) return;
      swap(i, p);
      i = p;
    }
    
  }

  void bubleDown(long int i) {
    
    while(i < cnt/2) {
      long int lc = (i+1)*2-1;
      long int rc = (lc+1 < cnt ? lc+1 : lc);
      long int c  = (heap[lc]->len < heap[rc]->len ? lc : rc);
      if(heap[i]->len <= heap[c]->len) return;
      swap(i, c);
      i = c;
    }

  }

  Road* extractMin() {
    
    Road* r = heap[0];
    swap(0, cnt-1);
    cnt--;
    bubleDown(0);
    return r;
    
  }
  
  void show() {
    for(long int i = 0; i < cnt; ++i)
      cout << heap[i]->len << "\n";
    cout << endl;
  }
  
};

long int     SQ, RD, HSIZE;
Set**   SSET;
Heap*   ROAD;

void getInput() {
  
  cin >> SQ >> RD;
  
  ROAD = new Heap(RD);
  SSET = new Set*[SQ];
  
  for(long int i = 0; i < SQ; ++i) SSET[i] = new Set(i);
  
  for(long int i = 0; i < RD; ++i) {
    long int x, y, l;
    cin >> x >> y >> l;
    ROAD->pushBack(new Road(i, x, y, l));
  }
  
}

long int findSet(long int set) {
  if(SSET[set]->set == set)
    return set;
  return findSet(SSET[set]->set);
}

void merge(long int x, long int y) {
  if(SSET[x]->rank >= SSET[y]->rank) {
    SSET[y]->set = x;
    if(SSET[x]->rank == SSET[y]->rank)
      SSET[x]->rank++;
  }
  else
    SSET[x]->set = y;
}

long int decRoad(Square* sqre, RoadList* road, long int& cnt) {
  long int len = 0;
  
  while(cnt < SQ-1) {
    Road* r = ROAD->extractMin();
    long int sx  = findSet(r->x);
    long int sy  = findSet(r->y);
    if(sx != sy) {
      cnt++;
      len += r->len;
      sqre[r->x].add(r->y);
      sqre[r->y].add(r->x);
      road->add(r);
      merge(sx, sy);
    }
  }
  return len;
}

Tuple* findSquare(Square* sqre, long int i) {
  if(sqre[i].vis) return nullptr;
  sqre[i].vis = 1;
  IntNode* n = sqre[i].nbr;
  while(n != nullptr) {
    Tuple* tup = findSquare(sqre, n->val);
    n = n->next;
    if(tup == nullptr) continue;
    sqre[i].pls += min(tup->p, tup->m);
    sqre[i].mns += tup->p;
    delete tup;
  }
  return new Tuple(sqre[i].pls, sqre[i].mns);
}

void decSquare(Square* sqre, IntList* dcor, long int i, bool add) {
  if(!sqre[i].vis) return;
  sqre[i].vis = false;
  bool flag;
  if(add) {
    flag = false;
    dcor->add(i);
  }
  else {
    if(sqre[i].pls < sqre[i].mns) {
      flag = false;
      dcor->add(i);
    }
    else flag = true;
  }
  IntNode* n = sqre[i].nbr;
//cout << i << " | " << sqre[i].pls << ":" <<  sqre[i].mns << " | " << flag << endl;
  while(n != nullptr) {
    decSquare(sqre, dcor, n->val, flag);
    n = n->next;
  }
}

void solve() {
  
  long int       rcnt = 0;
  long int       scnt = 0;
  long int       rlen = 0;
  Tuple*    tup   = nullptr;
  IntList*  dcor  = new IntList();
  RoadList* road  = new RoadList();
  Square*   sqre  = new Square[SQ];

  rlen = decRoad(sqre, road, rcnt);
  
  tup  = findSquare(sqre, 0);
  scnt = min(tup->p, tup->m);
  
  decSquare(sqre, dcor, 0, false);
  
  cout << rlen << " " << scnt << endl;
  
  for(long int i = 0; i < rcnt; ++i)
    cout << road->pop()->idx << (i == rcnt-1 ? "\n" : " ");

  for(long int i = 0; i < scnt; ++i)
    cout << dcor->pop() << (i == scnt-1 ? "\n" : " ");
  
  delete tup;
  delete road;
  delete dcor;
  delete [] sqre;
  
}

int main() {
  
  getInput();
  solve();
  
  return 0;
}
