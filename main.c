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

typedef struct {
  char *data;
  char *current_pos;
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

RPCJson* mount_input(Buffer *buff) {
  RPCJson *rpcJson = (RPCJson*)malloc(sizeof(RPCJson));
  char contentLengthKey[16] = "Content-Length: ";
  char contentTypeKey[14] = "Content-Type: ";
  size_t i = 0;
  int state = RPC_STATE_CONTENT_LENGTH;

  // TODO: Make contentLengthKey lowerCase
  if (memcmp(buff->current_pos, contentLengthKey, sizeof(contentLengthKey)) != 0) {
    free_rpc_json(rpcJson);
    return NULL;
  }

  buff->current_pos += sizeof(contentLengthKey) - 2;

  while(i < buff->size) {
    if (buff->current_pos[i] == '\r' && buff->current_pos[i + 1] == '\n') {
      buff->current_pos += 2;
      switch (state) {
        case (RPC_STATE_CONTENT_LENGTH): {
          char tmpBuff[i];
          state = RPC_STATE_CONTENT_TYPE;
          tmpBuff[i] = '\0';
          memcpy(tmpBuff, buff->current_pos, i);
          rpcJson->contentLength = atoi(tmpBuff);
          buff->current_pos += i;
          i = 0;

          // TODO: use memcmp and lowerCase
          if (memcmp(buff->data, contentTypeKey, sizeof(contentTypeKey))) {
            state = RPC_STATE_CONTENT;
            buff->current_pos += 2;
          }
          break;
        }

        case (RPC_STATE_CONTENT_TYPE):
          state = RPC_STATE_CONTENT;
          buff->current_pos += sizeof(contentTypeKey);
          rpcJson->contentType = malloc(i - sizeof(contentTypeKey));
          memcpy(rpcJson->contentType, buff->data, i - sizeof(contentTypeKey));
          rpcJson->contentType[i - sizeof(contentTypeKey)] = '\0';
          buff->current_pos += (i - sizeof(contentTypeKey) + 2);
          break;
      }
      continue;
    }

    i++;

    if (state == RPC_STATE_CONTENT) {
      rpcJson->content = malloc(rpcJson->contentLength + 1);
      memcpy(rpcJson->content, buff->current_pos, rpcJson->contentLength);
      rpcJson->content[rpcJson->contentLength + 1] = '\0';
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
