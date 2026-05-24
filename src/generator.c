/* generator.c
 * a password generator
 * written by kyle j
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/random.h>



int main(void) {

  const int SIZE = 32;
  char buffer[SIZE];
  // ssize_t getrandom (void *BUFFER, size_t LENGTH, unsigned, int FLAGS)
  size_t length = getrandom(buffer, SIZE, 0);

  printf("bytes: %u\n", length);

  for(int i = 0; i < length; i++) {
	// print the bytes in the buffer, and mask it to the lower 8 bits to
	// counteract the sign extension from the int promotion of signed char
	printf("byte %d: %x\n", i, buffer[i] & 0xff);
  }

}
