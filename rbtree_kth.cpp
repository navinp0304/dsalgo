#include <bits/stdc++.h>
#include <chrono>
#include <ratio>
using namespace std;

struct node {
  node *left = nullptr;
  node *right = nullptr;
  node *par = nullptr;
  int key = 0;
  int sz = 1;
  int h = 1;
  char color = 'b';
  ~node() {
    h=sz = key = color = -100;
    par = left = right = nullptr;
  }
};

node *root, *NIL;

void printtree(node *x) {
  if (x == NIL) return;
  printtree(x->left);
  // printf("NIL=%p key=%d this=%p left=%p
  // right=%p\n",NIL,x->key,x,x->left,x->right); printf("this->sz=%d left-sz=%d
  // right-sz=%d\n",x->sz,x->left->sz,x->right->sz);
  printf("%d,", x->key);
  printtree(x->right);
}

void fixsizes(node *x) {
  while (x != root) {
    x->sz = (x->left ? x->left->sz : 0) + (x->right ? x->right->sz : 0) + ((x != NIL) ? 1 : 0);
    x->h = max((x->left ? x->left->h : 0) , (x->right ? x->right->h : 0)) + ((x != NIL) ? 1 : 0);
    x = x->par;
  }
  root->sz = (root->left ? root->left->sz : 0) + (root->right ? root->right->sz : 0) + ((root != NIL) ? 1 : 0);
  root->h =max( (root->left ? root->left->h : 0) , (root->right ? root->right->h : 0)) + ((root != NIL) ? 1 : 0);
}

node *tree_min(node *x) {
  while (x->left != NIL) x = x->left;
  return x;
}

node *tree_max(node *x) {
  while (x->right != NIL) x = x->right;
  return x;
}

node *tree_succ(node *x) {
  if (x->right != NIL) return tree_min(x->right);
  node *y = x->par;
  while (y != NIL && x == y->right) {
    x = y;
    y = y->par;
  }
  return y;
}

node *tree_pred(node *x) {
  if (x->left != NIL) return tree_max(x->left);
  node *y = x->par;
  while (y != NIL && x == y->left) {
    x = y;
    y = y->par;
  }
  return y;
}

void left_rotate(node *x) {
  node *y = x->right;
  x->right = y->left;
  if (y->left != NIL) y->left->par = x;
  y->par = x->par;
  if (x->par == NIL)
    root = y;
  else if (x == x->par->left)
    x->par->left = y;
  else
    x->par->right = y;
  y->left = x;
  x->par = y;
  x->sz = x->left->sz + x->right->sz + ((x != NIL) ? 1 : 0);
  y->sz = y->left->sz + y->right->sz + ((y != NIL) ? 1 : 0);

  x->h = max(x->left->h , x->right->h) + ((x != NIL) ? 1 : 0);
  y->h = max(y->left->h , y->right->h) + ((y != NIL) ? 1 : 0);
}

void right_rotate(node *x) {
  node *y = x->left;
  x->left = y->right;
  if (y->right != NIL) y->right->par = x;
  y->par = x->par;
  if (x->par == NIL)
    root = y;
  else if (x == x->par->right)
    x->par->right = y;
  else
    x->par->left = y;
  y->right = x;
  x->par = y;
  x->sz = x->left->sz + x->right->sz + ((x != NIL) ? 1 : 0);
  y->sz = y->left->sz + y->right->sz + ((y != NIL) ? 1 : 0);

  x->h = max(x->left->h , x->right->h) + ((x != NIL) ? 1 : 0);
  y->h = max(y->left->h , y->right->h) + ((y != NIL) ? 1 : 0);
}

int findkth(int k) {
  node *tmp = root;
  // cout <<"tmp->sz= "<<tmp->sz<<" k= "<<k<<endl;
  // printtree(root);cout <<endl;
  assert(tmp->sz >= k);
  // cout <<" rootsz "<<root->sz<<" lsz "<<root->left->sz<<" rsz
  // "<<root->right->sz<<endl; printtree(root); printf("done\n");
  while (tmp != NIL) {
    // cout <<"dbg3:"<<tmp->left->sz<<endl;
    if (tmp->left->sz + 1 == k)
      return tmp->key;
    else if ((tmp->left->sz + 1) < k) {
      k -= tmp->left->sz + 1;
      // cout <<"choosing right "<<k<<endl;
      tmp = tmp->right;
    } else {
      // cout <<"choosing left "<<k<<endl;
      tmp = tmp->left;
    }
  }
  return -100;
}


int findh(node *n){
	if(n==NIL) return 0;
	int lh=findh(n->left);
	int rh=findh(n->right);
	int comp=max(lh,rh)+((n!=NIL)?1:0);
	assert(comp==n->h);
	return comp;
}

int findsz(node *n){
	if(n==NIL) return 0;
	int lsz=findsz(n->left);
	int rsz=findsz(n->right);
	int exp=lsz+rsz+((n!=NIL)?1:0);
	assert(exp==n->sz);
	return exp;
}

