#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

void strip(char *s) {
  int last_index = strlen(s) - 1;
  // set all spaces with 0 from end to start
  while (isblank(s[last_index]) || isspace(s[last_index]) || s[last_index] == '\n') {
    s[last_index--] = '\0';
  }

  // find first printable char
  int i = 0;
  while (isblank(s[i]) || isspace(s[i]) || s[i] == '\n') {
    i++;
  }

  // copy without left spaces
  char tmp[256];
  memset(tmp, 0, 256);

  int j = 0;
  for (; isprint(s[i]); i++, j++) {
    tmp[j] = s[i];
  }
  tmp[++j] = '\0';
  strcpy(s, tmp);
}

int is_empty_string(char *s) { return s[0] == '\0'; }

void clear_line(char *s) {
  // remove comment
  char *c = strchr(s, '/');
  if(c != NULL) {
    *c = '\0';
  }

  // remove spaces around
  strip(s);
}

char **explode(const char *source, const char *delimiter) {
  char *tmp = malloc(sizeof(char) * (strlen(source) + 1));
  strcpy(tmp, source);

  char **dest = NULL;
  char *token = strtok(tmp, delimiter);
  for(int i = 0; ; i++, token = strtok(NULL, delimiter)) {
    dest = realloc(dest, sizeof(char *) * (i + 1));

    // end of array
    if(token == NULL) {
      dest[i] = NULL;
      break;
    }

    dest[i] = malloc(sizeof(char) * (strlen(token) + 1));
    strcpy(dest[i], token);
  }

  free(tmp);
  return dest;
}

void free_exploded(char **arr) {
  for(int i = 0; arr[i] != NULL; i++) {
    free(arr[i]);
  }
  free(arr);
}

int arrlen(char **arr) {
  int size = 0;
  for(int i = 0; arr[i] != NULL; i++) {
    if(strlen(arr[i]) > 0) {
      size++;
    } else {
      break;
    }
  }

  return size;
}

int in_arr(const char *value, const char *arr[]) {
  for(int i = 0; arr[i] != NULL; i++) {
    if(strcmp(value, arr[i]) == 0) {
      return 1;
    }
  }
  return 0;
}

void re_match(const char *regex, const char *string, regmatch_t *pmatch) {
  regex_t reg;
  int err;

  err = regcomp(&reg, regex, REG_EXTENDED);
  if (err != 0) {
    char *errbuf;
    size_t errbuf_size = regerror(err, &reg, NULL, 0);

    errbuf = malloc(errbuf_size);
    regerror(err, &reg, errbuf, errbuf_size);

    fprintf(stderr, "%s\n", errbuf);
    free(errbuf);
    exit(1);
  }

  if (regexec(&reg, string, 1, pmatch, 0) != 0) {
    pmatch[0].rm_so = -1;
    pmatch[0].rm_eo = -1;
  }

  regfree(&reg);
}

void strtoupper(char *s) {
  for(int i = 0; s[i] != '\0'; i++) {
    s[i] = toupper(s[i]);
  }
}

char *get_file_name(char *file_path) {
  char *out = (char*)malloc(sizeof(char) * 128);
  char *c = strrchr(file_path, '/');
  if(c) {
    for(int i = 0; *c != '\0'; i++) {
      out[i] = *++c;
    }
  }

  c = strchr(out, '.');
  if(c) {
    *c = '\0';
  }

  return out;
}

FILE *open_source_file(const char *file_path) {
    FILE *file = fopen(file_path, "r");
    if(file == NULL) {
        perror("Error opening source file");
        exit(1);
    }

    return file;
}

FILE *open_output_file(char *file_path, FILE *source_file) {
    char out[128];
    char *c = strchr(file_path, '.');
    if (c) {
        *c = '\0';
    }
    snprintf(out, 128, "%s.asm", file_path);

    FILE *output_file = fopen(out, "w");
    if(output_file == NULL) {
        perror("Error opening output file");
        fclose(source_file);
        exit(1);
    }

    return output_file;
}

void write_output(const char *s, FILE *output_file) {
  #ifdef DEBUG
  printf("[utils.c] write_output: '%s'\n", s);
  #endif
  fputs(s, output_file);
}

