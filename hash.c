#include <stdio.h>
#include <stdlib.h>

typedef struct HashNode {
  char *key;
  void *value;
  struct HashNode *next;
} HashNode;

typedef struct {
  HashNode **values;
  size_t capacity;
  size_t size;
} HashTable;

HashNode* create_hash_node(char *key, void *value);
HashTable* create_hash_table(size_t capacity);
void hash_table_insert(HashTable *ht, char *key, void *value);
void hash_table_print(HashTable *ht);
int djb2(char *key, size_t size);

int main() {
  HashTable *ht = create_hash_table(512);
  int i = 1;
  char a = 'a';
  char str[3] = "abc";
  hash_table_insert(ht, "version", &i);
  hash_table_insert(ht, "apple", &a);
  hash_table_insert(ht, "orange", str);
  hash_table_print(ht);
  printf("SIZE: %zu\n", ht->size);
  printf("SIZE: %zu\n", ht->capacity);
  return 0;
}

void hash_table_print(HashTable *ht) {
  for (size_t i = 0; i < ht->capacity; i++) {
    if (ht->values[i] == NULL) {
      continue;
    }
    HashNode *hn = ht->values[i];
    while(hn != NULL) {
      printf("Node[%zu] { \"%s\": %s}\n", i, hn->key, (char *)hn->value);
      hn = hn->next;
    }
  }
}

void hash_table_insert(HashTable *ht, char *k, void *v) {
  if (ht->size * sizeof(HashTable) >= ht->capacity) {
    ht->capacity *= 2;
    ht->values = realloc(ht->values, ht->capacity);
  }

  int i = djb2(k, ht->capacity);
  HashNode *hn = create_hash_node(k, v);

  if (ht->values[i] == NULL) {
    ht->values[i] = hn;
    ht->size++;
    return;
  }

  HashNode *chn = ht->values[i];
  while(chn != NULL) {
    if (chn->next == NULL) {
      chn->next = hn;
      break;
    }
    chn = chn->next;
  }
  ht->size++;
}

HashTable* create_hash_table(size_t c) {
  HashTable *ht = (HashTable*)malloc(sizeof(HashTable));

  ht->size = 0;
  ht->capacity = c;
  ht->values = (void*)calloc(c, sizeof(HashNode*));

  return ht;
}

HashNode *create_hash_node(char *k, void *v) {
  HashNode *hn = (HashNode*)malloc(sizeof(HashNode));

  hn->key = k;
  hn->value = v;
  hn->next = NULL;

  return hn;
}

int djb2(char *key, size_t size) {
  unsigned long index = 5381;
  char *k = key;
  int c = *k;

  while(c) {
    index = ((index << 5) + index) + c;
    c = *k++;
  }

  return index % size;
}
