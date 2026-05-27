/* generator.c
 * a password generator
 * written by kyle j
 */
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/random.h>


char *generate_buffer(size_t);
void free_buffer(char *);
unsigned char select_char(char *, size_t, int);

typedef enum {
  CLASS_LOWER = 1 << 0, /* 0001 */
  CLASS_UPPER = 1 << 1, /* 0010 */
  CLASS_DIGIT = 1 << 2, /* 0100 */
  CLASS_PUNCT = 1 << 3, /* 1000 */
} CharClass;

int main(int argc, char **argv) {

  const int BUF_SIZE = 64;

  size_t password_length;
  CharClass enabled_classes[4];
  int num_classes = 0;
  int enabled = 0;


  int opt;
  // we're going to use getopt to parse our command-line arguments
  // part of unistd.h
  // int getopt (int ARGC, char *const *ARGV, const char *OPTIONS)  
  while ((opt = getopt(argc, argv, "l:c:d:g:w:x:i")) != -1) {
	switch (opt) {
	case 'l':
	  // int atoi (const char *STRING)
	  // parses int from optarg (set by getopt) and converts to size_t
	  password_length = (size_t) atoi(optarg);
	  break;
	case 'c':
	  break;
	case 'd':
	  break;
	case 'g':
	  // we need to tokenize the optarg and use bitmasking to enable our character classes
	  // char * strtok (char *restrict NEWSTRING, const char *restrict DELIMITERS)
	  char *token = strtok(optarg,",");
	  while (token)
		{
		  if (strcmp(token, "lower") == 0) enabled |= CLASS_LOWER;
		  if (strcmp(token, "upper") == 0) enabled |= CLASS_UPPER;
		  if (strcmp(token, "digit") == 0) enabled |= CLASS_DIGIT;
		  if (strcmp(token, "punct") == 0) enabled |= CLASS_PUNCT;
		  // call again on the same optarg (advances to the next token bc of NULL)
		  token = strtok(NULL,",");
		}
	  break;
	case 'w':
	  break;
	case 'x':
	  break;
	case 'i':
	  break;
	}
  }
  if (enabled & CLASS_LOWER) enabled_classes[num_classes++] = CLASS_LOWER;
  if (enabled & CLASS_UPPER) enabled_classes[num_classes++] = CLASS_UPPER;
  if (enabled & CLASS_DIGIT) enabled_classes[num_classes++] = CLASS_DIGIT;
  if (enabled & CLASS_PUNCT) enabled_classes[num_classes++] = CLASS_PUNCT;

  char password[password_length];
 
  for (int i = 0; i < (int) password_length; i++) {
	CharClass current = enabled_classes[i % num_classes]; // cycles 0,1,2,0,1,2...
	char *buffer = generate_buffer(BUF_SIZE);
	password[i] = select_char(buffer, BUF_SIZE, current); // single-bit mask
	free_buffer(buffer);
  }

  // then shuffle using Fisher-Yates
  for (int i = password_length - 1; i > 0; i--) {
    // need a random index from 0..i
    unsigned char rand_byte;
    getrandom(&rand_byte, 1, 0);
    int j = rand_byte % (i + 1);
    char tmp = password[i];
    password[i] = password[j];
    password[j] = tmp;
  }
  for (int i = 0; i < password_length; i++) {
	printf("%c",password[i]);
  }

  printf("\n"); 
}

char *generate_buffer(size_t len) {
  char *buffer = (char *)malloc(len + 1);
  if (!buffer)
	return NULL;
  // ssize_t getrandom (void *BUFFER, size_t LENGTH, unsigned, int FLAGS)
  ssize_t length = getrandom(buffer, len, 0);
  if ((int)length == (int)len)
	return buffer;
  else
	return NULL;
}

void free_buffer(char *buffer) {
  if (buffer)
	free(buffer);
  else
	printf("buffer is NULL!\n");
}

unsigned char select_char(char *buffer, size_t len, int class) {
  if (!buffer)
	return '\0';
  // loop through the buffer to find a valid char
  for (int i = 0; i < (int) len; i++) {
	// cast the character being tested with a mask (just to be sure)
	unsigned char c = *(buffer + i) & 0x7f;
	if ((class & CLASS_LOWER) && islower(c)) return c;
    if ((class & CLASS_UPPER) && isupper(c)) return c;
    if ((class & CLASS_DIGIT) && isdigit(c)) return c;
    if ((class & CLASS_PUNCT) && ispunct(c)) return c;
  }
  return '\0';
}
