#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game_functions.h"

//Most of the functionality is stored in "game_functions.h", this tests there is a level inputted and then runs the program until the user quits
int main(int argc, char *argv[] )  {  

   if(argc < 2){  
      printf("Usage: ./platformer <level file>\n");  
      return 0;
   }

    game_load(argv[1]);
    print_board();
    while(1){
         user_command();
         printf("\n");
         print_board();
    }

    return 0;
}

