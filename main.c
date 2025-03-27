#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "unistd.h"
#include "string.h"


#define BUFFER_SIZE 32
static const size_t bufferSize = BUFFER_SIZE;

typedef enum {
  RPC_STATE_CONTENT_LENGTH,
  RPC_STATE_CONTENT_TYPE,
  RPC_STATE_CONTENT,
} RPCReadState;

typedef struct {
  char *data;
  char *current_pos;
  size_t size;
  size_t capacity;
} Buffer;

typedef struct {
  int contentLength;
  char *contentType;
  Buffer *content;
} RPCJsonMetadata;

RPCJsonMetadata* mount_input(Buffer *buff);
Buffer* read_input(int fd);
Buffer* create_buffer();
void free_buffer(Buffer *buff);
void free_rpc_json(RPCJsonMetadata *rpcJson);
void toLowerCase(char *str);
int compare_key(Buffer *buff, char *key);

int main() {
  FILE *json = fopen("./package.json", "r");

  if (json == NULL) {
    fprintf(stderr, "Could not open json\n");
    return 1;
  }

  int fd = fileno(json);

  Buffer *buff = read_input(fd);

  if (buff == NULL) {
    fprintf(stderr, "Could create buffer\n");
    return 1;
  }

  RPCJsonMetadata *rpcJsonMetadata = mount_input(buff);

  if (rpcJsonMetadata == NULL) {
    free_buffer(buff);
    fprintf(stderr, "Could not parse RPCJson\n");
    return 1;
  }

  printf("BUFFER-LENGTH: %zu\n", buff->size);
  printf("BUFFER-CAPACITY: %zu\n", buff->capacity);
  printf("CONTENT-LENGTH: %d\n", rpcJsonMetadata->contentLength);
  printf("CONTENT-TYPE: %s\n", rpcJsonMetadata->contentType);
  printf("CONTENT: %s\n", rpcJsonMetadata->content->data);

  free_rpc_json(rpcJsonMetadata);
  free_buffer(buff);

  return 0;
}

Buffer* create_buffer() {
  Buffer* buff = (Buffer*)malloc(sizeof(Buffer));

  buff->capacity = bufferSize;
  buff->size = 0;
  buff->data = malloc(bufferSize);
  buff->current_pos = buff->data;

  return buff;
}

Buffer* read_input(int fd) {
  Buffer* buff = create_buffer();
  char bufferTmp[bufferSize];
  int running = 1;

  while(running) {
    int bc = read(fd, bufferTmp, bufferSize);
    if (bc < BUFFER_SIZE) {
      running = 0;
    }

    if (buff->size + bc > buff->capacity) {
      buff->capacity *= 2;
      buff->data = realloc(buff->data, buff->capacity);

      if (buff->data == NULL) {
        free_buffer(buff);
        return NULL;
      }
    }

    memcpy(buff->data + buff->size, bufferTmp, bc);
    buff->size += bc;
  }

  return buff;
}

RPCJsonMetadata* mount_input(Buffer *buff) {
  RPCJsonMetadata *rpcJsonMetadata = (RPCJsonMetadata*)malloc(sizeof(RPCJsonMetadata));

  char contentLengthKey[16] = "content-length: ";
  char contentTypeKey[14] = "content-type: ";
  size_t bytes_moved = 0;
  int state = RPC_STATE_CONTENT_LENGTH;

  rpcJsonMetadata->content = create_buffer();

  if (rpcJsonMetadata->content == NULL) {
    free_rpc_json(rpcJsonMetadata);
    return NULL;
  }

  if (compare_key(buff, contentLengthKey) != 0) {
    free_rpc_json(rpcJsonMetadata);
    return NULL;
  }

  buff->current_pos += sizeof(contentLengthKey) - 2;

  while(bytes_moved < buff->size) {
    if (buff->current_pos[bytes_moved] == '\r' && buff->current_pos[bytes_moved + 1] == '\n') {
      buff->current_pos += 2;
      switch (state) {
        case (RPC_STATE_CONTENT_LENGTH): {
          char tmpBuff[bytes_moved];
          state = RPC_STATE_CONTENT_TYPE;
          tmpBuff[bytes_moved] = '\0';
          memcpy(tmpBuff, buff->current_pos, bytes_moved);
          rpcJsonMetadata->contentLength = atoi(tmpBuff);
          buff->current_pos += bytes_moved;
          bytes_moved = 0;

          if (compare_key(buff, contentTypeKey) != 0) {
            state = RPC_STATE_CONTENT;
            buff->current_pos += 2;
            break;
          }

          buff->current_pos += sizeof(contentTypeKey) - 2;
          break;
        }

        case (RPC_STATE_CONTENT_TYPE):
          state = RPC_STATE_CONTENT;
          rpcJsonMetadata->contentType = malloc(bytes_moved);
          memcpy(rpcJsonMetadata->contentType, buff->current_pos, bytes_moved - 1);
          rpcJsonMetadata->contentType[bytes_moved] = '\0';
          buff->current_pos += bytes_moved + 2;
          break;
      }
      continue;
    }

    bytes_moved++;

    if (state == RPC_STATE_CONTENT) {
      rpcJsonMetadata->content->capacity = rpcJsonMetadata->contentLength + 1;
      rpcJsonMetadata->content->size = rpcJsonMetadata->contentLength + 1;
      rpcJsonMetadata->content->data = realloc(rpcJsonMetadata->content->data, rpcJsonMetadata->content->capacity);
      memcpy(rpcJsonMetadata->content->data, buff->current_pos, rpcJsonMetadata->contentLength);
      rpcJsonMetadata->content->data[rpcJsonMetadata->contentLength + 1] = '\0';
      return rpcJsonMetadata;
    }
  }

  free_rpc_json(rpcJsonMetadata);
  return NULL;
}

int compare_key(Buffer *buff, char *key) {
  char *keyReaded = malloc(sizeof(key) + 1);
  memcpy(keyReaded, buff->current_pos, sizeof(*key));
  keyReaded[sizeof(key) + 1] = '\0';
  toLowerCase(keyReaded);
  int result = memcmp(keyReaded, key, sizeof(*key));
  free(keyReaded);
  return result;
}

void free_buffer(Buffer *buff) {
  if (buff != NULL) {
    free(buff->data);
    free(buff);
  }
}
void free_rpc_json(RPCJsonMetadata *rpcJson) {
  if (rpcJson != NULL) {
    free_buffer(rpcJson->content);
    free(rpcJson->contentType);
    free(rpcJson);
  }
}

void toLowerCase(char *str) {
  char *aux = str;

  while(*aux != '\0') {
    if (*aux >= 65 && *aux <= 90) {
      *aux = *aux + 32;
    }
    aux++;
  }
}
