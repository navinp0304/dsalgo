#include <bits/stdc++.h>
using namespace std;

struct node
{
  node *left = nullptr;
  node *right = nullptr;
  node *par = nullptr;
  int key = 0;
  int sz = 0;
  char color = 'b';
   ~node ()
  {
    sz = key = color = -1;
    par=left=right=nullptr;
  }
};

node *root,*NIL;

void printtree(node *x){
	if(x==NIL) return;
	printtree(x->left);
	cout <<x->key<<",";
	printtree(x->right);
}
node* tree_min(node *x){
	while(x->left!=NIL) x=x->left;
	return x;
}

node* tree_max(node *x){
	while(x->right!=NIL) x=x->right;
	return x;
}

node* tree_succ(node *x){
	if(x->right!=NIL) return tree_min(x->right);
	node *y=x->par;
	while(y!=NIL && x==y->right){
		x=y;
		y=y->par;
	}
	return y;
}

node* tree_pred(node *x){
	if(x->left!=NIL) return tree_max(x->left);
	node *y=x->par;
	while(y!=NIL && x==y->left){
		x=y;
		y=y->par;
	}
	return y;
}


void left_rotate(node *x){
	node *y=x->right;
	x->right=y->left;
	if(y->left!=NIL) y->left->par=x;
	y->par=x->par;
	if(x->par==NIL) root=y;
	else if (x==x->par->left)
		x->par->left=y;
	else x->par->right=y;
	y->left=x;
	x->par=y;
}

void right_rotate(node *x){
	node *y=x->left;
	x->left=y->right;
	if(y->right!=NIL) y->right->par=x;
	y->par=x->par;
	if(x->par==NIL) root=y;
	else if (x==x->par->right)
		x->par->right=y;
	else x->par->left=y;
	y->right=x;
	x->par=y;
}


void rb_insert_fixup(node *);

void rb_insert(int val){
	node *z=new node();
	z->key=val;
	node *x=root;
	node *y=NIL;
	while (x!=NIL){
		y=x;
		if(z->key < x->key) x=x->left;
		else x=x->right;
	}
	z->par=y;
	if(y==NIL) root=z;
	else if (z->key < y->key)
		y->left=z;
	else y->right=z;
	z->left=NIL;
	z->right=NIL;
	z->color='r';
	rb_insert_fixup(z);
}


void rb_insert_fixup(node *z){
	node *y=NIL;
	while(z->par->color=='r'){
		if(z->par==z->par->par->left){
			y=z->par->par->right;
			if(y->color=='r'){
				z->par->color='b';
				y->color='b';
				z->par->par->color='r';
				z=z->par->par;
			} else {
				if(z==z->par->right){
					z=z->par;
					left_rotate(z);
				}
				z->par->color='b';
				z->par->par->color='r';
				right_rotate(z->par->par);
			}
		} else {
			y=z->par->par->left;
			if(y->color=='r'){
				z->par->color='b';
				y->color='b';
				z->par->par->color='r';
				z=z->par->par;
			} else {
				if(z==z->par->left){
					z=z->par;
					right_rotate(z);
				}
				z->par->color='b';
				z->par->par->color='r';
				left_rotate(z->par->par);
			}
		}
	
	}
	root->color='b';
}


void rb_transplant(node *u,node *v){
	if(u->par==NIL) root=v;
	else if (u==u->par->left) u->par->left=v;
	else u->par->right=v;
	v->par=u->par;
}


void rb_delete_fixup(node *);

void rb_delete(int val){
	node *tmp=root;
	while(tmp!=NIL){
		if(tmp->key < val) tmp=tmp->right;
		else if(tmp->key > val) tmp=tmp->left;
		else break;
	}
	if(tmp==NIL) return;
	if(tmp->key!=val) return;
	node *z=tmp;
	node *y=z;
	node *x=NIL;
	char y_original_color=y->color;
	if(z->left==NIL){
		x=z->right;
		rb_transplant(z,z->right);
	} else if(z->right==NIL){
		x=z->left;
		rb_transplant(z,z->left);
	} else {
		y=tree_min(z->right);
		y_original_color=y->color;
		x=y->right;
		if(y!=z->right){
			rb_transplant(y,y->right);
			y->right=z->right;
			y->right->par=y;
		} else x->par=y;
		rb_transplant(z,y);
		y->left=z->left;
		y->left->par=y;
		y->color=z->color;
	}
	delete z;
	if(y_original_color=='b')
		rb_delete_fixup(x);
}

