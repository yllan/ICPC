#include<iostream>
#include<algorithm>
#include<set>
#include<vector>
#include<string>

using namespace std;

int n, k;

typedef long long ll;

struct node {
  ll v;
  node *left;
  node *right;
};

void insert(node *tree, int v) {
  if (v < tree->v) {
    if (tree->left) {
      insert(tree->left, v);
    } else {
      node *leaf = new node();
      leaf->v = v;
      leaf->left = NULL;
      leaf->right = NULL;
      tree->left = leaf;
    }
  } else if (v > tree->v) {
    if (tree->right) {
      insert(tree->right, v);
    } else {
      node *leaf = new node();
      leaf->v = v;
      leaf->left = NULL;
      leaf->right = NULL;
      tree->right = leaf;
    }
  }
}

string representation(node *tree) {
  if (tree == NULL) return ".";
  return "(" + representation(tree->left) + representation(tree->right) + ")";
}

int main()
{
  cin >> n >> k;
  set<string> prototypes;
  for (int i = 0; i < n; i++) {
    node *root = NULL;
    for (int j = 0; j < k; j++) {
      ll v;
      cin >> v;
      if (!root) {
        root = new node();
        root->v = v;
        root->left = NULL;
        root->right = NULL;
      } else {
        insert(root, v);
      }
    }
    prototypes.insert(representation(root));
  }
  cout << prototypes.size() << endl;
}