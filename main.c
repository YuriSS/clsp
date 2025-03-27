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
  RPC_STATE_EXIT
} RPCReadState;

typedef struct {
  int contentLength;
  char *contentType;
  char *content;
} RPCJson;

// TODO: Create a index of the current position in the buffer
typedef struct {
  char *data;
  size_t size;
  size_t capacity;
} Buffer;

RPCJson* mount_input(Buffer *buff);
Buffer* read_input(int fd);
Buffer* create_buffer();
void free_buffer(Buffer *buff);
void free_rpc_json(RPCJson *rpcJson);

int main() {
  FILE *json = fopen("./package.json", "r");

  if (json == NULL) {
    fprintf(stderr, "Could not open json\n");
    return 1;
  }

  int fd = fileno(json);

  Buffer *buff = read_input(fd);
  RPCJson *rpcJson = mount_input(buff);

  if (rpcJson == NULL) {
    free_buffer(buff);
    fprintf(stderr, "Could not parse RPCJson\n");
    return 1;
  }

  printf("BUFFER-LENGTH: %zu\n", buff->size);
  printf("CONTENT-LENGTH: %d\n", rpcJson->contentLength);
  printf("CONTENT-TYPE: %s\n", rpcJson->contentType);
  printf("CONTENT: %s\n", rpcJson->content);

  free_rpc_json(rpcJson);
  //free_buffer(buff);

  return 0;
}

Buffer* create_buffer() {
  Buffer* buff = (Buffer*)malloc(sizeof(Buffer));

  buff->capacity = bufferSize;
  buff->size = 0;
  buff->data = malloc(bufferSize);

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

RPCJson* mount_input(Buffer *buff) {
  RPCJson *rpcJson = (RPCJson*)malloc(sizeof(RPCJson));
  size_t i = 0;
  int state = RPC_STATE_CONTENT_LENGTH;

  // TODO: Make contentLengthKey lowerCase
  if (memcmp(buff->data, "Content-Length: ", 16) != 0) {
    free_rpc_json(rpcJson);
    return NULL;
  }

  buff->data += 16;

  // TODO:  handle cases where header dont have content_type
  while(i < buff->size) {
    if (buff->data[i] == '\r' && buff->data[i + 1] == '\n') {
      i += 2;
      switch (state) {
        case (RPC_STATE_CONTENT_LENGTH): {
          char tmpBuff[i];
          state = RPC_STATE_CONTENT_TYPE;
          tmpBuff[i] = '\0';
          memcpy(tmpBuff, buff->data, i);
          rpcJson->contentLength = atoi(tmpBuff);
          buff->data += i;
          i = 0;

          // TODO: use memcmp and lowerCase
          if (memcmp(buff->data, "Content-Type: ", 14)) {
            state = RPC_STATE_CONTENT;
            buff->data += 2;
          }
          break;
        }

        case (RPC_STATE_CONTENT_TYPE):
          state = RPC_STATE_CONTENT;
          buff->data += 14;
          rpcJson->contentType = malloc(i - 16);
          memcpy(rpcJson->contentType, buff->data, i - 16);
          rpcJson->contentType[i - 16] = '\0';
          buff->data += (i - 14 + 2);
          break;
      }
      continue;
    }

    i++;

    if (state == RPC_STATE_CONTENT) {
      rpcJson->content = malloc(rpcJson->contentLength);
      memcpy(rpcJson->content, buff->data, rpcJson->contentLength);
      return rpcJson;
    }
  }

  free_rpc_json(rpcJson);
  return NULL;
}

void free_buffer(Buffer *buff) {
  if (buff != NULL) {
    free(buff->data);
    free(buff);
  }
}
void free_rpc_json(RPCJson *rpcJson) {
  if (rpcJson != NULL) {
    free(rpcJson->content);
    free(rpcJson->contentType);
    free(rpcJson);
  }
}
