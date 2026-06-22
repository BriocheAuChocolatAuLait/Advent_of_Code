

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct move_s {
  char dir;
  int value;
} move;

int read_line(const char *line, move *move) {
  move->dir = line[0];
  move->value = atoi(line + 1);
  return 1;
}

size_t parse_file(const char *filename, move **out_steps) {
  FILE *f = fopen(filename, "r");
  if (!f)
    return 0;

  move *steps = NULL;
  size_t count = 0;
  char line[128];

  while (fgets(line, sizeof(line), f)) {
    line[strcspn(line, "\r\n")] = '\0';
    steps = realloc(steps, (count + 1) * sizeof(move));
    if (!steps) {
      fclose(f);
      return 0;
    }
    if (!read_line(line, &steps[count])) {
      free(steps);
      fclose(f);
      return 0;
    }
    count++;
  }
  fclose(f);

  *out_steps = steps;
  return count;
}

int code(move *move, int n) {
  int count_0 = 0;
  int current_number = 50;
  for (int i = 0; i < n; i++) {
    if (move[i].dir == 'L') {
      current_number -= move[i].value;
    } else {
      current_number += move[i].value;
    }
    current_number %= 100;
    if (current_number < 0)
      current_number += 100;
    if (current_number == 0)
      count_0++;
  }
  return count_0;
}

int code2(move *move, int n) {
  int count_0 = 0;
  int current_number = 50;
  for (int i = 0; i < n; i++) {
    count_0 += move[i].value / 100;
    if (move[i].dir == 'L') {
        if(!current_number) 
            count_0 --;
      current_number -= move[i].value%100;
    } else {
      current_number += move[i].value%100;
    }
    int temp = current_number;
    current_number %= 100;
    if (current_number < 0) {
      current_number += 100;
    }
    if(temp != current_number || !current_number) 
        count_0 ++;
  }
  return count_0;
}

int main(int argc, char **argv) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s fichier_input\n", argv[0]);
    return 1;
  }

  move *move = NULL;
  size_t count = parse_file(argv[1], &move);
  if (count == 0) {
    fprintf(stderr, "Erreur dans le parsing du fichier.\n");
    return 1;
  }
  printf("Le nombre de retour en 0 est %i\n", code(move, count));
  printf("Le vrai code en vrai c'est %i\n", code2(move, count));

  free(move);
  return 0;
}
