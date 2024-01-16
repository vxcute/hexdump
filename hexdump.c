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

  return 0;
}