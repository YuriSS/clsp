#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
  HASH_TYPE_INT,
  HASH_TYPE_CHAR,
  HASH_TYPE_STRING,
  HASH_TYPE_HASH_TABLE,
} HashType;

typedef struct HashNode {
  char *key;
  void *value;
  HashType type;
  struct HashNode *next;
} HashNode;

typedef struct {
  HashNode **nodes;
  size_t capacity;
  size_t size;
} HashTable;

HashNode *create_hash_node(char *key, void *value, size_t valueSize,
                           HashType valueType);
HashTable *create_hash_table(size_t capacity);

void hash_table_insert(HashTable *ht, char *key, void *value, size_t valueSize,
                       HashType valueType);
void hash_table_print(HashTable *ht, int identation);
void hash_table_free(HashTable *ht);
void hash_table_realloc(HashTable *ht);
void *hash_table_get(HashTable *ht, char *key);

void hash_node_print(HashNode *hn, int identation);
void hash_node_free(HashNode *hn);

int djb2(char *key, size_t size);

int main() {
  HashTable *ht = create_hash_table(32);

  char str[1000] = "XepiUSzUfgZeihnhTzcotOGvjyZbYWWKkaGbSYVuJsJygZieirfqhkkIFRQUyZYLUIsVlglrSaPwFSVQrgWOgXjiygeBifGoRfHhtBvLowAQWqUUKVWDnFAuUzctsKPHjjCajKldhEKwdmOuFTyTHsCmSFjICVDzOJnEqtvcuwBFtnpobkofHLhWTrHXCloaIswHixrQGwHQeCJwfEhjouuqWFnovqJTHHqajlGlZurXcBXEmQhziZSWsGAzDfxbGJbvaCpTDudeutrtFpuAKBVuUFKeHwmTyrIikJQqCuZEFOimbiBzRDyshIDhZJMPpZYhyKcbxuLkhSagrraOWbJrlLBVxSHSZGEFwnyZMPBatLiBDTbLXPZbFBIvddEhWwkhdvqcJWPSuIYTDqRIVMWxenCdSYOJvrPOqMZHoCHXimRLQHZzOSVdFHlgpAaZdHdAlCsZmrdLHJBEPqViFxDsKXdFPaRbXreyzNsYqPEBUYseblSFFdAafWwzHIqWkybpHVixaTMrqEXpNPvNReaCXjNHeaYpEFoaQzpPxjPjPNcvjBjXUZRKqJtoGqQytAXlVmbzoZsCVczByPQWooCPZbgKsSWxmnmomYvVlPKzTGbJByFQdrMaNCYSZXpnoGnlLrSPbmyeWlQDqHTjlSTniQbwwZsEWhkSTQgjfkGsLoQlEgoUhZGsFEuIizuQOgcKLvmoOPPhWRyQjPPmZNOWTpyKSwaVYkYXfxBGvWXvMiZJWzfTvtFTOeqrKEBQzkxiVpSFasClNOzUzNRVXFjHqvPZAlDCxYWtlMowxUhgUGqlXzhpXhtdpxnRhNaLXHDDcGFmJoCxTXQuHxVevtCxzUyEypEouyajbmhmuQMTUzjTdMQloMalcBEblxyPkPxwTARgAPuQGPAeHEAqrNYDVsklBkDnUViClppVfBoMWOVSzlsKslxzkhdLjaHrUViDvycyeoMdAcdIblwxAzIxWPfsypJvIFwPbECf";

  for (int i = 0; i < 100; i++) {
    char key[50];
    sprintf(key, "key_%d", i);
    hash_table_insert(ht, key, str, strlen(str), HASH_TYPE_STRING);
  }

  hash_table_print(ht, 2);

  printf("SIZE: %zu\n", ht->size);
  printf("CAPACITY: %zu\n\n", ht->capacity);

  // TODO: Fix the free
  hash_table_free(ht);

  return 0;
}

void *hash_table_get(HashTable *ht, char *key) {
  int i = djb2(key, ht->capacity);

  printf("%d\n", i);
  HashNode *hn = ht->nodes[i];

  while (hn != NULL) {
    if (memcmp(hn->key, key, strlen(key) + 1) == 0) {
      return hn;
    }
    hn = hn->next;
  }

  return NULL;
}

