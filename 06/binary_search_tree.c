#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _node {
  int key;
  char value[32];
  struct _node* left;
  struct _node* right;
} node;

typedef struct {
  node* root;
} tree;

node* init_node(int key, const char* value) {
  node* n = (node*)malloc(sizeof(node));
  n->key = key;
  n->left = NULL;
  n->right = NULL;
  strcpy(n->value, value);
  return n;
}

void clear(node** p_current) {
  node* current = *p_current;
  if (current != NULL) {
    clear(&current->left);
    clear(&current->right);
    free(current);
    *p_current = NULL;
  }
}

node* search(node* current, int target) {
  if (current == NULL) {
    return NULL;
  }

  if (target == current->key) {
    return current;
  }

  if (target < current->key) {
    return search(current->left, target);
  } else {
    return search(current->right, target);
  }
}

void insert(node** p_current, int key, const char* value) {
  node* current = *p_current;
  if (current == NULL) {
    *p_current = init_node(key, value);
    return;
  }

  if (key == current->key) {
    printf("The key is already used.\n");
    return;
  }

  if (key < current->key) {
    insert(&current->left, key, value);
  } else {
    insert(&current->right, key, value);
  }
}

node* extract_max(node** p_current) {
  node* current = *p_current;
  if (current->right == NULL) {
    *p_current = current->left;
    return current;
  }
  return extract_max(&current->right);
}

void erase(node** p_current, int key) {
  node* current = *p_current;
  if (current == NULL) {
    printf("This key does not exist.\n");
    return;
  }

  if (key == current->key) {
    if (current->left == NULL) {
      *p_current = current->right;
      free(current);
    } else {
      node* max = extract_max(&current->left);
      max->left = current->left;
      max->right = current->right;
      *p_current = max;
      free(current);
    }
    return;
  }

  if (key < current->key) {
    erase(&current->left, key);
  } else {
    erase(&current->right, key);
  }
}

void print(node* current, int depth) {
  if (current == NULL) {
    return;
  }
  // right
  print(current->right, depth + 1);

  // current
  for (int i = 0; i < depth; i++) {
    printf("  ");
  }
  printf("{ %d, %s}\n", current->key, current->value);

  // left
  print(current->left, depth + 1);
}

int main() {
  // create tree
  int keys[] = {44, 55, 12, 42, 14, 18, 06, 67};
  int num_keys = sizeof(keys) / sizeof(int);
  tree t = {NULL};
  for (int i = 0; i < num_keys; i++) {
    insert(&t.root, keys[i], "AAA");
  }
  printf("TREE:\n");
  print(t.root, 0);

  // search target
  int target = 44;
  node* result = search(t.root, target);
  if (result != NULL) {
    printf("%d is %s\n", target, result->value);
  } else {
    printf("%d is not found\n", target);
  }

  // erase target
  erase(&t.root, target);
  printf("%d was deleted.\n", target);
  printf("TREE:\n");
  print(t.root, 0);

  // search target
  result = search(t.root, target);
  if (result != NULL) {
    printf("%d is %s\n", target, result->value);
  } else {
    printf("%d is not found\n", target);
  }

  clear(&t.root);
  return 0;
}

// 実行結果
// TREE:
//     { 67, AAA}
//   { 55, AAA}
// { 44, AAA}
//     { 42, AAA}
//         { 18, AAA}
//       { 14, AAA}
//   { 12, AAA}
//     { 6, AAA}
// 44 is AAA
// 44 was deleted.
// TREE:
//     { 67, AAA}
//   { 55, AAA}
// { 42, AAA}
//       { 18, AAA}
//     { 14, AAA}
//   { 12, AAA}
//     { 6, AAA}
// 44 is not found