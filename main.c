#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>
#include <stdint.h>
const char* NC="\033[0m";
const char* BRed="\033[1;31m";

#define BUFFSIZE 1000

/* takes ints 0-9 */
uint64_t digstack[BUFFSIZE];
uint64_t stackp = 0;
void pushdig(uint64_t val) {
  if (stackp >= BUFFSIZE)
    return;
  digstack[stackp++] = val;
}

uint64_t popdig(void) {
  if (stackp == 0)
    return EOF;
  return digstack[--stackp];
}

struct ListNode {
  uint64_t value;
  struct ListNode* next;
};

#define HASH_SIZE 500
struct ListNode* hash[HASH_SIZE];
uint64_t hash_initialize() {
  for (uint64_t p = 0; p < HASH_SIZE; p++) {
    hash[p] = malloc(sizeof(struct ListNode));
    assert(hash[p]);
    hash[p]->value=0;
    hash[p]->next=NULL;
  }
  return 1;
}

uint64_t hash_free() {
  for (uint64_t p = 0; p < HASH_SIZE; p++) {
    struct ListNode* node = hash[p];
    do {
      struct ListNode* tmp = node;
      node = node->next;
      free(tmp);
    } while (node);
  }
  return 1;
}

uint64_t hash_hashing(uint64_t val) {
  /* super basic hashing */
  return val % HASH_SIZE;
}

uint64_t hash_set(uint64_t val) {
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

uint64_t hash_check(uint64_t val) {
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

uint64_t* findfactors(uint64_t num, uint64_t* return_length) {
#define BLOCK_SIZE 100
  uint64_t* factors = malloc(sizeof(uint64_t) * BLOCK_SIZE);
  assert(factors);
  uint64_t fp = 0;
  for (uint64_t i = 1; i <= sqrt(num); ++i) {
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
  uint64_t ch;
  while ((ch = fgetc(stdin)) != EOF) {
    if (isdigit(ch)) {
      pushdig(ch - '0');
      continue;
    }
    uint64_t n = 1;
    uint64_t sum = 0;

    uint64_t x;
    while ((x = popdig()) != EOF) {
      sum += x * n;
      n *= 10;
    }
    if (sum) {
      hash_initialize();
      uint64_t return_length;
      uint64_t* factors = findfactors(sum, &return_length);
      printf("factors of %llu:\n", sum);
      for (uint64_t fp = return_length - 1; fp < return_length; --fp) {
        printf("%llu", factors[fp]);
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
