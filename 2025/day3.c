#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *read_line(FILE *f) {
  size_t size = 128;
  size_t len = 0;
  char *buffer = malloc(size);

  if (!buffer)
    return NULL;

  int c;
  while ((c = fgetc(f)) != EOF && c != '\n') {
    if (len + 1 >= size) {
      size_t new_size = size * 2;
      char *new_buffer = realloc(buffer, new_size);
      if (!new_buffer) {
        free(buffer);
        return NULL;
      }
      buffer = new_buffer;
      size = new_size;
    }
    buffer[len++] = c;
  }

  if (c == EOF && len == 0) {
    free(buffer);
    return NULL;
  }

  buffer[len] = '\0';
  return buffer;
}

int max_per_line(FILE *file) {
  char *line;
  int count = 0;

  while ((line = read_line(file)) != NULL) {
    printf("On traite la ligne %s\n", line);
    int n = strlen(line);
    int max1 = 0;
    int max2 = 0;
    for (int i = 0; i < n; i++) {
      int int_read = line[i] - '0';
      if (i < n - 1 && int_read > max1) {
        max1 = int_read;
        max2 = 0;
      } else if (int_read > max2) {
        max2 = int_read;
      }
    }
    char str[3];

    str[0] = max1 + '0';
    str[1] = max2 + '0';
    str[2] = '\0';
    int found = (int)strtol(str, NULL, 10);
    count += found;
    printf("On a trouvé %i.\n\n", found);
    free(line);
  }

  return count;
}

long long int max12_per_line(FILE *file) {
  char *line;
  long long int count = 0;

  while ((line = read_line(file)) != NULL) {
    int n = strlen(line);
    int to_keep = 12;
    int to_remove = n - to_keep;
    char *stack = malloc(sizeof(char) * (n + 1));
    int cursor = 0;
    for (int i = 0; i < n; i++) {
      char int_read = line[i];
      while (cursor > 0 && stack[cursor - 1] < int_read && to_remove > 0) {
        cursor--;
        to_remove--;
      }
      stack[cursor++] = int_read;
    }
    stack[to_keep] = '\0';
    long long int result_line = strtoll(stack, NULL, 10);
    count += result_line;
    printf("Pour la ligne %s je trouve : %lli. Soit un total de %lli\n", line, result_line, count);
    free(stack);
  }
  return count;
}

int main(int argc, char **argv) {
  if (argc < 2) {
    fprintf(stderr, "Format : %s fichier_input\n", argv[0]);
    return 1;
  }

  FILE *f = fopen(argv[1], "r");
  printf("Et le résultaaaaat : %lli\n", max12_per_line(f));

  return 0;
}