/* generator.c
 * a password generator
 * written by kyle j
 */

#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/random.h>

char *generate_buffer(size_t);
void free_buffer(char *);
unsigned char select_char(char *, size_t, int);

int main(void) {

  const int SIZE = 32;

  size_t password_length;
  int character_classes;

  printf("character count: ");
  scanf(" %lu", &password_length);

  printf("character class: ");
  scanf(" %d", &character_classes);

  for (int i = 0; i < password_length; i++) {
	char *buffer = generate_buffer(SIZE);
	char c = select_char(buffer,SIZE,character_classes);
	printf("%c", c);
	free_buffer(buffer);
  }
  printf("\n"); 
}

char *generate_buffer(size_t len) {
  char *buffer = (char *)malloc(len + 1);
  if (!buffer)
	return NULL;
  // ssize_t getrandom (void *BUFFER, size_t LENGTH, unsigned, int FLAGS)
  int length = getrandom(buffer, len, 0);
  if (length)
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
	return ' ';
  // loop through the buffer to find a valid char
  for (int i = 0; i < (int) len; i++) {
	// cast the character being tested with a mask (just to be sure)
	unsigned char c = *(buffer + i) & 0x7f;
	switch (class) {
	case 0:
	  if (islower(c))
		return (unsigned char) (*(buffer + i) & 0x7f);
	  break;
	case 1:
	  if (isupper(c))
		return (unsigned char) (*(buffer + i) & 0x7f);
	  break;
	case 2:
	  if (isdigit(c))
		return (unsigned char) (*(buffer + i) & 0x7f);
	  break;
	case 3:
	  if (ispunct(c))
		return (unsigned char) (*(buffer + i) & 0x7f);
	  break;
	}
  }
}
