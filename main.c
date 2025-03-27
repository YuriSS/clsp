#include "stdio.h"
#include <stdlib.h>
#include <unistd.h>

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
  char buffer[128];
  char contentBuffer[128];
  int i = 0;
  int j = 0;
  int fd = fileno(json);

  read(fd, buffer, 128);

  while(1) {
    if (buffer[i] == '\n' || buffer[i] == '\0' || buffer[i] == '\r') {
      break;
    }

    if (buffer[i] >= 48 && buffer[i] <= 57) {
      contentBuffer[j++] = buffer[i];
    }
    i++;
  }

  printf("%d\n", atoi(contentBuffer));

  return 0;
}


RPCJson* mount_input(int file_descriptor) {
  RPCJson *rpcJson = (RPCJson*)malloc(sizeof(RPCJson));
}

