#ifndef _AHILL_DOT_H
#define _AHILL_DOT_H

#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define ROW 3
#define COLUMN 3
#define BLOCK_SIZE 3
#define MOD 256

typedef enum { ENC, DEC } cryptMode_t;
extern char *program_invocation_short_name;
extern int gcd(int a, int b);
extern long det(int key[ROW][COLUMN * 2]);
extern int multiplicativeInverse(int m, int a);
extern int inverse(int a[ROW][COLUMN * 2], int mod);
extern int additiveInverse(int num, int mod);
#endif  //  _AHILL_DOT_H
