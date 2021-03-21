#include "util.h"

/* MEMORY */
void *copy_value(int value_type, void *value) {
  void *copy;

  /* setup */
  copy = NULL;

  /* copy value */
  switch (value_type) {
    case INTEGER:
      copy = malloc(sizeof(int));
      *(int *) copy = *(int *) value;
      break;
    case FLOAT:
      copy = malloc(sizeof(float));
      *(float *) copy = *(float *) value;
      break;
    case DOUBLE:
      copy = malloc(sizeof(double));
      *(double *) copy = *(double *) value;
      break;
    case STRING:
      copy = malloc(sizeof(char) * strlen(value) + 1);
      strcpy(copy, value);
      break;
  }

  return copy;
}

int *malloc_int(int i) {
  int *int_ptr;
  int_ptr = malloc(sizeof(int));
  *(int *) int_ptr = i;
  return int_ptr;
}

float *malloc_float(float f) {
  float *float_ptr;
  float_ptr = malloc(sizeof(float));
  *(float *) float_ptr = f;
  return float_ptr;
}

double *malloc_double(double d) {
  double *double_ptr;
  double_ptr = malloc(sizeof(double));
  *(double *) double_ptr = d;
  return double_ptr;
}

char *malloc_string(const char *s) {
  char *str_ptr;

  if (s != NULL) {
    str_ptr = malloc(sizeof(char) * strlen(s) + 1);
    strcpy(str_ptr, s);
    return str_ptr;
  } else {
    return NULL;
  }
}

/* STRING */
int trim_char(const char c) {
  int i;
  const char trim[4] = {'\n', '\t', '\r', ' '};

  for (i = 0; i < 4; i++) {
    if (c == trim[i]) {
      return 1;
    }
  }

  return 0;
}

char *trim(const char *s) {
  int i = 0;
  int left = 0;
  int right = 0;
  char *trimmed = NULL;

  /* pre-check */
  silent_check(s != NULL);

  /* trim left */
  while (i < (int) strlen(s) && trim_char(s[i]))
    i++;
  left = i;

  /* trim right */
  i = (int) strlen(s) - 1;
  while (i > 0 && trim_char(s[i]))
    i--;
  right = i;

  /* return trimmed string */
  trimmed = malloc(sizeof(char) * (unsigned long) (right - left + 2));
  strncpy(trimmed, s + left, (unsigned long) (right - left + 1));
  trimmed[(right - left) + 1] = '\0';

  return trimmed;
error:
  return NULL;
}

char **split_kv(const char *s, const char token) {
  char *c;
  size_t index;
  size_t length;
  char **kv;
  char buf[9046];

  /* pre-check */
  kv = NULL;
  silent_check(s != NULL);

  /* setup */
  length = strlen(s);
  kv = malloc(sizeof(char *) * (size_t) 2);

  /* find index of first token */
  c = strchr(s, token);
  silent_check(c != NULL);
  index = (size_t)(c - s);

  /* copy key */
  bzero(buf, 9046);
  snprintf(buf, index + 1, "%s", s);
  kv[0] = trim(buf);

  /* copy value */
  bzero(buf, 9046);
  snprintf(buf, length - index, "%s", s + index);
  kv[1] = trim(buf);

  return kv;
error:
  free_mem(kv, free);
  return NULL;
}

char **split(const char *s, const char *token, int *nb_elements) {
  char *s_trimmed;
  char *el;
  char **elements;
  char buf[9046];
  size_t index;
  size_t num_of_elements;

  /* pre-check */
  elements = NULL;
  silent_check(s != NULL);
  s_trimmed = trim(s);

  /* calculate number of elements */
  bzero(buf, 9046);
  strcpy(buf, s_trimmed);

  el = strtok(buf, token);
  num_of_elements = 0;
  while (el != NULL) {
    num_of_elements++;
    el = strtok(NULL, token);
  }

  /* split string */
  silent_check(num_of_elements != 0);

  elements = malloc(sizeof(char *) * num_of_elements);
  bzero(buf, 9046);
  strcpy(buf, s_trimmed);

  index = 0;
  el = strtok(buf, token);
  while (el != NULL) {
    elements[index] = trim(el);
    index++;
    el = strtok(NULL, token);
  }

  *nb_elements = num_of_elements;
  free(s_trimmed);
  return elements;
error:
  free(s_trimmed);
  free_mem(elements, free);
  return NULL;
}

