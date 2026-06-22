#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct doublon {
  char operateur;
  int nb_caracteres;
} doublet;

char *read_line(FILE *f) {
  size_t size = 128;
  char *buffer = malloc(sizeof(char) * size);
  if (!buffer)
    return NULL;

  size_t len = 0;
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

doublet **read_last_line(FILE *f, int *nb_colonnes) {
  char *line;
  while ((line = read_line(f)) != NULL) {
    if (line[0] == '*' || line[0] == '+') {
      break;
    }
    free(line);
  }

  if (!line)
    return NULL;

  int i = 0;
  doublet **tab = NULL;
  int colonnes = 0;

  while (line[i] != '\0') {
    colonnes++;
    tab = realloc(tab, sizeof(doublet *) * colonnes);
    doublet *new_doublet = malloc(sizeof(doublet));
    new_doublet->operateur = line[i];
    new_doublet->nb_caracteres = 0;
    i++;

    while (line[i] == ' ') {
      new_doublet->nb_caracteres++;
      i++;
    }
    tab[colonnes - 1] = new_doublet;
  }

  tab[colonnes - 1]->nb_caracteres++;
  *nb_colonnes = colonnes;
  free(line);
  return tab;
}


long long int solve_maths2(FILE *f, doublet **tab, int nb_colonnes) {
    long long int *results = malloc(sizeof(long long int) * nb_colonnes);
    if (!results) return 0;
    
    for (int col = 0; col < nb_colonnes; col++) {
        if (tab[col]->operateur == '+') {
            results[col] = 0;
        } else {
            results[col] = 1;
        }
    }
    
    char *lines[3];
    for (int i = 0; i < 3; i++) {
        lines[i] = read_line(f);
        if (!lines[i]) {
            for (int j = 0; j < i; j++) free(lines[j]);
            free(results);
            return 0;
        }
    }
    
    // IMPORTANT: Dans chaque bloc, lire les colonnes de DROITE à GAUCHE
    int pos = 0;
    
    for (int col = 0; col < nb_colonnes; col++) {
        int largeur = tab[col]->nb_caracteres;
        
        // Lire les colonnes de ce bloc de DROITE à GAUCHE
        for (int char_pos = largeur - 1; char_pos >= 0; char_pos--) {
            char number_str[5] = {0};
            int num_idx = 0;
            
            for (int ligne = 0; ligne < 3; ligne++) {
                char c = lines[ligne][pos + char_pos];
                if (c != '\0' && c != ' ') {
                    number_str[num_idx++] = c;
                }
            }
            number_str[num_idx] = '\0';
            
            if (num_idx > 0) {
                long long int valeur = atoll(number_str);
                
                if (tab[col]->operateur == '+') {
                    results[col] += valeur;
                } else if (tab[col]->operateur == '*') {
                    results[col] *= valeur;
                }
            }
        }
        
        pos += largeur;
    }
    
    for (int i = 0; i < 3; i++) {
        free(lines[i]);
    }
    
    long long int count = 0;
    printf("\nRésultats détaillés:\n");
    for (int i = 0; i < nb_colonnes; i++) {
        printf("Colonne %d (op='%c'): %lld\n", i, tab[i]->operateur, results[i]);
        count += results[i];
    }
    
    free(results);
    return count;
}

void free_doublets(doublet **tab, int nb_colonnes) {
    for (int i = 0; i < nb_colonnes; i++) {
        free(tab[i]);
    }
    free(tab);
}

long long int solve_maths(FILE *f) {
  long long int *numbers_mat[3];
  int colonne = 0;
  int nb_total_colonnes;

  char *line;

  for (int i = 0; i < 3; i++) {
    numbers_mat[i] = NULL;
    line = read_line(f);
    char *token = strtok(line, " \t");

    colonne = 0;

    while (token) {

      size_t n = strlen(token);
      while (n > 0 && (token[n - 1] == '\n' || token[n - 1] == '\r'))
        token[--n] = '\0';

      long long int a = atoll(token);
      long long int *temp =
          realloc(numbers_mat[i], (colonne + 1) * sizeof(long long int));
      numbers_mat[i] = temp;
      numbers_mat[i][colonne] = a;
      colonne++;

      token = (strtok(NULL, " \t"));
    }
    if (i == 0)
      nb_total_colonnes = colonne;
    free(line);
  }
  line = read_line(f);
  long long int count = 0;
  char *ops = line;
  int op_index = 0;
  char *op_tok = strtok(ops, " \t");

  while (op_tok && op_index < nb_total_colonnes) {

    char op = op_tok[0];
    int is_mul = (op == '*');

    long long inside_count = is_mul ? 1 : 0;

    for (int j = 0; j < 3; j++) {
      inside_count = is_mul ? inside_count * numbers_mat[j][op_index]
                            : inside_count + numbers_mat[j][op_index];
    }

    count += inside_count;

    op_tok = strtok(NULL, " \t");
    op_index++;
  }

  for (int i = 0; i < 3; i++) {
    free(numbers_mat[i]);
  }
  free(line);
  return count;
}

int main(int argc, char **argv) {
  if (argc < 2) {
    fprintf(stderr, "Usage : %s <input>\n", argv[0]);
    return 1;
  }

  FILE *f = fopen(argv[1], "r");
  if (!f) {
    fprintf(stderr, "Erreur de lecture du fichier");
    return 1;
  }

  int nb_colonnes = 0;

  doublet** tab = read_last_line(f, &nb_colonnes);
  f = fopen(argv[1], "r");

  printf("Et le résultaaaaat : %lli\n", solve_maths2(f, tab, nb_colonnes));

    free_doublets(tab, nb_colonnes);

  return 0;
}