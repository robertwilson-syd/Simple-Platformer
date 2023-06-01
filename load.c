#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// typedef struct game_tile {
//     char* north;
//     char* east;
//     char* south;
//     char* west;
//     char c;
//     int is_ramp;
// } tile;


char static_board[20][61];
char dynamic_board[20][61];
int player[4];

// void init_game_tile (tile* x, char* "north", char* "east", char* "south", char* "west", char c, int is_ramp) {
//     x->north = "north";
//     x->east = "east";
//     x->south = "south";
//     x->west = "west";
//     x->c = c;
//     x->is_ramp = is_ramp;
// }    

// void init_all_tiles() {
//     tile low_ground;
//     init_game_tile(&low_ground, "lg", "lg", "lg", "lg", '-', 0);
//     tile high_ground;
//     init_game_tile(&high_ground, "hg", "hg", "hg", "hg", '+', 0);
//     tile wall;
//     init_game_tile(&wall, "wall", "wall", "wall", "wall", '*', 0);
//     tile sn_ramp;
//     init_game_tile(&sn_ramp, "hg", "sn_ramp", "lg", "sn_ramp", '#', 1);
//     tile ns_ramp;
//     init_game_tile(&ns_ramp, "lg", "ns_ramp", "hg", "ns_ramp", '=', 1);
//     tile ew_ramp;
//     init_game_tile(&ew_ramp, "ew_ramp", "lg", "ew_ramp", "hg", '\\', 1);
//     tile we_ramp;
//     init_game_tile(&we_ramp, "we_ramp", "hg", "we_ramp", "lg", '/', 1);
// }

void movement() {
    char move_direction;
// May need to fix use of scanf
    scanf("%c", &move_direction);
    switch(move_direction) {
        case 'w':
            printf("%c", move_direction);
            break;
        case 'd':
            dynamic_board[player[2]][(player[3])] = static_board[player[2]][(player[3])];
            player[3]++;
            dynamic_board[player[2]][(player[3])] = 'P';
            printf("%c", move_direction);
            break;
        case 's':
            printf("%c", move_direction);
            break;
        case 'a':
            printf("%c", move_direction);
            break;
    }
}


void game_load(char* user_file) {
    int x = 0;
    int y = 0;
    int exit[4];
    FILE *board_file;
    board_file = fopen(user_file, "r");

    char c;

    while(1) {
        c = fgetc(board_file);

        if (c == EOF){
            break;
        }

        if (c == '\n'){
            x = 0;
            y++;
            continue; 
        }

        switch(c){
            case 'P':
                player[0] = y;
                player[1] = x;
                break;
            case 'X':
                exit[0] = y;
                exit[1] = x;
                break;
        }


        //Old code in case needed

        // if (c == 'P'){
        //     player[0] = y;
        //     player[1] = x;
        // }

        // if (c == 'X'){
        //     exit[0] = y;
        //     exit[1] = x;
        // }       

        static_board[y][x] = c;
        dynamic_board[y][x] = c;
        x++;
    }
    fclose(board_file);
    player[2] = dynamic_board[player[0]][(player[1]+2)] - '0';
    player[3] = dynamic_board[player[0]][(player[1]+4)] - '0';

    exit[2] = dynamic_board[exit[0]][(exit[1]+2)] - '0';
    exit[3] = dynamic_board[exit[0]][(exit[1]+4)] - '0';

    dynamic_board[player[2]][(player[3])] = 'P';
    dynamic_board[exit[2]][(exit[3])] = 'X';
}
//Need to generalise this and not print bottom part
void print_game_board() {
    int i = 0;
    int j = 0;
    while (i < 10){
        while(j < 10){
            printf("%c", dynamic_board[i][j]);
            j++;
        }
        i++;
        j = 0;
        printf("\n");
    }
}

int main(int argc, char *argv[] )  {  

   if(argc < 2){  
      printf("Usage: ./platformer <level file>\n");  
      return 0;
   }  
    game_load(argv[1]);
    print_game_board();
    movement();
    print_game_board();
    return 0;
}
