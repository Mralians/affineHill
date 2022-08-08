#include <stdarg.h>

#include "affinehill.h"

#define PRINTERR(str)   \
  {                     \
    perror(#str);       \
    exit(EXIT_FAILURE); \
  }

#define ERRMSG(exitstatus, format, ...) \
  { fprintf(stderr, "[%s:%s] " format " ", __TIME__, __FILE__, ##__VA_ARGS__); }

/*Allow "cc -D" to override definition*/
#ifndef MAX_LINE
#define MAX_LINE 256
#endif

cryptMode_t mode;

// b vector
int b[BLOCK_SIZE] = {1, 15, 125};

// return size of file
static size_t fsize(FILE *stream) {
  size_t size;
  fseek(stream, 0l, SEEK_END);
  size = ftell(stream);
  rewind(stream);
  return size;
}

// padding with NUL character
static int padding(FILE *stream, int bufsize) {
  int i;
  size_t numWrite;
  char *padchr = "\0\0\0";
  unsigned short int pad;
  size_t size = fsize(stream);       // size of file
  pad = bufsize - (size % bufsize);  // size of pad
  if (pad == bufsize) return 0;      // no padding required
  numWrite = fwrite(padchr, 1, pad, stream);
  if (!numWrite) {
    perror("write");
    return 1;
  }
  return 0;
}
// crypt prototype
static void crypt(unsigned char *plainbuf, unsigned char *cipherbuf,
                  int key[ROW][COLUMN * 2], int b[BLOCK_SIZE], int mode);

int main(int argc, char *argv[]) {
  // Checking Command Line arguments for error
  if (argc != 5) {
    fprintf(stderr, "%s: missing file operand\n",
            program_invocation_short_name);
    fprintf(stdout,
            "Usage: %s [OPTION]... keyfile.txt SOURCE DECT\n\
                -e\t\tfor encryption\n\
                -d\t\tfor decryption\n",
            program_invocation_short_name);
    exit(EXIT_FAILURE);
  }

  mode = ((strcmp(argv[1], "-d")) == 0) ? DEC : ENC;

  clock_t start, finish;
  double time_taken;
  int i, j, key[ROW][COLUMN * 2], block;

  unsigned char cipherbuf[BLOCK_SIZE], plainbuf[BLOCK_SIZE];
  char cipherName[MAX_LINE];
  size_t size, numRead, numWrite;

  if (mode == ENC) sprintf(cipherName, "%s.enc", argv[4]);

  FILE *keyfile = fopen(argv[2], "rb");
  if (!keyfile) {
    fprintf(stderr, "%s:%s\n", argv[2], strerror(errno));
    exit(EXIT_FAILURE);
  }

  FILE *input_file = fopen(argv[3], "a+b");
  if (!input_file) {
    fprintf(stderr, "%s:%s\n", argv[3], strerror(errno));
    exit(EXIT_FAILURE);
  }

  FILE *output_file = fopen((mode == ENC) ? cipherName : argv[4], "wb");
  if (!output_file) {
    fprintf(stderr, "%s:%s\n", argv[4], strerror(errno));
    exit(EXIT_FAILURE);
  }

  for (i = 0; i < ROW; i++)
    for (j = 0; j < COLUMN; j++) fscanf(keyfile, "%d", &key[i][j]);

  if (fclose(keyfile)) PRINTERR(close);

  padding(input_file, BLOCK_SIZE);
  size = fsize(input_file);   // size of input file
  block = size / BLOCK_SIZE;  // Number of blocks

  if (mode == DEC) {
    inverse(key, MOD);
    for (i = 0; i < BLOCK_SIZE; i++) b[i] = additiveInverse(b[i], MOD);
  }
  errno = 0;
  start = clock();
  // iteration crypt function for each block
  for (i = 0; i < block; i++) {
    numRead = fread(plainbuf, 1, BLOCK_SIZE, input_file);
    if (!numRead || errno != 0) PRINTERR(read);
    crypt(plainbuf, cipherbuf, key, b, mode);
    numWrite = fwrite(cipherbuf, 1, numRead, output_file);
    if (!numWrite || errno != 0) PRINTERR(write);
    if (numWrite != numRead)
      ERRMSG(EXIT_FAILURE, "couldn't write whole buffer\n");
  }
  finish = clock();
  time_taken = (double)(finish - start) / (double)CLOCKS_PER_SEC;

  if (fclose(input_file)) PRINTERR(close);
  if (fclose(output_file)) PRINTERR(close);
  printf("Finished processing. Time taken: %lf seconds.\n", time_taken);
  exit(EXIT_SUCCESS);
}

static void crypt(unsigned char *plainbuf, unsigned char *cipherbuf,
                  int key[ROW][COLUMN * 2], int b[BLOCK_SIZE], int mode) {
  /*
  additiveInverse(b[i]) + b[i] = 0 mod n
  for DEC(decryption) => pi = (ci + b[i]) * k^-1
  for enc(encryption) => ci = (pi * k) + b[i]
  */

  int i, j, sum = 0;

  if (mode == DEC)
    for (i = 0; i < BLOCK_SIZE; i++) plainbuf[i] += b[i];

  for (i = 0; i < ROW; i++) {
    for (j = 0; j < COLUMN; j++) sum = sum + (plainbuf[j] * key[j][i]);
    cipherbuf[i] = sum % 256;
    if (mode == ENC) cipherbuf[i] += b[i];
    sum = 0;
  }
}
