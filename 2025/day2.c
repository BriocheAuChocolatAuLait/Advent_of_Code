#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
typedef struct ids_s {
  long long int *ids1;
  long long int *ids2;
  long long int nb_ids;
} ids;

int parseur(const char *filename, ids *ids) {
  FILE *f = fopen(filename, "r");
  if (!f)
    return 0;

  ids->ids1 = NULL;
  ids->ids2 = NULL;
  ids->nb_ids = 0;
  char buffer[32768];

  while (fgets(buffer, sizeof(buffer), f)) {

    char *token = strtok(buffer, ",");

    while (token) {

      // nettoyer fin de ligne
      size_t n = strlen(token);
      while (n > 0 && (token[n - 1] == '\n' || token[n - 1] == '\r'))
        token[--n] = '\0';

      char *tiret = strchr(token, '-');
      *tiret = '\0';

      long long int a = atoll(token);
      long long int b = atoll(tiret + 1);
      long long int new_size = ids->nb_ids + 1;

      long long int *temp1 =
          realloc(ids->ids1, new_size * sizeof(long long int));
      long long int *temp2 =
          realloc(ids->ids2, new_size * sizeof(long long int));

      if (!temp1 || !temp2) {
        free(temp1);
        free(temp2);
        fclose(f);
        return 0;
      }

      ids->ids1 = temp1;
      ids->ids2 = temp2;
      ids->nb_ids = new_size;

      ids->ids1[new_size - 1] = a;
      ids->ids2[new_size - 1] = b;

      token = (strtok(NULL, ","));
    }
  }
  fclose(f);
  return 1;
}

long long int verifier_ids(ids *my_ids) {
  long long int count = 0;
  for (int i = 0; i < my_ids->nb_ids; i++) {
    for (long long int j = my_ids->ids1[i]; j <= my_ids->ids2[i]; j++) {
      char buffer[128];

      sprintf(buffer, "%lli", j);
      int n = strlen(buffer);
      if (n % 2 == 0) {

        int curseur = 0;
        while (curseur < (n / 2) &&
               buffer[curseur] == buffer[curseur + (n / 2)]) {
          curseur++;
        }
        if (curseur == n / 2) {
          count += j;
        }
      }
    }
  }
  return count;
}

long long int verifier_ids2(ids *my_ids) {
  long long int count = 0;
  for (int i = 0; i < my_ids->nb_ids; i++) {
    for (long long int j = my_ids->ids1[i]; j <= my_ids->ids2[i]; j++) {
      char buffer[128];
      sprintf(buffer, "%lli", j);
      int n = strlen(buffer);
      int has_pattern = 0;
      for (int len_pattern = 1; len_pattern <= n / 2; len_pattern++) {
        int valid = 1;
        for (int k = 0; k < n; k++) {
          if (buffer[k] != buffer[k % len_pattern]) {
            valid = 0;
            break;
          }
        }
        if (valid && n%len_pattern == 0) {
          has_pattern = 1; // motif trouvé
          break;
        }
      }

      if (has_pattern) {
        printf("On compte %lli\n", j);
        count += j;
      }
    }
  }
  return count;
}

int main(int argc, char **argv) {
  if (argc < 2) {
    fprintf(stderr, "Format : %s fichier_input\n", argv[0]);
    return 1;
  }

  ids *ids = malloc(sizeof(*ids));
  if (!parseur(argv[1], ids)) {
    fprintf(stderr, "Erreur dans le parsing");
    return 1;
  }

  printf("%lli\n", verifier_ids2(ids));

  return 0;
}