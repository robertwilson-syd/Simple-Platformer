#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game_functions.h"

//Defining a tile struct to be used for movement rules
typedef struct board_tile {
    int north;
    int east;
    int south;
    int west;
    char character;
    int is_ramp;
} game_tile;


//Declaring global variables (The level and player position and the special tile information)
game_tile game_level[21][63];
char dynamic_board[21][61];
char special_characters[11][17];
int player[3];
char valid_special_characters[11] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'P', 'X'};

void init_game_tile(game_tile* x, int north, int east, int south, int west, char c, int is_ramp) {
    x->north = north;
    x->east = east;
    x->south = south;
    x->west = west;
    x->character = c;
    x->is_ramp = is_ramp;
}    
//low ground = 0, high ground = 1, ew_ramp = 2, we_ramp = 3, sn_ramp = 4, ns_ramp = 5, wall = 6
void init_all_tiles(game_tile* low_ground, game_tile* high_ground, game_tile* wall, game_tile* sn_ramp, game_tile* ns_ramp, game_tile* ew_ramp, game_tile* we_ramp, game_tile* platform_a, game_tile* platform_b, game_tile* platform_c, game_tile* platform_d, game_tile* platform_e, game_tile* platform_f, game_tile* platform_g, game_tile* platform_h) {
    init_game_tile(low_ground, 0, 0, 0, 0, '-', 0);
    init_game_tile(high_ground, 1, 1, 1, 1, '+', 0);
    init_game_tile(wall, 6, 6, 6, 6, '*', 0);
    init_game_tile(sn_ramp, 1, 4, 0, 4, '#', 1);
    init_game_tile(ns_ramp, 0, 5, 1, 5, '=', 1);
    init_game_tile(ew_ramp, 2, 0, 2, 1, '\\', 1);
    init_game_tile(we_ramp, 3, 1, 3, 0, '/', 1);
    init_game_tile(platform_a, 1, 1, 1, 1, 'A', 0);
    init_game_tile(platform_b, 1, 1, 1, 1, 'B', 0);
    init_game_tile(platform_c, 1, 1, 1, 1, 'C', 0);
    init_game_tile(platform_d, 1, 1, 1, 1, 'D', 0);
    init_game_tile(platform_e, 1, 1, 1, 1, 'E', 0);
    init_game_tile(platform_f, 1, 1, 1, 1, 'F', 0);
    init_game_tile(platform_g, 1, 1, 1, 1, 'G', 0);
    init_game_tile(platform_h, 1, 1, 1, 1, 'H', 0);
}