void hash_table_print(HashTable *ht, int identation) {
  for (int j = 0; j < identation; j++)
    printf(" ");
  printf("{\n");
  for (size_t i = 0; i < ht->capacity; i++) {
    if (ht->nodes[i] == NULL) {
      continue;
    }
    HashNode *hn = ht->nodes[i];
    while (hn != NULL) {
      for (int j = 0; j < identation + 2; j++)
        printf(" ");
      printf("\"%s\": ", hn->key);
      hash_node_print(hn, identation);
      printf("\n");
      hn = hn->next;
    }
  }
  for (int j = 0; j < identation; j++)
    printf(" ");
  printf("}\n");
}

void hash_node_print(HashNode *hn, int identation) {
  if (hn == NULL) {
    return;
  }

  if (hn->type == HASH_TYPE_HASH_TABLE) {
    printf("\n");
    hash_table_print(hn->value, identation + 2);
  } else if (hn->type == HASH_TYPE_CHAR || hn->type == HASH_TYPE_STRING) {
    printf("%s", (char *)hn->value);
  } else if (hn->type == HASH_TYPE_INT) {
    printf("%d", *(int *)hn->value);
  }
}

void hash_table_realloc(HashTable *ht) {
  ht->capacity *= 2;
  ht->nodes = realloc(ht->nodes, ht->capacity * sizeof(HashNode *));
  int *realloc_indexs = calloc(ht->capacity, sizeof(int));

  for (size_t i = 0; i < ht->capacity; i++) {
    if (i >= ht->capacity / 2) {
      ht->nodes[i] = NULL;
    } else if (ht->nodes[i] != NULL) {
      int nextIndex = djb2(ht->nodes[i]->key, ht->capacity);

      if (realloc_indexs[i] == 0 || (size_t)nextIndex == i) {
        ht->nodes[i]->next = ht->nodes[nextIndex];
        ht->nodes[nextIndex] = ht->nodes[i];
        ht->nodes[i] = 0;
        realloc_indexs[nextIndex] = 1;
      }
    }
  }
}

void hash_table_insert(HashTable *ht, char *k, void *v, size_t vs,
                       HashType vt) {
  if (ht->size >= ht->capacity) {
    hash_table_realloc(ht);
  }

  int i = djb2(k, ht->capacity);
  HashNode *hn = create_hash_node(k, v, vs, vt);

  hn->next = ht->nodes[i];
  ht->nodes[i] = hn;
  ht->size++;
}

HashTable *create_hash_table(size_t c) {
  HashTable *ht = (HashTable *)malloc(sizeof(HashTable));

  ht->size = 0;
  ht->capacity = c;
  ht->nodes = (HashNode **)calloc(c, sizeof(HashNode *));

  return ht;
}

HashNode *create_hash_node(char *k, void *v, size_t vs, HashType vt) {
  HashNode *hn = (HashNode *)malloc(sizeof(HashNode));

  hn->key = malloc(strlen(k) + 1);
  memcpy(hn->key, k, strlen(k) + 1);

  if (vt == HASH_TYPE_HASH_TABLE) {
    hn->value = v;
  } else {
    hn->value = malloc(vs);
    memcpy(hn->value, v, vs);
  }

  hn->next = NULL;
  hn->type = vt;

  return hn;
}

int djb2(char *key, size_t size) {
  unsigned long index = 5381;
  char *k = key;
  int c = *k;

  while (c) {
    index = ((index << 5) + index) + c;
    c = *++k;
  }

  return index % size;
}

void hash_table_free(HashTable *ht) {
  for (size_t i = 0; i < ht->capacity; i++) {
    hash_node_free(ht->nodes[i]);
  }
  free(ht);
}

void hash_node_free(HashNode *hn) {
  HashNode *n = hn->next;

  if (hn->type == HASH_TYPE_HASH_TABLE) {
    hash_table_free(hn->value);
  } else {
    free(hn->value);
  }

  while (n != NULL) {
    HashNode *tmp = hn->next;
    hash_node_free(n);
    n = tmp;
  }

  free(hn->key);
  free(hn);
}
