#include <iostream>

using namespace std;

struct Node {
  uint32_t key, lcnt, rcnt;
  int32_t  height, weight;
  Node*    parent;
  Node*    lchild;
  Node*    rchild;
  
  Node(uint32_t i) : key(i), lcnt(0), rcnt(0), height(0), weight(0), parent(nullptr), lchild(nullptr), rchild(nullptr) {}
};

struct Tuple {
  Node*    node;
  uint32_t index;
  
  Tuple(Node* n, uint32_t i) : node(n), index(i) {}
};

struct Tree {
  Node* root;
  
  Tree() : root(nullptr) {}
  
  Tuple* findNode(Node* node, uint32_t val, uint32_t idx) {
    if(root == nullptr || node->key == val)
      return new Tuple(node, idx);
    if(val < node->key && node->lchild != nullptr) 
      return findNode(node->lchild, val, idx-(node->lchild->rcnt)-1);
    if(val > node->key && node->rchild != nullptr) 
      return findNode(node->rchild, val, idx+(node->rchild->lcnt)+1);
    return new Tuple(node, idx);
  }
  
  Node* append(uint32_t val) {
    if(root == nullptr) {
      root = new Node(val);
      return nullptr;
    }
    
    Tuple* t = findNode(root, val, root->lcnt);
    Node* np = t->node; 
    delete t;
    
    if(np->key == val) return nullptr;
    
    Node* nc = new Node(val);
    nc->parent = np;
    
    if(np->key >  val) np->lchild = nc;
    else               np->rchild = nc;
    
    return np;
  }
  
  void leftRotate(Node* x) {
    Node* y = x->rchild;
    Node* b = y->lchild;
    
    if(x->parent != nullptr) {
      if(x == x->parent->rchild) x->parent->rchild = y;
      else x->parent->lchild = y;
    }
    else root = y;
    y->parent = x->parent;
    y->lchild = x;
    x->parent = y;
    x->rchild = b;
    if(b != nullptr) b->parent = x;
  }

  void rightRotate(Node* y) {
    Node* x = y->lchild;
    Node* b = x->rchild;
    
    if(y->parent != nullptr) {
      if(y == y->parent->rchild) y->parent->rchild = x;
      else y->parent->lchild = x;
    }
    else root = x;
    x->parent = y->parent;
    x->rchild = y;
    y->parent = x;
    y->lchild = b;
    if(b != nullptr) b->parent = y;

    setBundle(y);
    setBundle(x);
  }
  
  void rotateL(Node* node) {
    leftRotate(node);
    
    setBundle(node);
    setBundle(node->parent);
  }
  
  void rotateR(Node* node) {
    rightRotate(node);
    
    setBundle(node);
    setBundle(node->parent);
  }
  
  void rotateLR(Node* node) {
    leftRotate(node->lchild);
    rightRotate(node);
    
    setBundle(node->parent->lchild);
    setBundle(node);
    setBundle(node->parent);
  }

  void rotateRL(Node* node) {
    rightRotate(node->rchild);
    leftRotate(node);
    
    setBundle(node);
    setBundle(node->parent->rchild);
    setBundle(node->parent);
  }
  
  void balance(Node* node) {
    bool wn = node->weight > 0;
    bool wc = wn > 0 ? node->rchild->weight > 0 : node->lchild->weight > 0;

//cout << "bal: " << node->key << endl;
    if     (wn  &&  wc) rotateL(node);
    else if(wn  && !wc) rotateRL(node);
    else if(!wn &&  wc) rotateLR(node);
    else                rotateR(node);
  }
  
  void setHeight(Node* node) {
    if(node == nullptr) return;
    
    if(node->lchild == nullptr && node->rchild == nullptr)
      node->height = 0;
    else if(node->rchild == nullptr)
      node->height = node->lchild->height + 1;
    else if(node->lchild == nullptr)
      node->height = node->rchild->height + 1;
    else
      node->height = max(node->lchild->height, node->rchild->height) + 1;
  }
  
  void setCount(Node* node) {
    node->lcnt = node->rcnt = 0;
    if(node->lchild != nullptr)
      node->lcnt = node->lchild->lcnt + node->lchild->rcnt + 1;
    if(node->rchild != nullptr)
      node->rcnt = node->rchild->lcnt + node->rchild->rcnt + 1;
  }
  
