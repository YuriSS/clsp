#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
  HASH_TYPE_INT,
  HASH_TYPE_CHAR,
  HASH_TYPE_STRING,
  HASH_TYPE_HASH_TABLE
} HashType;

typedef struct HashNode {
  char *key;
  void *value;
  HashType valueType;
  struct HashNode *next;
} HashNode;

typedef struct {
  HashNode **values;
  size_t capacity;
  size_t size;
} HashTable;

HashNode* create_hash_node(char *key, void *value, size_t valueSize, HashType valueType);
HashTable* create_hash_table(size_t capacity);
void hash_table_insert(HashTable *ht, char *key, void *value, size_t valueSize, HashType valueType);
void hash_table_print(HashTable *ht, int identation);
void hash_node_print(HashNode *hn, int identation);
int djb2(char *key, size_t size);

int main() {
  HashTable *ht = create_hash_table(512);
  HashTable *ht2 = create_hash_table(128);
  HashTable *ht3 = create_hash_table(128);
  int i = 1;
  char a = 'a';
  char str[3] = "abc";

  hash_table_insert(ht, "version", &i, sizeof(int), HASH_TYPE_INT);
  hash_table_insert(ht, "apple", &a, sizeof(char), HASH_TYPE_CHAR);
  hash_table_insert(ht, "orange", str, sizeof(str), HASH_TYPE_STRING);

  hash_table_insert(ht2, "version", &i, sizeof(int), HASH_TYPE_INT);
  hash_table_insert(ht2, "apple", &a, sizeof(char), HASH_TYPE_CHAR);
  hash_table_insert(ht2, "orange", ht, sizeof(*ht), HASH_TYPE_HASH_TABLE);

  hash_table_insert(ht3, "version", &i, sizeof(int), HASH_TYPE_INT);
  hash_table_insert(ht3, "apple", &a, sizeof(char), HASH_TYPE_CHAR);
  hash_table_insert(ht3, "orange", ht2, sizeof(*ht2), HASH_TYPE_HASH_TABLE);

  hash_table_print(ht3, 2);

  printf("SIZE: %zu\n", ht->size);
  printf("SIZE: %zu\n", ht->capacity);
  return 0;
}

void hash_table_print(HashTable *ht, int identation) {
  for (int j = 0; j < identation; j++) printf(" ");
  printf("{\n");
  for (size_t i = 0; i < ht->capacity; i++) {
    if (ht->values[i] == NULL) {
      continue;
    }
    HashNode *hn = ht->values[i];
    while(hn != NULL) {
      for (int j = 0; j < identation + 2; j++) printf(" ");
      printf("\"%s\": ", hn->key);
      hash_node_print(hn, identation);
      printf("\n");
      hn = hn->next;
    }
  }
  for (int j = 0; j < identation; j++) printf(" ");
  printf("}\n");
}

void hash_node_print(HashNode *hn, int identation) {
  if (hn == NULL) {
    return;
  }

  if (hn->valueType == HASH_TYPE_HASH_TABLE) {
    printf("\n");
    hash_table_print(hn->value, identation + 2);
  }
  else if(hn->valueType == HASH_TYPE_CHAR || hn->valueType == HASH_TYPE_STRING) {
    printf("%s", (char*)hn->value);
  }
  else if(hn->valueType == HASH_TYPE_INT) {
    printf("%d", *(int*)hn->value);
  }
}

void hash_table_insert(HashTable *ht, char *k, void *v, size_t vs, HashType vt) {
  if (ht->size >= ht->capacity) {
    ht->capacity *= 2;
    ht->values = realloc(ht->values, ht->capacity * sizeof(HashNode*));

    for (size_t i = ht->capacity / 2; i < ht->capacity; i++) {
      ht->values[i] = NULL;
    }
  }

  int i = djb2(k, ht->capacity);
  HashNode *hn = create_hash_node(k, v, vs, vt);

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
  ht->values = (HashNode**)calloc(c, sizeof(HashNode*));

  return ht;
}

HashNode *create_hash_node(char *k, void *v, size_t vs, HashType vt) {
  HashNode *hn = (HashNode*)malloc(sizeof(HashNode));

  hn->key = malloc(strlen(k) + 1);
  memcpy(hn->key, k, strlen(k) + 1);

  hn->value = malloc(vs);
  memcpy(hn->value, v, vs);

  hn->next = NULL;
  hn->valueType = vt;

  return hn;
}

int djb2(char *key, size_t size) {
  unsigned long index = 5381;
  char *k = key;
  int c = *k;

  while(c) {
    index = ((index << 5) + index) + c;
    c = *++k;
  }

  return index % size;
}
