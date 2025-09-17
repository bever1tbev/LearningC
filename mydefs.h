#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <assert.h>
#include <stdbool.h>

#define BRDSZ 6
#define MAXBRDS 200000
#define IMPOSSIBLE -1
#define LASTNEWLINE 1
#define PREV 1
#define FIRST 0
#define LASTROW 1
#define NOPARENT -1
#define MAXSTR (BRDSZ*BRDSZ+BRDSZ+2)

typedef struct board{
   char tiles[BRDSZ][BRDSZ];
   char hawk;
   int parent;
   int cols;
   int rows;
}board;

typedef struct state{
   board allbrds[MAXBRDS];
}state;

#include "md.h"