void rb_insert_fixup(node *);

void rb_insert(int val) {
  node *z = new node();
  z->key = val;
  node *x = root;
  node *y = NIL;
  while (x != NIL) {
    y = x;
    if (z->key < x->key)
      x = x->left;
    else
      x = x->right;
  }
  z->par = y;
  if (y == NIL)
    root = z;
  else if (z->key < y->key)
    y->left = z;
  else
    y->right = z;
  z->left = NIL;
  z->right = NIL;
  z->color = 'r';
  rb_insert_fixup(z);
  fixsizes(z);
}

void rb_insert_fixup(node *z) {
  node *y = NIL;
  while (z->par->color == 'r') {
    if (z->par == z->par->par->left) {
      y = z->par->par->right;
      if (y->color == 'r') {
        z->par->color = 'b';
        y->color = 'b';
        z->par->par->color = 'r';
        z = z->par->par;
      } else {
        if (z == z->par->right) {
          z = z->par;
          left_rotate(z);
        }
        z->par->color = 'b';
        z->par->par->color = 'r';
        right_rotate(z->par->par);
      }
    } else {
      y = z->par->par->left;
      if (y->color == 'r') {
        z->par->color = 'b';
        y->color = 'b';
        z->par->par->color = 'r';
        z = z->par->par;
      } else {
        if (z == z->par->left) {
          z = z->par;
          right_rotate(z);
        }
        z->par->color = 'b';
        z->par->par->color = 'r';
        left_rotate(z->par->par);
      }
    }
  }
  root->color = 'b';
}

void rb_transplant(node *u, node *v) {
  if (u->par == NIL)
    root = v;
  else if (u == u->par->left)
    u->par->left = v;
  else
    u->par->right = v;
  v->par = u->par;
}

void rb_delete_fixup(node *);

void rb_delete(int val) {
  node *tmp = root;
  while (tmp != NIL) {
    if (tmp->key < val)
      tmp = tmp->right;
    else if (tmp->key > val)
      tmp = tmp->left;
    else
      break;
  }
  if (tmp == NIL) return;
  if (tmp->key != val) return;
  node *z = tmp;
  node *y = z;
  node *x = NIL;
  char y_original_color = y->color;
  if (z->left == NIL) {
    x = z->right;
    rb_transplant(z, z->right);
  } else if (z->right == NIL) {
    x = z->left;
    rb_transplant(z, z->left);
  } else {
    y = tree_min(z->right);
    y_original_color = y->color;
    x = y->right;
    if (y != z->right) {
      rb_transplant(y, y->right);
      y->right = z->right;
      y->right->par = y;
    } else
      x->par = y;
    rb_transplant(z, y);
    y->left = z->left;
    y->left->par = y;
    y->color = z->color;
  }
  if (y_original_color == 'b') rb_delete_fixup(x);
  fixsizes(x);
  delete z;
}

void rb_delete_fixup(node *x) {
  node *w = NIL;
  while (x != root && x->color == 'b') {
    if (x == x->par->left) {
      w = x->par->right;
      if (w->color == 'r') {
        w->color = 'b';
        x->par->color = 'r';
        left_rotate(x->par);
        w = x->par->right;
      }
      if (w->left->color == 'b' && w->right->color == 'b') {
        w->color = 'r';
        x = x->par;
      } else {
        if (w->right->color == 'b') {
          w->left->color = 'b';
          w->color = 'r';
          right_rotate(w);
          w = x->par->right;
        }
        w->color = x->par->color;
        x->par->color = 'b';
        w->right->color = 'b';
        left_rotate(x->par);
        x = root;
      }
    } else {
      w = x->par->left;
      if (w->color == 'r') {
        w->color = 'b';
        x->par->color = 'r';
        right_rotate(x->par);
        w = x->par->left;
      }
      if (w->right->color == 'b' && w->left->color == 'b') {
        w->color = 'r';
        x = x->par;
      } else {
        if (w->left->color == 'b') {
          w->right->color = 'b';
          w->color = 'r';
          left_rotate(w);
          w = x->par->left;
        }
        w->color = x->par->color;
        x->par->color = 'b';
        w->left->color = 'b';
        right_rotate(x->par);
        x = root;
      }
    }
  }
  x->color = 'b';
}

int arr[10];

vector<vector<int>> v1, v2;

void gendel(int lvl, int lim) {
  if (lvl == lim) {
    vector<int> tmp;
    for (int i = 0; i < lim; i++) {
      // cout <<arr[i];
      tmp.push_back(arr[i]);
    }
    v2.push_back(tmp);
    // cout <<endl;
    return;
  }
  for (int i = 0; i < lim; i++) {
    arr[lvl] = i;
    gendel(lvl + 1, lim);
  }
}

