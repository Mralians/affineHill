#include "affinehill.h"

typedef enum { FALSE, TRUE } bool_t;

int keyGen(bool_t detail) {
  int i, j;
  int key[ROW][COLUMN * 2];
  srand(time(0));
  long determinant;

  FILE *keyfile = fopen("keyfile.txt", "wb");
  if (!keyfile) {
    perror("open: ");
    exit(EXIT_FAILURE);
  }

  // process time
  clock_t start, finish;
  double time_taken;
  start = clock();

  while (TRUE) {
    for (i = 0; i < ROW;) {
      for (j = 0; j < COLUMN; j++) key[i][j] = rand() % MOD;
      // check for invertible
      if (gcd(key[i][i], MOD) != 1) continue;
      i++;
    }
    determinant = det(key);
    if (gcd(determinant, MOD) == 1)  // gcd(det,mod) = 1
      break;
  }
  // show details(-d)
  if (detail) {
    for (i = 0; i < ROW; i++) {
      printf("\n");
      for (j = 0; j < COLUMN; j++) printf("%d\t", key[i][j]);
    }
    printf("\nDeterminant of 3X3 matrix: %ld\n", determinant);

    printf("GCD(%ld,%d) = %d\n", determinant, MOD, gcd(determinant, MOD));
  }

  for (i = 0; i < ROW; i++)
    for (j = 0; j < COLUMN; j++) fprintf(keyfile, "%d ", key[i][j]);

  fclose(keyfile);

  finish = clock();
  time_taken = (double)(finish - start) / (double)CLOCKS_PER_SEC;
  printf("\nFinished processing. Time taken: %lf seconds.\n", time_taken);
  puts("The key was written in the keyfile.txt");

  return 0;
}

int main(int argc, char *argv[]) {
  bool_t showDet = FALSE;
  if (argc > 1 && (strcmp(argv[1], "-d")) == 0) showDet = TRUE;
  keyGen(showDet);
  exit(EXIT_SUCCESS);
}
