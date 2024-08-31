#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
const char* NC="\033[0m";
const char* BRed="\033[1;31m";

#define BUFFSIZE 1000

int numstack[BUFFSIZE];
int* numstackp = numstack;

/* takes ints 0-9 */
int digstack[BUFFSIZE];
int stackp = 0;
void pushdig(int val) {
  if (stackp >= BUFFSIZE)
    return;
  digstack[stackp++] = val;
}

int popdig(void) {
  if (stackp == 0)
    return EOF;
  return digstack[--stackp];
}

struct ListNode {
  int value;
  struct ListNode* next;
};

#define HASH_SIZE 500
struct ListNode* hash[HASH_SIZE];
int hash_initialize() {
  for (int p = 0; p < HASH_SIZE; p++) {
    hash[p] = malloc(sizeof(struct ListNode));
    assert(hash[p]);
    hash[p]->value=0;
    hash[p]->next=NULL;
  }
  return 1;
}

int hash_free() {
  for (int p = 0; p < HASH_SIZE; p++) {
    struct ListNode* node = hash[p];
    do {
      struct ListNode* tmp = node;
      node = node->next;
      free(tmp);
    } while (node);
  }
  return 1;
}

int hash_hashing(int val) {
  /* super basic hashing */
  return val % HASH_SIZE;
}

int hash_set(int val) {
  struct ListNode* node = hash[hash_hashing(val)];
  do {
    if (node->value == val) {
      return 1;
    }  
    if (node->value == 0) {
      node->value = val;
      return 0;
    }

    if (node->next == NULL) {
      node->next = malloc(sizeof(struct ListNode));
      node->next->value = val;
      node->next->next = NULL;
      return 0;
    }
    node = node->next;
  } while (1);
}

int hash_check(int val) {
  const struct ListNode* node = hash[hash_hashing(val)];
  do {
    if (node->value == val) {
      return 1;
    }
    if (node->next == NULL) {
      return 0;
    }
    node = node->next;
  } while (1);
}

int* findfactors(int num, int* return_length) {
#define BLOCK_SIZE 100
  int* factors = malloc(sizeof(int) * BLOCK_SIZE);
  assert(factors);
  int fp = 0;
  for (int i = 1; i <= num; ++i) {
    if (num % i == 0) {
      const int div = num / i;
      if (!hash_set(div)) {
        factors[fp++] = div;
      }
      if (!hash_set(i)) {
        factors[fp++] = i;
      }
    }
  }
  *return_length = fp;
  return factors;
}

int main(int argc, char** argv)
{
  int ch = 0;
  while ((ch = fgetc(stdin)) != EOF) {
    if (isdigit(ch)) {
      pushdig(ch - '0');
      continue;
    }
    int n = 1;
    int sum = 0;

    int x;
    while ((x = popdig()) != EOF) {
      sum += x * n;
      n *= 10;
    }
    if (sum) {
      hash_initialize();
      int return_length;
      int* factors = findfactors(sum, &return_length);
      printf("factors of %d:\n", sum);
      for (int fp = return_length - 1; fp >= 0; --fp) {
        printf("%d", factors[fp]);
        if (fp != 0) {
          printf(", ");
        }
      }
      free(factors);
      hash_free();
      printf("\n");
    }
  }
}
