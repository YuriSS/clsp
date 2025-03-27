#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "unistd.h"
#include "string.h"

#define BUFFER_SIZE 8
static const size_t bufferSize = BUFFER_SIZE;

typedef enum {
  CONTENT_LENGTH,
  CONTENT_TYPE,
  END_OF_HEADER,
  CONTENT,
  EXIT
} RPCReadState;

typedef struct {
  int contentLength;
  char *contentType;
  char *content;
} RPCJson;

RPCJson* mount_input(int file_descriptor);
char* get_content_length(int file_descriptor);
char* get_content_type(int file_descriptor);
char* get_content(int file_descriptor);

int main() {
  FILE *json = fopen("./package.json", "r");

  if (json == NULL) {
    fprintf(stderr, "Could not open json\n");
    return 1;
  }

  int fd = fileno(json);

  RPCJson *rpcJson = mount_input(fd);

  if (rpcJson == NULL) {
    fprintf(stderr, "Could not parse RPCJson\n");
    return 1;
  }

  printf("CONTENT-LENGTH: %d\n", rpcJson->contentLength);
  printf("CONTENT-TYPE: %s\n", rpcJson->contentType);

  return 0;
}


RPCJson* mount_input(int fd) {
  RPCJson *rpcJson = (RPCJson*)malloc(sizeof(RPCJson));
  char *buffer = malloc(bufferSize);
  char *bufferTmp = malloc(bufferSize);
  char clKey[128];
  char clValue[128];
  char ctKey[128];
  char ctValue[128];
  int i = 0;
  int j = 0;
  int k = 0;
  int l = 0;
  int m = 0;
  int state = CONTENT_LENGTH;
  int keepGoing = 1;
  size_t currentSize = 0;
  size_t bufferLength = bufferSize;

  while (keepGoing) {
    int bc = read(fd, bufferTmp, bufferSize);
    if (bc < BUFFER_SIZE) {
      keepGoing = 0;
    }

    if (keepGoing == 1) {
      bufferLength *= 2;
      buffer = realloc(buffer, bufferLength);

      if (buffer == NULL) {
        return NULL;
      }
    }

    memcpy(buffer + currentSize, bufferTmp, bc);
    currentSize += bc;
  }

  free(bufferTmp);

  keepGoing = 1;
  int first_time = 1;

  while(keepGoing) {
    if (buffer[i] == '\r' && buffer[i + 1] == '\n') {
      i += 2;
      state++;
      continue;
    }

    switch (state) {
      case CONTENT_LENGTH:
        if (buffer[i] >= 48 && buffer[i] <= 57) {
          // TODO Make contentLengthKey lowerCase
          if (memcmp(clKey, "Content-Length: ", 16) != 0) {
            return NULL;
          }
          clValue[j++] = buffer[i++];
        }
        else {
          clKey[k++] = buffer[i++];
        }
        break;

      case CONTENT_TYPE:
        if (buffer[i] == 58 && buffer[i+1] == 32 && first_time == 1) {
          i+=2;
          first_time = 0;
        }
        else if(first_time == 0) {
          // TODO use memcmp and lowerCase
          if (memcmp(ctKey, "Content-Type", 12) != 0) {
            return NULL;
          }
          ctValue[l++] = buffer[i++];
        }
        else {
          ctKey[m++] = buffer[i++];
        }
        break;

      case CONTENT:
        i++;
        //printf("CONTENT %c\n", buffer[i++]);
        break;

      case(EXIT):
        printf("EXIT\n");
        keepGoing = 0;
        break;
    }
  }

  ctValue[l] = '\0';
  rpcJson->contentType = malloc(l);
  rpcJson->contentLength = atoi(clValue);
  memcpy(rpcJson->contentType, ctValue, l);

  return rpcJson;
}

