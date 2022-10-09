#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
  LEFT,
  RIGHT,
  BALANCED,
} direction;

typedef struct node_ {
  int key;
  char value[32];
  struct node_* children[2];
  direction balance;
} node;

bool rebalance(node** p, direction dir) {
  direction opp;
  if (dir == LEFT) {
    opp = RIGHT;
  } else {
    opp = LEFT;
  }

  // case 1: 挿入された方向と逆の高さが1高い場合
  node* a = *p;
  if (a->balance == opp) {
    a->balance = BALANCED;
    return false;
  }

  // case 2: 左右の高さが等しい場合
  if (a->balance == BALANCED) {
    a->balance = dir;
    return true;
  }

  // case 3: 挿入された方向の高さが1高い場合
  // case 3a: 1重回転
  node* b = a->children[dir];
  if (b->balance == dir) {
    a->children[dir] = b->children[opp];  // βをBからAに付け替え
    b->children[opp] = a;                 // βの場所にAを入れる
    a->balance = BALANCED;
    b->balance = BALANCED;
    *p = b;  // Bを上に持ち上げる
    return false;
  }

  // case 3c: 2重回転
  if (b->balance == opp) {
    node* c = b->children[opp];
    b->children[opp] = c->children[dir];  // β1をCからBに付け替え
    a->children[dir] = c->children[opp];  // β2をCからAに付け替え
    c->children[dir] = b;                 // BをCに付け替え
    c->children[opp] = a;                 // AをCに付け替え
    if (c->balance != opp) {
      b->balance = BALANCED;
    } else {
      b->balance = dir;
    }
    if (c->balance != dir) {
      a->balance = BALANCED;
    } else {
      a->balance = opp;
    }
    *p = c;  // Cを上に持ち上げる
    return false;
  }

  // case 3b (invalid)
  assert(false);
}

// grown を返す。
bool insert(node** p_current, int key, const char* value) {
  node* current = *p_current;

  // リーフに達したら挿入
  if (current == NULL) {
    node* n = (node*)malloc(sizeof(node));
    n->key = key;
    n->children[LEFT] = NULL;
    n->children[RIGHT] = NULL;
    n->balance = BALANCED;
    strcpy(n->value, value);
    *p_current = n;
    return true;
  }

  // キーが一致したらエラー
  assert(key != current->key);

  if (key < current->key) {
    if (insert(&current->children[LEFT], key, value)) {
      return rebalance(p_current, LEFT);
    }
  } else {
    if (insert(&current->children[RIGHT], key, value)) {
      return rebalance(p_current, RIGHT);
    }
  }
  return false;
}

void print(node* current, int depth) {
  if (current == NULL) {
    return;
  }
  // right
  print(current->children[RIGHT], depth + 1);

  // current
  printf("|");
  for (int i = 0; i < depth; i++) {
    printf("  ");
  }
  printf("{%d, %s}\n", current->key, current->value);

  // left
  print(current->children[LEFT], depth + 1);
}

int main() {
  node* root = NULL;

  insert(&root, 5, "5");
  insert(&root, 8, "8");  // case 2
  printf("TREE:\n");
  print(root, 1);

  insert(&root, 9, "9");  // case 3a
  printf("TREE:\n");
  print(root, 1);

  insert(&root, 4, "4");  // case 2
  insert(&root, 6, "6");  // case 1
  printf("TREE:\n");
  print(root, 1);

  insert(&root, 7, "7");  // case 3b
  printf("TREE:\n");
  print(root, 1);
}