/* char *strex(const char *s, const char *open, const char *close) */
/* { */
/*     char *p1; */
/*     char *p2; */
/*     size_t len; */
/*     char *res; */
/*  */
/*     p1 = strstr(s, open); */
/*     p2 = strstr(s, close); */
/*     len = (size_t) p2 - (size_t) p1; */
/*     res = malloc(sizeof(char) * (len + 1)); */
/*     strncpy(res, p1, len); */
/* } */

/* RANDOM */
int randi(int min, int max) {
  int divisor;
  int retval;

  /* pre-check */
  if (min == max || min > max) {
    return min;
  }

  /* setup */
  divisor = RAND_MAX / (max + 1);
  retval = rand() / divisor;

  while (retval < min || retval > max) {
    retval = rand() / divisor;
  }

  return retval;
}

float randf(float min, float max) {
  float divisor;
  float retval;

  /* pre-check */
  if (min > max) {
    return min;
  }

  /* setup */
  divisor = (float) RAND_MAX / (float) (max + 1.0);
  retval = rand() / divisor;

  while (retval < min || retval > max) {
    retval = rand() / divisor;
  }

  return retval;
}

char *randstr(size_t length) {
  size_t i;
  char *r_str;

  r_str = malloc(sizeof(char *) * length);
  check_mem(r_str);

  /* generate random ASCII char */
  memset(r_str, '\0', sizeof(char *) * length);
  for (i = 0; i < length; i++) {
    r_str[i] = (char) randi(32, 122);
  }

  r_str[length] = '\0'; /* add null terminator for safety */
  return r_str;
error:
  free_mem(r_str, free);
  return NULL;
}

void *sample(void **array, int array_length) {
  return array[randi(0, array_length)];
}

/* COMPARATOR */
int intcmp(int v1, int v2) {
  if (v1 > v2) {
    return 1;
  } else if (v1 < v2) {
    return -1;
  } else {
    return 0;
  }

  return -2;
}

int intcmp_asc(int v1, int v2) { return intcmp(v1, v2); }

int intcmp_desc(int v1, int v2) { return intcmp(v1, v2) * -1; }

int fltcmp(float v1, float v2) {
  /* compare floats */
  if (fabs(v1 - v2) <= FLOAT_EPSILON) {
    return 0;
  } else if (v1 > v2) {
    return 1;
  } else if (v1 < v2) {
    return -1;
  }

  return -2;
}

int fltcmp_asc(float v1, float v2) { return fltcmp(v1, v2); }

int fltcmp_desc(float v1, float v2) { return fltcmp(v1, v2) * -1; }

int dblcmp(double v1, double v2) {
  /* compare doubles */
  if (fabs(v1 - v2) <= FLOAT_EPSILON) {
    return 0;
  } else if (v1 > v2) {
    return 1;
  } else if (v1 < v2) {
    return -1;
  }

  return -2;
}

int dblcmp_asc(double v1, double v2) { return dblcmp(v1, v2); }

int dblcmp_desc(double v1, double v2) { return dblcmp(v1, v2) * -1; }

int cmp_values(int value_type, void *v1, void *v2) {
  switch (value_type) {
    case INTEGER: return intcmp(*(int *) v1, *(int *) v2);
    case FLOAT: return fltcmp(*(float *) v1, *(float *) v2);
    case DOUBLE: return dblcmp(*(double *) v1, *(double *) v2);
    case STRING: return strcmp(v1, v2);
    default: return -2;
  }
}

/* FILE UTILS */
off_t fsize(const char *file_path) {
  struct stat st;

  if (stat(file_path, &st) == 0) {
    return st.st_size;
  }

  return -1;
}

char *fstring(const char *file_path) {
  FILE *fp;
  char *str;
  off_t file_size;

  /* pre-check */
  check(file_path, "file path is NULL!");
  check(fexists(file_path), "file path %s does not exists!", file_path);

  /* setup */
  fp = fopen(file_path, "r");
  file_size = fsize(file_path);

  /* copy file contents into variable */
  str = calloc((size_t) file_size + 1, 1);
  fread(str, (size_t) file_size, 1, fp);

  /* clean up */
  fclose(fp);

  return str;
error:
  return NULL;
}