void gen(int lvl, int lim) {
  if (lvl == lim) {
    vector<int> tmp;
    for (int i = 0; i < lim; i++) {
      tmp.push_back(arr[i]);
    }
    // cout <<endl;
    v1.push_back(tmp);
    return;
  }
  for (int i = 0; i < lim; i++) {
    arr[lvl] = i;
    gen(lvl + 1, lim);
  }
}

void genfact(int lim) {
  vector<int> v1, v2;
  for (int i = 0; i < lim; i++) v1.push_back(i);
  for (int i = 0; i < lim; i++) v2.push_back(i);

  do {
    do {
      for (int i = 0; i < v1.size(); i++) {
        rb_insert(v1[i]);
	findh(root);
	findsz(root);
        // printtree(root);cout <<"done\n";
        // cout <<"leftsz= "<<root->left->sz<<" Rsz= "<<root->right->sz<<" =
        // "<<i+1<<endl;
	assert((root->left?root->left->sz:0)+(root->right?root->right->sz:0)+((root!=NIL)?1:0)==i+1);
        vector<int> tmp(v1.begin(), v1.begin() + i + 1);
        sort(tmp.begin(), tmp.end());
        for (int j = 0; j < tmp.size(); j++) {
          int kth = findkth(j + 1);
          // printtree(root);
          // cout <<"found "<<kth<<" "<<tmp[j]<<" kth="<<j+1<<endl;
          assert(kth == tmp[j]);
        }
      }
      set<int> s(v1.begin(), v1.end());
      for (int i = 0; i < v2.size(); i++) {
        s.erase(v2[i]);
        vector<int> tmp(s.begin(), s.end());

	assert((root->left?root->left->sz:0)+(root->right?root->right->sz:0)+((root!=NIL)?1:0)==v2.size()-i);
        // cout <<"deleting ="<<v2[i]<<" tmp=";
        // for(int p:tmp) cout <<p<<","; cout <<endl;
        rb_delete(v2[i]);
	findh(root);
	findsz(root);
        for (int j = 0; j < tmp.size(); j++) {
          int kth = findkth(j + 1);
          // printtree(root);cout <<endl;
          // cout <<"k="<<j+1<<" kth= "<<kth<<" exp="<<tmp[j]<<endl;
          assert(kth == tmp[j]);
        }
      }
    } while (next_permutation(v2.begin(), v2.end()));
  } while (next_permutation(v1.begin(), v1.end()));
fail:;
}

int main() {
  NIL = new node();
  NIL->sz = NIL->h=0;
  root = NIL;
  // gendel(0,6);
  // printtree(root);
  // printf("\n");
  // genfact (6);
  genfact(6);
  //

  printf("genfact done\n");
  // rb_insert(0); rb_insert(0); rb_insert(0);
  // rb_delete(0); rb_delete(2); rb_delete(0);
  // printtree(root);
  v1.clear();
  v2.clear();
  gen(0, 4);
  gendel(0, 4);
  assert(root->sz==0);
  int cnt=0;
  const auto start = std::chrono::system_clock::now();
  for (int i = 0; i < v1.size(); i++) {
    vector<int> &vv1 = v1[i];
    for (int j = 0; j < v2.size(); j++) {
      vector<int> &vv2 = v2[j];
       //for(int u=0;u<vv1.size();u++) cout <<vv1[u]<<"#"; cout <<endl;
       //for(int u=0;u<vv2.size();u++) cout <<vv2[u]<<"@"; cout <<endl;

      const auto cur = std::chrono::system_clock::now();
      if(cnt%125==0){
	      auto ms=std::chrono::duration_cast<std::chrono::milliseconds>(cur-start);
	      double hours=ms.count()/(3600.0*1000.0);
	      double perc=cnt*100./(v1.size()*v2.size()*2*vv1.size());
	      printf(" ETA=: %.3f hours=%.3f perc=%.3f\r",hours*100.0/perc,hours,perc);

      }
      for (int u = 0; u < vv1.size(); u++) {
      	cnt++;
        rb_insert(vv1[u]);
	findh(root);
	findsz(root);
	/*
	ms.insert(vv1[u]);
	vector<int> tmp(ms.begin(),ms.end());
        for (int p = 0; p < tmp.size(); p++) {
          int kth = findkth(p + 1);
          assert(kth == tmp[p]);
        }
	*/
      }
      for (int q = 0; q < vv2.size(); q++) {
        cnt++;
        rb_delete(vv2[q]);
	findh(root);
	findsz(root);
	/*
	auto it=ms.find(vv2[q]);
	if(it!=ms.end()) ms.erase(it);
	vector<int> tmp(ms.begin(),ms.end());
        for (int p = 0; p < tmp.size(); p++) {
          int kth = findkth(p + 1);
          assert(kth == tmp[p]);
        }
	*/
      }
    }
  }
  printf("==========\n");
  // printtree (root);
}