//Places special characters on the game level
void place_special_character(int x, int y, char character, char* name){
    int j = 0;
    char platforms[9] = {'*', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
    while(j < 11){
        if (dynamic_board[y][x] == valid_special_characters[j]){
            printf("Two special objects are placed at the same coordinates %d, %d.\n", x, y);
            exit(0);
        }
        if(j == 10){
            break;
        }
        j++;
    } 
    int i = 0;
    if (dynamic_board[y][x] == '*'){
        printf("A special object was placed on a wall at %d, %d.\n", x, y);
        exit(0);
    }
    while(i < 9){
        if (dynamic_board[y][x] == platforms[i]){
            printf("%s was placed on a platform.\n", name);
            exit(0);
        }
        if(i == 8){
            dynamic_board[y][x] = character;
        }
        i++;
    } 
}


//Function to convert the strings of the special character information into coordinates. 
int get_coords(int size_of_special_character, int(*special_character)[size_of_special_character], char character, char* name) {
    char *token;
    char* buffer = special_characters[*special_character[0]];
    const char delimiter[2] = " ";
    int i = 0;

    //We never care about the first value as we already what the letter is
    token = strtok(buffer, delimiter);

    while (i < size_of_special_character){
        token = strtok(NULL, delimiter);
        if (token == NULL){
            return -1;
        }
        (*special_character)[i] = atoi(token);
        i++;
    }
    if(size_of_special_character == 2){
        place_special_character((*special_character)[0], (*special_character)[1], character, name);
        
    }
    if(size_of_special_character == 5){
        char game_switch[9] = "Switch ";
        char key[6] = "Key ";
        strcat(game_switch, name);
        strcat(key, name);
        place_special_character((*special_character)[0], (*special_character)[1], character, game_switch);
        place_special_character((*special_character)[2], (*special_character)[3], '$', key);
    }
    return 0;
}


// void switches_keys(int switches_present[9]){
//     int i = 0;
//     char platform = 'a';
//     while (i < 9){
//         if (*switches_present[i] != 0) {
//             if (get_coords(5, &platform_array) == -1){
//             printf("Could not find 5 integers for switch %d.\n", platform);
//             exit(0);
//             }
//         }
//         i++;
//         platform++;
//     }
// }








//This function sets the 2D level arrays and the positions of all special tiles
void game_load(char* user_file) {
    int x = 0;
    int y = 0;
    int game_exit[3];
    int row_length;
    int column_length;
    int is_special_characters = 0;
    int player_present = 0;
    int game_exit_present = 0;
    int platforms_present[9] = {0, 0, 0, 0, 0, 0, 0, 0};
    int switches_present[9] = {0, 0, 0, 0, 0, 0, 0, 0};;
    int a_platform[8];
    int b_platform[8];
    int c_platform[8];
    int d_platform[8];
    int e_platform[8];
    int f_platform[8];
    int g_platform[8];
    int h_platform[8];
    FILE *board_file;
    char valid_tiles[16] = {'\\', '*', '-', '+', '#', '=', '/', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
    char tile;  

    game_tile low_ground;
    game_tile high_ground;
    game_tile wall;
    game_tile sn_ramp;
    game_tile ns_ramp;
    game_tile ew_ramp;
    game_tile we_ramp;
    game_tile platform_a;
    game_tile platform_b;
    game_tile platform_c;
    game_tile platform_d;
    game_tile platform_e;
    game_tile platform_f;
    game_tile platform_g;
    game_tile platform_h;
    init_all_tiles(&low_ground, &high_ground, &wall, &sn_ramp, &ns_ramp, &ew_ramp, &we_ramp, &platform_a, &platform_b, &platform_c, &platform_d, &platform_e, &platform_f, &platform_g, &platform_h);

    board_file = fopen(user_file, "r");
    if (board_file == NULL){
        printf("File %s does not exist.\n", user_file);
        exit(0);
    }

    //This while loop loads the regular tiles
    while(1){
        tile = fgetc(board_file);

        if (tile == EOF){
            break;
        }

        //Once the regular tiles are loaded this switches to loading the special characters
        if (is_special_characters == 0){
            if (tile == '\n'){
				if (x == 0){
                    column_length = y;
                    y = 0;
                    is_special_characters = 1;
                    continue;
                }  

                if (y != 0){
                    if (x != row_length) {
                        printf("Row %d of the level had incorrect length %d; length should have been %d.\n", y, x, row_length);
                        exit(0);
                    }
                }
                else{
                    row_length = x;
                }
                x = 0;
                y++;
                continue; 
            }

            //Fix this variable name
            int i = 0;
            while(i < 16){
                if (tile == valid_tiles[i]){
                    break;
                }
                if(i == 15){
                    printf("Found invalid tile at position %d, %d in the level.\n", x, y);
                    exit(0);
                }
                i++;
            } 
            //Late night jank needs fixing

            if (x > 59){
                printf("Row %d is longer than maximum row length 60.\n", y);
                exit(0); 
            }

            if (y > 20){
                printf("Level has more rows than maximum row number 20.\n");
                exit(0);
            }

            switch(tile){
                case '*':
                    game_level[y][x] = wall;
                    break;
                case '+':
                    game_level[y][x] = high_ground;
                    break;
                case '-':
                    game_level[y][x] = low_ground;
                    break;
                case '/':
                    game_level[y][x] = we_ramp;
                    break;
                case '\\':
                    game_level[y][x] = ew_ramp;
                    break;
                case '#':
                    game_level[y][x] = sn_ramp;
                    break;
                case '=':
                    game_level[y][x] = ns_ramp;
                    break;
                case 'A':
                    platforms_present[0] = 1;
                    game_level[y][x] = platform_a;
                    break;
                case 'B':
                    platforms_present[1] = 1;
                    game_level[y][x] = platform_b;
                    break;
                case 'C':
                    platforms_present[2] = 1;
                    game_level[y][x] = platform_c;
                    break;
                case 'D':
                    platforms_present[3] = 1;
                    game_level[y][x] = platform_d;
                    break;
                case 'E':
                    platforms_present[4] = 1;
                    game_level[y][x] = platform_e;
                    break;
                case 'F':
                    platforms_present[5] = 1;
                    game_level[y][x] = platform_f;
                    break;
                case 'G':
                    platforms_present[6] = 1;
                    game_level[y][x] = platform_g;
                    break;
                case 'H':
                    platforms_present[7] = 1;
                    game_level[y][x] = platform_h;
                    break;
            }
            dynamic_board[y][x] = tile;
            x++;
        }
        else{
            //All special tile stuff
            if (y > 10){
                printf("Level file contained invalid object %c.", tile);
                exit(0);
            } 

			if (tile == '\n'){
            x = 0;
            y++;
            continue; 
        	}
            
            if (x == 0){
                int j = 0;
                while(j < 11){
                    if (tile == valid_special_characters[j]){
                        break;
                    }
                    if(j == 10){
                        printf("Level file contained invalid object %c.\n", tile);
                        exit(0);
                    }
                    j++;
                } 
            }
			special_characters[y][x] = tile;
            switch(tile){
                case 'P':
                    player[0] = y;
                    player_present = 1;
                    break;
                case 'X':
                    game_exit[0] = y;
                    game_exit_present = 1;
                    break;
                case 'a':
                    a_platform[0] = y;
                    switches_present[0] = 1;
                    break;
                case 'b':
                    b_platform[0] = y;
                    switches_present[1] = 1;
                    break;
                case 'c':
                    c_platform[0] = y;
                    switches_present[2] = 1;
                    break;
                case 'd':
                    d_platform[0] = y;
                    switches_present[3] = 1;
                    break;
                case 'e':
                    e_platform[0] = y;
                    switches_present[4] = 1;
                    break;
                case 'f':
                    f_platform[0] = y;
                    switches_present[5] = 1;
                    break;
                case 'g':
                    g_platform[0] = y;
                    switches_present[6] = 1;
                    break;
                case 'h':
                    h_platform[0] = y;
                    switches_present[7] = 1;
                    break;
            }
			x++;
		}
    }

	fclose(board_file);

    if (special_characters[0][0] == '\0'){
        printf("The file ended before any special objects had been defined.\n");
        exit(0);
    }

    if (player_present == 0){
        printf("No player is defined in the level file.\n");
        exit(0);
    }

    if (game_exit_present == 0){
        printf("No exit is defined in the level file.\n");
        exit(0);
    }

    //fix variable?
    int k = 0;
    char failed_platform = 'A';
    char failed_switch = 'a';
    while (k < 9){
        if (platforms_present[k] != switches_present[k]){
            if (platforms_present[k] == 0){
                printf("No platform exists for switch %c.\n", failed_switch);
                exit(0);
            }
            if (switches_present[k] == 0){
            printf("No switch exists for platform %c.\n", failed_platform);
            exit(0);
            }
        }
        failed_platform++;
        failed_switch++;
        k++;
    }

    if (get_coords(2, &player, 'P', "Player P") == -1){
        printf("Could not find 2 integers for player.\n");
        exit(0);
    }

    // place_special_character(player[0], player[1], 'P', "Player P");

    if (get_coords(2, &game_exit, 'X', "Exit X") == -1){
        printf("Could not find 2 integers for exit.\n");
        exit(0);
    }

    //I can make this one large function
    if (switches_present[0] == 1){
        if (get_coords(5, &a_platform, 'a', "a") == -1){
            printf("Could not find 5 integers for switch a.\n");
            exit(0);
        }
    }

    if (switches_present[1] == 1){
        if (get_coords(5, &b_platform, 'b', "b") == -1){
            printf("Could not find 5 integers for switch b.\n");
            exit(0);
        }
    }

    if (switches_present[2] == 1){
        if (get_coords(5, &c_platform, 'c', "c") == -1){
            printf("Could not find 5 integers for switch c.\n");
            exit(0);
        }
    }

    if (switches_present[3] == 1){
        if (get_coords(5, &d_platform, 'd', "d") == -1){
            printf("Could not find 5 integers for switch d.\n");
            exit(0);
        }
    }

    if (switches_present[4] == 1){
        if (get_coords(5, &e_platform, 'e', "e") == -1){
            printf("Could not find 5 integers for switch e.\n");
            exit(0);
        }
    }

    if (switches_present[5] == 1){
        if (get_coords(5, &f_platform, 'f', "f") == -1){
            printf("Could not find 5 integers for switch f.\n");
            exit(0);
        }
    }

    if (switches_present[6] == 1){
        if (get_coords(5, &g_platform, 'g', "g") == -1){
            printf("Could not find 5 integers for switch g.\n");
            exit(0);
        }
    }

    if (switches_present[7] == 1){
        if (get_coords(5, &h_platform, 'h', "") == -1){
            printf("Could not find 5 integers for switch h\n");
            exit(0);
        }
    }

    y = 0;
    while (y < column_length){
        x = 0;
        while (x < row_length){
            if (game_level[y][x].is_ramp == 1){
                switch(game_level[y][x].character){
                    case '\\':
                        if(x == 0 || x == row_length-1){
                            printf("Ramp at %d, %d leads off the board.\n", x, y);
                            break;
                        }
                        if(game_level[y][x-1].east != 1 || game_level[y][x+1].west != 0){
                            printf("Ramp at %d, %d does not start and end on the correct levels.\n", x, y);
                        }
                        break;
                    case '/':
                        if(x == 0 || x == row_length-1){
                            printf("Ramp at %d, %d leads off the board.\n", x, y);
                            break;
                        }
                        if(game_level[y][x-1].east != 0 || game_level[y][x+1].west != 1){
                            printf("Ramp at %d, %d does not start and end on the correct levels.\n", x, y);      
                        }
                        break;
                    case '=':
                        if(y == 0 || y == column_length-1){
                            printf("Ramp at %d, %d leads off the board.\n", x, y);
                            break;
                        }
                        if (game_level[y-1][x].south != 0 || game_level[y+1][x].north != 1){
                            printf("Ramp at %d, %d does not start and end on the correct levels.\n", x, y); 
                        }
                        break;
                    case '#':
                        if(y == 0 || y == column_length-1){
                            printf("Ramp at %d, %d leads off the board.\n", x, y);
                            break;
                        }
                        if (game_level[y-1][x].south != 1 || game_level[y+1][x].north != 0){
                            printf("Ramp at %d, %d does not start and end on the correct levels.\n", x, y);           
                        }
                        break;
                }
            }
            x++;
        }
        y++;
    }


    // printf("%d, %d, %d, %d\n", player[0], player[1], game_exit[0], game_exit[1]);
    // printf("%d, %d", row_length, column_length);
}



void movement() {
    char move_direction;
// May need to fix use of scanf
    scanf("%c", &move_direction);
    switch(move_direction) {
        case 'w':
            printf("%c", move_direction);
            break;
        case 'd':
            dynamic_board[player[2]][(player[3])] = game_level[player[2]][(player[3])].character;
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

void print_game_board() {
    int i = 0;
    int j = 0;
    while (i < 10){
        while(j < 10){
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
    //movement();
    return 0;
}
