#include "mydefs.h"

bool solvedcheck(state* s, board child);
void printsolution(state* s, int moves, int f);
bool boardexist(state* s, board child);
board childassign(state* s, int i, int f, char hawkhold);
bool sametiles(state* s, board child, int pos);
int movecounter(state* s, int pos);
bool imposscheck(state* s, board child);
bool zeromove(state* s, bool verbose);


/*Style Notes:
FLEN: Needs lots of adjustment
BRIEF: use comments for unclear functions
*/

bool file2str(const char* fname, char* str){
   char hold[BRDSZ] = "", hold2[BRDSZ*BRDSZ] = "";
   if (fname == NULL || str == NULL){
      return false;
   }
   else{
      FILE* file = fopen(fname, "r");
      while (fgets(hold, BRDSZ, file) != NULL){
         strcat(hold2, hold);
      }
      fclose(file);
      //empties string of previous content
      for (unsigned long j = 0; j < sizeof(str); ++j){
         str[j] = '\0';
      }
      for (unsigned long i = 0; i < strlen(hold2)-LASTNEWLINE; ++i){
         if (hold2[i] == '\n'){
            str[i] = '-';
         }
         else {
            str[i] = hold2[i];
         }
      }
   }
   //removes inexplicable '@' that appears at end of string
   for (unsigned long k = 0; k < strlen(str); ++k){
      if (isalpha(str[k]) == false && str[k] != '-'){
         str[k] = '\0';
      }
   }
   return true;
}

state* str2state(const char* str){
   int row = 0, col = 0;
   state* s = malloc(sizeof(state));
   s->allbrds[FIRST].hawk = str[FIRST];
   //i = 1 to skip hawk at front of string
   for (unsigned long i = 1; i < strlen(str); ++i){
      if (str[i] == '-'){
         ++row;
         s->allbrds[FIRST].cols = col;
         col = 0;
      }
      else {
         //PREV used for PREVious row or column throughout
         s->allbrds[FIRST].tiles[row-PREV][col] = str[i];
         ++col;
      }
   }
   s->allbrds[FIRST].rows = row;
   s->allbrds[FIRST].parent = NOPARENT;
   return s;
}

int solve(state* s, bool verbose){
   int moves = 0, pos = 1;
   board child;
   //catches already solved boards early
   if (zeromove(s, verbose) == true){
      return moves;
   }
   for (int f = 0; f < MAXBRDS; ++f){
      //catches impossible boards early
      if (imposscheck(s, s->allbrds[f]) == true){
         return IMPOSSIBLE;
      }
      for (int i = 0; i < s->allbrds[FIRST].cols; ++i){
         child = childassign(s, i, f, s->allbrds[f].hawk);
         if (boardexist(s, child) == false){
            s->allbrds[pos] = child;
            if (solvedcheck(s, child) == true){
               moves = movecounter(s, pos);
               if (verbose == true){
                  printsolution(s, moves, pos);
               }
               return moves;
            }
            ++pos;
         }
      }
   }
   return IMPOSSIBLE;
}

bool solvedcheck(state* s, board child){
   int solvedcols = 0, same;
   
   for (int i = 0; i < s->allbrds[FIRST].cols; ++i){
      same = 0;
      for (int j = 1; j < s->allbrds[FIRST].rows; ++j){
         if (child.tiles[j][i] == child.tiles[FIRST][i]){
            ++same;
         }
      }
      if (same == s->allbrds[FIRST].rows-LASTROW){
         ++solvedcols;
      }
   }
   if (solvedcols == s->allbrds[FIRST].cols){
      return true;
   }
   return false;
}

void printsolution(state* s, int moves, int pos){
   int poshold = pos, printhold[MAXBRDS];
   for (int l = 0; l < moves; ++l){
      printhold[l] = poshold;
      poshold = s->allbrds[poshold].parent;
   }
   printhold[moves] = 0;
   //k > -1 to avoid missing out k = 0
   for (int k = moves; k > -1; --k){
      printf("\n");
      for (int j = 0; j < s->allbrds[FIRST].rows; ++j){
         printf("\n");
         for (int i = 0; i < s->allbrds[FIRST].cols; ++i){
            printf("%c", s->allbrds[printhold[k]].tiles[j][i]);
         }
      }
   }
}

bool boardexist(state* s, board child){
   for (int i= 0; i < MAXBRDS; ++i){
      if (sametiles(s, child, i) == true){
         return true;
      }
   }
   return false;
}

board childassign(state* s, int i, int f, char hawkhold){
   board child;
   child.hawk = s->allbrds[f].tiles[s->allbrds[FIRST].rows-PREV][i];
   child.parent = f;
   child.rows = s->allbrds[FIRST].rows;
   child.cols = s->allbrds[FIRST].cols;
   //Shifts tiles for move. j = 1 to avoid out of bounds
   for (int j = 1; j < s->allbrds[FIRST].rows; ++j){
      child.tiles[j][i] = s->allbrds[f].tiles[j-PREV][i];
   }
   //All tiles not in the shifted column kept same
   for (int y = 0; y < s->allbrds[FIRST].rows; ++y){
      for (int x = 0; x < s->allbrds[FIRST].cols; ++x){
         if (x != i){
            child.tiles[y][x] = s->allbrds[f].tiles[y][x];
         }
      }
   }
   child.tiles[FIRST][i] = hawkhold;
   return child;
}

bool sametiles(state* s, board child, int pos){
   for (int j = 0; j < s->allbrds[FIRST].rows; ++j){
      for(int i = 0; i < s->allbrds[FIRST].cols; ++i){
         if (s->allbrds[pos].tiles[j][i] != child.tiles[j][i]){
            return false;
         }
      }
   }
   return true;
}

int movecounter(state* s, int pos){
   int poshold = pos, moves = 0;
   while (poshold != 0){
      poshold = s->allbrds[poshold].parent;
      ++moves;
   }
   return moves;
}

//checks for garbage values produced by impossible boards
bool imposscheck(state* s, board child){
   if (isalpha(child.hawk) == false){
      return true;
   }
   for (int j = 0; j < s->allbrds[FIRST].rows; ++j){
      for (int i = 0; i < s->allbrds[FIRST].cols; ++i){
         if (isalpha(child.tiles[j][i]) == false){
            return true;
         }
      }
   }
   return false;
}

bool zeromove(state* s, bool verbose){
   if (solvedcheck(s, s->allbrds[FIRST]) == true){
      if (verbose == true){
         for (int y = 0; y < s->allbrds[FIRST].rows; ++y){
            printf("\n");
            for (int x = 0; x < s->allbrds[FIRST].cols; ++x){
               printf("%c", s->allbrds[FIRST].tiles[y][x]);
            }
         }
      }
      return true;
   }
   return false;
}

void test(void){

}
