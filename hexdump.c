#include <astro/cutils.h>
#include <errno.h>

#define N 16

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
  long readen_bytes = 0;
  u8 *file = read_file(argv[1], &readen_bytes);
  if (!file) {
    fprintf(stderr, "[ERROR]: %s\n", strerror(errno));
    return -1;
  }

  int j = 0;
  size_t x = 0;

  if (readen_bytes % 16 == 0) {
    x = readen_bytes;
  } else {
    x = readen_bytes % 16;
  }

  size_t i = 0;

  while (i < readen_bytes) {

    if ((i % N) == 0) {
      printf("| %08X |   ", j);
      j += N;
    }

    printf("%02X ", file[i]);

    if ((i % N) == (N - 1)) {
      printf("   ");
      print_ascii(file, i - (N - 1), i + 1);
      printf("\n");
    }

    i++;
  }

  if (x < readen_bytes) {
    int spaces = ((N - x) * 3) + 3;
    i -= x;
    print_spaces(spaces);
    print_ascii(file, i, i + x);
  }

  return 0;
}