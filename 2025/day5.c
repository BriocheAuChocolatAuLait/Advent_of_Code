#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct liste_doublet {
  long long int c1;
  long long int c2;
  struct liste_doublet *suivant;
} l;

// Sépare la liste en deux moitiés
void split_list(l *source, l **front, l **back) {
  l *slow = source;
  l *fast = source->suivant;

  while (fast != NULL) {
    fast = fast->suivant;
    if (fast != NULL) {
      slow = slow->suivant;
      fast = fast->suivant;
    }
  }

  *front = source;
  *back = slow->suivant;
  slow->suivant = NULL;
}

// Fusionne deux listes triées
l *merge_sorted(l *a, l *b) {
  if (!a)
    return b;
  if (!b)
    return a;

  l *result = NULL;

  if (a->c1 <= b->c1) {
    result = a;
    result->suivant = merge_sorted(a->suivant, b);
  } else {
    result = b;
    result->suivant = merge_sorted(a, b->suivant);
  }

  return result;
}

// Tri principal
void merge_sort_list(l **headRef) {
  if (*headRef == NULL || (*headRef)->suivant == NULL)
    return;

  l *head = *headRef;
  l *a;
  l *b;

  split_list(head, &a, &b);

  merge_sort_list(&a);
  merge_sort_list(&b);

  *headRef = merge_sorted(a, b);
}

void destroy_list(l *head) {
  l *tmp;
  while (head != NULL) {
    tmp = head;
    head = head->suivant;
    free(tmp);
  }
}

char *read_line(FILE *f) {
  size_t size = 128;
  size_t len = 0;
  char *buffer = malloc(sizeof(char) * size);

  if (!buffer) {
    return NULL;
  }

  int c;
  while ((c = fgetc(f)) != EOF && c != '\n') {
    if (len + 1 > size) {
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
    return NULL;
  }

  buffer[len] = '\0';
  return buffer;
}

long long int count_fresh(FILE *f) {
  long long int count = 0;
  size_t size = 200;
  size_t len = 0;
  long long int *bornes = malloc(sizeof(long long int) * size);

  char *line;
  while ((line = read_line(f)) != NULL) {

    if (line[0] == '\n' || line[0] == '\0') {
      free(line);
      break;
    }
    if (len + 2 > size) {
      size_t new_size = size * 2;
      long long int *new_bornes =
          realloc(bornes, new_size * sizeof(long long int));
      if (!new_bornes) {
        free(bornes);
        return 0;
      }
      size = new_size;
      bornes = new_bornes;
    }

    char *tiret = strchr(line, '-');
    *tiret = '\0';

    long long int a = atoll(line);
    long long int b = atoll(tiret + 1);
    bornes[len++] = a;
    bornes[len++] = b;

    free(line);
  }

  while ((line = read_line(f)) != NULL) {
    long long int id = atoll(line);
    for (size_t j = 0; j + 1 < len; j += 2) {
      if (id >= bornes[j] && id <= bornes[j + 1]) {
        count++;
        break;
      }
    }
  }

  return count;
}

long long int count_ids_fresh(FILE *f) {
  long long int count = 0;

  l *liste = NULL;

  char *line;
  while ((line = read_line(f)) != NULL) {

    if (line[0] == '\n' || line[0] == '\0') {
      free(line);
      break;
    }

    l *new_chainon = malloc(sizeof(l));

    char *tiret = strchr(line, '-');
    *tiret = '\0';

    new_chainon->c1 = atoll(line);
    new_chainon->c2 = atoll(tiret + 1);
    new_chainon->suivant = liste;

    liste = new_chainon;

    free(line);
  }

  merge_sort_list(&liste);
  l *chainon_precedent = liste;
  l *chainon_suivant = liste->suivant;

  while (chainon_suivant != NULL) {
    if (chainon_precedent->c2 >= chainon_suivant->c1) {
      chainon_precedent->c2 = (chainon_precedent->c2 > chainon_suivant->c2)
                                  ? chainon_precedent->c2
                                  : chainon_suivant->c2;
      chainon_precedent->suivant = chainon_suivant->suivant;
      free(chainon_suivant);
      chainon_suivant = chainon_precedent->suivant;
    } else {
      chainon_precedent = chainon_suivant;
      chainon_suivant = chainon_suivant->suivant;
    }
  }

  l *chainon = liste;
  while (chainon != NULL) {
    count += chainon->c2 - chainon->c1 + 1;
    chainon = chainon->suivant;
  }

  destroy_list(liste);
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

  printf("Et le résultaaaat : %lli\n", count_ids_fresh(f));

  return 0;
}