char **flines(const char *file_path) {
  FILE *fp;
  int line_index;
  char buf[4096];
  char *line;
  char **lines;

  /* pre-check */
  check(access(file_path, F_OK) != -1, "file %s does not exist!", file_path);

  /* setup */
  line_index = 0;
  fp = fopen(file_path, "r");

  /* calculate number of lines */
  while (fgets(buf, sizeof(buf), fp)) {
    line_index++;
  }

  /* record lines length */
  lines = malloc(sizeof(char *) * ((size_t) line_index + 1));
  lines[0] = malloc(sizeof(char) * 4096);
  sprintf(lines[0], "%d", line_index);

  /* create lines array */
  rewind(fp);
  line_index = 1;
  while (fgets(buf, sizeof(buf), fp)) {
    line = trim(buf);
    lines[line_index] = line;
    line_index++;
  }

  /* clean up */
  fclose(fp);

  return lines;
error:
  return NULL;
}

int fexists(const char *file_path) {
  if (access(file_path, F_OK) != -1) {
    return 1;
  } else {
    return 0;
  }
}

char *fext(const char *file_path) {
  char *ch;
  char ext[50];
  char *copy;

  /* setup */
  copy = malloc_string(file_path);

  /* get file extension */
  ch = strtok(copy, ".");
  while (ch != NULL) {
    bzero(ext, 50);
    strcpy(ext, ch);
    ch = strtok(NULL, ".");
  }

  /* clean up */
  free(copy);

  return malloc_string(ext);
}

/* PATH UTILS */
char *path_join(int len, ...) {
  int i;
  int add_slash;
  int rm_slash;
  va_list args;
  char tmp[256];
  char buf[1024];
  size_t pointer;
  size_t buflen;

  /* setup */
  pointer = 0;
  bzero(buf, 1024);
  va_start(args, len);

  /* join paths */
  for (i = 0; i < len; i++) {
    bzero(tmp, 256);
    strcpy(tmp, va_arg(args, char *));

    /* check if need to add slash */
    add_slash = 1;
    rm_slash = 0;
    buflen = strlen(buf);
    if (buf[strlen(buf) - 1] == '/' && tmp[0] == '/') {
      rm_slash = 1;
    } else if (buflen == 0 || buf[buflen - 1] == '/' || tmp[0] == '/') {
      add_slash = 0;
    }

    /* build string */
    if (rm_slash) {
      sprintf(buf + pointer, "%s", tmp + 1);
    } else if (add_slash) {
      sprintf(buf + pointer, "/%s", tmp);
    } else {
      sprintf(buf + pointer, "%s", tmp);
    }

    pointer += strlen(tmp);
  }

  return malloc_string(buf);
}

/* I/O */
struct terminal_settings {
  struct termios old;
  struct termios new;
};

static struct terminal_settings *init_term_io_settings(int echo) {
  struct terminal_settings *ts;

  /* grab old i/o settings and make a copy */
  ts = malloc(sizeof(struct terminal_settings));
  tcgetattr(0, &ts->old);
  ts->new = ts->old;

  /* disable buffered i/o */
  ts->new.c_lflag &= (unsigned int) ~ICANON;

  /* set echo mode */
  ts->new.c_lflag &= echo ? ECHO : (unsigned int) ~ECHO;

  /* use new terminal i/o */
  tcsetattr(0, TCSANOW, &ts->new);

  return ts;
}

static void reset_term_io_settings(struct terminal_settings *ts) {
  tcsetattr(0, TCSANOW, &ts->old);
}

static char getch_(int echo) {
  char ch;
  struct terminal_settings *ts;

  ts = init_term_io_settings(echo);
  ch = (char) getchar();
  reset_term_io_settings(ts);
  free(ts);

  return ch;
}

char getch(void) {
  /* read 1 character without echo */
  return getch_(0);
}

char getche(void) {
  /* read 1 character with echo */
  return getch_(1);
}

/* TIMING */
void tic(struct timespec *tic) { clock_gettime(CLOCK_MONOTONIC, tic); }

float toc(struct timespec *tic) {
  struct timespec toc;
  float time_elasped;

  clock_gettime(CLOCK_MONOTONIC, &toc);
  time_elasped = (toc.tv_sec - tic->tv_sec);
  time_elasped += (toc.tv_nsec - tic->tv_nsec) / 1000000000.0;

  return time_elasped;
}