void rb_delete_fixup(node *x){
	node *w=NIL;
	while(x!=root && x->color=='b'){
		if(x==x->par->left){
			w=x->par->right;
			if(w->color=='r'){
				w->color='b';
				x->par->color='r';
				left_rotate(x->par);
				w=x->par->right;
			}
			if(w->left->color=='b' && w->right->color=='b'){
				w->color='r';
				x=x->par;
			}
			else {
				if(w->right->color=='b' ){
					w->left->color='b';
					w->color='r';
					right_rotate(w);
					w=x->par->right;
				}
				w->color=x->par->color;
				x->par->color='b';
				w->right->color='b';
				left_rotate(x->par);
				x=root;
			}
		} else {
			w=x->par->left;
			if(w->color=='r'){
				w->color='b';
				x->par->color='r';
				right_rotate(x->par);
				w=x->par->left;
			}
			if(w->right->color=='b' && w->left->color=='b'){
				w->color='r';
				x=x->par;
			} else {
				if(w->left->color=='b'){
					w->right->color='b';
					w->color='r';
					left_rotate(w);
					w=x->par->left;
				}
				w->color=x->par->color;
				x->par->color='b';
				w->left->color='b';
				right_rotate(x->par);
				x=root;
			}
		}
	}
	x->color='b';
}

int arr[10];

vector < vector < int >>v1, v2;

void
gendel (int lvl, int lim)
{
  if (lvl == lim)
    {
      vector < int >tmp;
      for (int i = 0; i < lim; i++)
	{
	  // cout <<arr[i];
	  tmp.push_back (arr[i]);
	}
      v2.push_back (tmp);
      // cout <<endl;
      return;
    }
  for (int i = 0; i < lim; i++)
    {
      arr[lvl] = i;
      gendel (lvl + 1, lim);
    }
}

void
gen (int lvl, int lim)
{
  if (lvl == lim)
    {
      vector < int >tmp;
      for (int i = 0; i < lim; i++)
	{
	  tmp.push_back (arr[i]);
	}
      // cout <<endl;
      v1.push_back (tmp);
      return;
    }
  for (int i = 0; i < lim; i++)
    {
      arr[lvl] = i;
      gen (lvl + 1, lim);
    }
}

void
genfact (int lim)
{
  vector < int >v1, v2;
  for (int i = 0; i < lim; i++)
    v1.push_back (i);
  for (int i = 0; i < lim; i++)
    v2.push_back (i);

  do
    {
      do
	{
	  for (int i = 0; i < v1.size (); i++)
	    {
	      rb_insert (v1[i]);
	    }
	      //printtree(root);
	  for (int i = 0; i < v2.size (); i++){
	    //cout <<"deleting "<<v2[i]<<endl;
	    //printtree(root);
	    rb_delete (v2[i]);
	    //cout <<"After"<<endl;
	    //printtree(root);

	  }
	}
      while (next_permutation (v2.begin (), v2.end ()));
    }
  while (next_permutation (v1.begin (), v1.end ()));
fail:;
}

int
main ()
{
NIL = new node();
root=NIL;
  // gendel(0,6);
  //printtree(root);
  //printf("\n");
  //genfact (6);
  genfact (6);
  //return 0;
  //
  
  
  //rb_insert(0); rb_insert(0); rb_insert(0);
  //rb_delete(0); rb_delete(2); rb_delete(0);
  //printtree(root);
  gen (0, 5);
  gendel (0, 5);
  for (int i = 0; i < v1.size (); i++)
    {
      vector < int >&vv1 = v1[i];
      for (int j = 0; j < v2.size (); j++)
	{
	  vector < int >&vv2 = v2[j];
	  //for(int u=0;u<vv1.size();u++) cout <<vv1[u]<<"#"; cout <<endl;
	  //for(int u=0;u<vv2.size();u++) cout <<vv2[u]<<"@"; cout <<endl;

	  for (int u = 0; u < vv1.size (); u++)
	    {
	      rb_insert (vv1[u]);
	    }
	  for(int q=0;q<vv2.size();q++)
	  {
		  rb_delete(vv2[q]);
	  }

	}
    }
  printf ("==========\n");
  //printtree (root);
}
