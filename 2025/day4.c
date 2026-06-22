#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *read_line(FILE *f) {
  size_t size = 128;
  size_t len = 0;
  char *buffer = malloc(sizeof(char) * 128);

  if (!buffer) {
    return NULL;
  }

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

int compte(FILE *f) {
  char *line = read_line(f);
  int n = strlen(line);
  int **matrice = malloc(sizeof(int *) * n);
  int count = 0;

  for (int i = 0; i < n; i++) {
    matrice[i] = malloc(sizeof(int) * n);
  }
  int i = 0;
  while (i < n && line != NULL) {
    for (int j = 0; j < n; j++) {
      if (line[j] == '@') {
        matrice[i][j] = 1;
      }
    }
    line = read_line(f);
    i++;
  }

  int **matrice_count = malloc(sizeof(int *) * n);
  for (int i = 0; i < n; i++) {
    matrice_count[i] = malloc(sizeof(int) * n);
  }

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      int has_roll = matrice[i][j];
      if (has_roll) {
        for (int delta_x = (i - 1 > 0 ? i - 1 : 0);
             delta_x <= (i + 1 < n ? i + 1 : n - 1); delta_x++) {
          for (int delta_y = (j - 1 > 0 ? j - 1 : 0);
               delta_y <= (j + 1 < n ? j + 1 : n - 1); delta_y++) {
            if (delta_x != i || delta_y != j) {
              matrice_count[delta_x][delta_y] += has_roll;
            }
          }
        }
      }
    }
  }

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (matrice[i][j] == 1 && matrice_count[i][j] < 4) {
        count += 1;
      }
    }
    free(matrice[i]);
  }
  free(matrice);
  free(matrice_count);
  free(line);
  return count;
}

int removed(FILE *f) {
  char *line = read_line(f);
  int n = strlen(line);
  int **matrice = malloc(sizeof(int *) * n);
  int count = 0;
  int removed = 1;

  for (int i = 0; i < n; i++) {
    matrice[i] = malloc(sizeof(int) * n);
  }
  int i = 0;
  while (i < n && line != NULL) {
    for (int j = 0; j < n; j++) {
      if (line[j] == '@') {
        matrice[i][j] = 1;
      } else {
        matrice[i][j] = 0;
      }
    }
    line = read_line(f);
    i++;
  }

  int **matrice_count = malloc(sizeof(int *) * n);
  for (int i = 0; i < n; i++) {
    matrice_count[i] = malloc(sizeof(int) * n);
    for (int j = 0; j < n; j++) {
      matrice_count[i][j] = 0;
    }
  }
  while (removed != 0) {
    removed = 0;
    for (int i = 0; i < n; i++)
      for (int j = 0; j < n; j++)
        matrice_count[i][j] = 0;
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        int has_roll = matrice[i][j];
        if (has_roll) {
          for (int delta_x = (i - 1 >= 0 ? i - 1 : 0);
               delta_x <= (i + 1 < n ? i + 1 : n - 1); delta_x++) {
            for (int delta_y = (j - 1 >= 0 ? j - 1 : 0);
                 delta_y <= (j + 1 < n ? j + 1 : n - 1); delta_y++) {
              if (delta_x != i || delta_y != j) {
                matrice_count[delta_x][delta_y] += has_roll;
              }
            }
          }
        }
      }
    }

    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        if (matrice[i][j] == 1 && matrice_count[i][j] < 4) {
          removed += 1;
          matrice[i][j] = 0;
        }
      }
    }
    printf("%i\n", removed);
    count += removed;
  }
  for (int i = 0; i < n; i++) {
    free(matrice[i]);
    free(matrice_count[i]);
  }
  free(matrice);
  free(matrice_count);
  free(line);
  return count;
}

int main(int argc, char **argv) {
  if (argc < 2) {
    fprintf(stderr, "Utilisation incorrecte : %s <input>", argv[0]);
  }

  FILE *f = fopen(argv[1], "r");

  if (!f) {
    return 1;
  }

  printf("Et le résultaaaaat : %i\n", removed(f));

  return 0;
}