#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef uint32_t u32;
typedef const char *string;
typedef char byte;
typedef uint8_t u8;

#define N 16

long get_file_size(FILE *file);
u8 *read_file(string filename, long *readen_bytes);
void print_spaces(size_t n);
void print_ascii(u8 *bytes, size_t start, size_t end);

u8 *read_file(string filename, long *readen_bytes) {
  FILE *file;
  u8 *buf;

  file = fopen(filename, "rb");
  if (!file)
    goto error;

  long file_size = get_file_size(file);
  buf = malloc(file_size);
  if (!buf)
    goto error;

  memset(buf, 0, file_size);

  size_t bytes_read = fread(buf, 1, file_size, file);
  if (bytes_read < file_size)
    goto error;

  *readen_bytes = bytes_read;
  return buf;

error:
  if (file)
    fclose(file);

  if (buf)
    free(buf);

  return NULL;
}

long get_file_size(FILE *file) {
  fseek(file, 0, SEEK_END);
  long end = ftell(file);
  fseek(file, 0, SEEK_SET);
  return end;
}

void print_spaces(size_t n) {
  for (int i = 0; i < n; i++)
    putchar(' ');
}

void print_ascii(u8 *bytes, size_t start, size_t end) {
  printf("| ");
  for (size_t i = start; i < end; i++) {
    if (bytes[i] >= ' ' && bytes[i] <= '~') {
      putchar(bytes[i]);
    } else {
      putchar('.');
    }
  }

  if ((end - start) < N) {
    print_spaces(N - (end - start));
  }

  printf(" |");
}

int main(int argc, char **argv) {

  if (argc < 2) {
    fprintf(stderr, "Usage:\n  %s <file>\n", argv[0]);
    exit(-1);
  }

  long readen_bytes = 0;
  u8 *file = read_file(argv[1], &readen_bytes);
  if (!file) {
    fprintf(stderr, "[ERROR]: %s\n", strerror(errno));
    return -1;
  }

  int offset = 0;
  size_t rest = 0;

  if (readen_bytes % N == 0) {
    rest = readen_bytes;
  } else {
    rest = readen_bytes % N;
  }

  size_t i = 0;
  while (i < readen_bytes) {

    if ((i % N) == 0) {
      printf("| %08X |   ", offset);
      offset += N;
    }

    printf("%02X ", file[i]);

    if ((i % N) == (N - 1)) {
      printf("   ");
      print_ascii(file, i - (N - 1), i + 1);
      printf("\n");
    }

    i++;
  }

  if (rest < readen_bytes) {
    int spaces = ((N - rest) * 3) + 3;
    print_spaces(spaces);
    print_ascii(file, i - rest, i);
  }

  printf("\n");

  return 0;
}