  void setWeight(Node* node) {
    if(node == nullptr) return;
    
    int32_t lh = node->lchild == nullptr ? -1 : node->lchild->height;
    int32_t rh = node->rchild == nullptr ? -1 : node->rchild->height;
    
    node->weight = rh - lh;
    
    if(abs(node->weight) > 1) balance(node);
  }
  
  void setBundle(Node* node) {
    setHeight(node);
    setCount(node);
    setWeight(node);
  }
  
  void rearrange(Node* node) {
    if(node == nullptr) return;
    setBundle(node);
    rearrange(node->parent);
  }
  
  Node* findSubstitude(Node* node) {
    if(node->rchild == nullptr) return node;
    
    Node* subs = node->rchild;
    while(subs->lchild != nullptr) subs = subs->lchild;
    
    return subs;
  }
  
  void remove(uint32_t val) {
    if(root == nullptr) return;
    
    Tuple* t = findNode(root, val, root->lcnt);
    Node* node = t->node;
    delete t;
    
    if(node->key != val) return;
    
    Node* subs = findSubstitude(node);
    Node* prnt = subs->parent;
    
    if(node == subs) {
      if(prnt != nullptr) subs == prnt->lchild ? prnt->lchild = subs->lchild : prnt->rchild = subs->lchild;
      else root = node->lchild;
      if(node->lchild != nullptr) node->lchild->parent = prnt;
    }
    else {
      node->key = subs->key;
      if(prnt != nullptr) subs == prnt->lchild ? prnt->lchild = subs->rchild : prnt->rchild = subs->rchild;
      if(subs->rchild != nullptr) subs->rchild->parent = prnt;
    }
    
    delete subs;
    rearrange(prnt);
  }
  
  Node* findIndex(Node* node, uint32_t val) {
    if(val < node->lcnt) return findIndex(node->lchild, val);
    if(val > node->lcnt) return findIndex(node->rchild, val-(node->lcnt)-1);
    return node;
  }
  
  uint32_t findMedian(uint32_t a, uint32_t b) {
    if(root == nullptr) return 0;
    
    Tuple* ta = findNode(root, a, root->lcnt);
    Tuple* tb = findNode(root, b, root->lcnt);
    Node*  na = ta->node;
    Node*  nb = tb->node;
    uint32_t ia = ta->index;
    uint32_t ib = tb->index;
    delete ta;
    delete tb;
    
    if(a > na->key) {
      if(ia == root->lcnt + root->rcnt) return 0;
      ia++;
    }
    if(b < nb->key) {
      if(!ib) return 0;
      ib--;
    }
    if(ia > ib) return 0;
    
    Node* nm = findIndex(root, (ia + ib) / 2);
    return nm->key;
  }
  
  void show(Node* n) {
    if(n == nullptr) return;
    if(n->lchild != nullptr) show(n->lchild);
    if(n->rchild != nullptr) show(n->rchild);
    cout << (n->lcnt ? n->lchild->key : 0) << ":" << n->key << ":" << (n->rcnt ? n->rchild->key : 0);
    cout << "\t| " << n->height << ":" << n->weight;
    cout << " | " << n->lcnt << ":" << n-> rcnt << endl;
  }
};

Tree* tree = new Tree();

void insert() {
  uint32_t val;
  cin >> val;
  
  tree->rearrange(tree->append(val));

//cout << "\n***TREE***\t" << val << endl << endl;
//tree->show(tree->root);
}

void remove() {
  uint32_t val;
  cin >> val;

  tree->remove(val);

//cout << "\n***TREE***\t" << val << endl << endl;
//tree->show(tree->root);
}

void median() {
  uint32_t a, b;
  cin >> a >> b;
  
//tree->show(tree->root);
  
  uint32_t med = tree->findMedian(a, b);
  if(med) cout << med << endl;
  else    cout << "notfound\n";
}

int main() {
  
  int cmd;
  
  while(true) {
    cin >> cmd;
    switch(cmd) {
      case 1 : insert(); break;
      case 2 : remove(); break;
      case 3 : median(); break;
      default: return 0;
    }
  }
}















































