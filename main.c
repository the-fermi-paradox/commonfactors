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
int* digstackp = digstack;
void pushdig(int val) {
  if (digstackp - digstack >= BUFFSIZE)
    return;
  *digstackp++ = val;
}

int popdig(void) {
  if (digstackp - digstack == 0)
    return EOF;
  return *--digstackp;
}

struct ListNode {
  int value;
  struct ListNode* next;
};

#define HASH_SIZE 500
struct ListNode* hash[HASH_SIZE];
int hash_initialize() {
  struct ListNode** hashp = hash;
  while (hashp - hash < HASH_SIZE) {
    *hashp = malloc(sizeof(struct ListNode));
    assert(hashp);
    (*hashp)->value=0;
    (*hashp)->next=NULL;
    ++hashp;
  }
  return 1;
}
int hash_free() {
  struct ListNode** hashp = hash;
  while (hashp - hash < HASH_SIZE) {
    struct ListNode* node = *hashp;
    do {
      struct ListNode* tmp = node;
      node = node->next;
      free(tmp);
    } while (node);
    ++hashp;
  }
  return 1;
}

int hash_hashing(int val) {
  /* super basic hashing */
  return val % HASH_SIZE;
}

int hash_set(int val) {
  struct ListNode* node = *(hash + hash_hashing(val));
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
  struct ListNode* node = *(hash + hash_hashing(val));
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
  int* factorsp = factors;
  int i = 1;
  int div;
  for (i = 1; i <= num; ++i) {
    /* if i evenly divides num */
    if (num % i == 0) {
      div = num / i;
      if (!hash_set(div)) {
        *factorsp++ = div;
      }
      if (!hash_set(i)) {
        *factorsp++ = i;
      }
    }
  }
  *return_length = factorsp - factors;
  return factors;
}

int main(int argc, char** argv)
{
  int ch = 0;
  /* routine to retrieve numbers to be processed and store them on a stack */
  while ((ch = fgetc(stdin)) != EOF) {
    if (isdigit(ch)) {
      pushdig(ch - '0');
    } else {
      int x = 0;
      int n = 1;
      int sum = 0;
      while ((x = popdig()) != EOF) {
        sum = sum + x * n; 
        n *= 10;
      }
      if (sum) {
        hash_initialize();
        int return_length;
        int* factors = findfactors(sum, &return_length);
        int* factorsp = factors + return_length - 1;
        printf("factors of %d:\n", sum);
        while (factorsp >= factors) {
          printf("%d", *factorsp);
          if (factorsp - factors != 0) {
            printf(", ");
          }
          --factorsp;
        }
        free(factors);
        hash_free();
        printf("\n");
      }
    }
  }
}
