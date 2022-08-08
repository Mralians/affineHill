#include "affinehill.h"

int gcd(int a, int b) {
  if (b == 0) return a;
  return gcd(b, (a % b));
}

int extendedEuclidean(int a, int b) {
  int q;
  int swap;

  // init
  int r[2] = {a, b};
  int t[2] = {0, 1};  // t1 = 0 , t2 = 1
  int s[2] = {1, 0};  // s1 = 1 , s2 = 0
  // end

  while (r[1] != 0) {
    q = r[0] / r[1];
    swap = r[1];
    r[1] = r[0] % r[1];
    r[0] = swap;

    swap = s[1];
    s[1] = s[0] - (q * s[1]);
    s[0] = swap;

    swap = t[1];
    t[1] = t[0] - (q * t[1]);
    t[0] = swap;
  }

  // return (a * s[1]) + (b * t[1]);
  return t[0];
}
//////////////////////////////////////////////////////

int multiplicativeInverse(int m, int a) {
  if (gcd(m, a) != 1) return 1;
  int t = extendedEuclidean(m, a);
  if (t % m < 0) return m + t;
  return t % m;
}
int additiveInverse(int num, int mod) { return mod - num; }

long det(int key[ROW][COLUMN * 2]) {
  long determinant;
  determinant =
      key[0][0] * ((key[1][1] * key[2][2]) - (key[2][1] * key[1][2])) -
      key[0][1] * (key[1][0] * key[2][2] - key[2][0] * key[1][2]) +
      key[0][2] * (key[1][0] * key[2][1] - key[2][0] * key[1][1]);
  // determinant = key[0][0]*key[1][1] - key[1][0]*key[0][1];
  determinant = determinant % MOD;
  if (determinant < 0) determinant = MOD + determinant;

  return determinant;
}

int inverse(int a[ROW][COLUMN * 2], int mod) {
  int ratio, temp;
  int i, j, k, n = BLOCK_SIZE;
  for (i = 0; i < n; i++)
    for (j = 0; j < n; j++) {
      if (i == j)
        a[i][j + n] = 1;
      else
        a[i][j + n] = 0;
    }
  /*
  //Pivoting
  for (int i = n - 1; i > 0; i--)
          if (a[i - 1][0] < a[i][0])
                  for (j = 0; j < 2 * n; j++) {
                          temp = a[i][j];
                          a[i][j] = a[i - 1][j];
                          a[i - 1][j] = temp;
                  }*/
  for (i = 0; i < n; i++)
    for (j = 0; j < n; j++)
      if (i != j) {
        ratio = (a[j][i] * (multiplicativeInverse(mod, a[i][i]) % mod)) % mod;
        for (k = 0; k < 2 * n; k++) {
          a[j][k] = (a[j][k] - ((ratio * a[i][k]) % mod)) % mod;
          if (a[j][k] < 0) a[j][k] = mod + a[j][k];
        }
      }
  for (i = 0; i < n; i++)
    for (j = n; j < 2 * n; j++) {
      a[i][j] = (a[i][j] * (multiplicativeInverse(mod, a[i][i])) % mod) % mod;
      if (a[i][j] < 0) a[i][j] = mod + a[i][j];
    }
  for (i = 0; i < n; i++)
    for (j = n; j < 2 * n; j++) a[i][j - n] = a[i][j];
  return 0;
}

