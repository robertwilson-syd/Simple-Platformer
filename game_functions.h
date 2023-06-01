#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//Defining a tile struct to be used for movement rules
typedef struct board_tile {
    int north;
    int east;
    int south;
    int west;
    char character;
    int is_ramp;
    int is_platform;
} game_tile;

//Declaring global variables
game_tile game_level[21][61];
char dynamic_level[21][61];
char special_objects[11][17];
char valid_special_objects[11] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'P', 'X'};
char switch_list[9] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
char current_tile;
int player[3];
int row_length;
int column_length;
int a_platform[8];
int b_platform[8];
int c_platform[8];
int d_platform[8];
int e_platform[8];
int f_platform[8];
int g_platform[8];
int h_platform[8];
int keys_obtained[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int switches_present[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int switch_state[8] = {0, 0, 0, 0, 0, 0, 0, 0};

//This initiates a tile variable with its relevant properties
void init_game_tile(game_tile* x, int north, int east, int south, int west, char c, int is_ramp, int is_platform) {
    x->north = north;
    x->east = east;
    x->south = south;
    x->west = west;
    x->character = c;
    x->is_ramp = is_ramp;
    x->is_platform = is_platform;
}    
/*This initialises all the tile variables - low ground = 0, high ground = 1, ew_ramp = 2, we_ramp = 3, sn_ramp = 4, ns_ramp = 5, wall = 6
These integers are necessary because the player can move to adjacent ramps of the same kind*/
void init_all_tiles(game_tile* low_ground, game_tile* high_ground, game_tile* wall, game_tile* sn_ramp, game_tile* ns_ramp, game_tile* ew_ramp, game_tile* we_ramp, game_tile* platform_a, game_tile* platform_b, game_tile* platform_c, game_tile* platform_d, game_tile* platform_e, game_tile* platform_f, game_tile* platform_g, game_tile* platform_h) {
    init_game_tile(low_ground, 0, 0, 0, 0, '-', 0, 0);
    init_game_tile(high_ground, 1, 1, 1, 1, '+', 0, 0);
    init_game_tile(wall, 6, 6, 6, 6, '*', 0, 0);
    init_game_tile(sn_ramp, 1, 4, 0, 4, '#', 1, 0);
    init_game_tile(ns_ramp, 0, 5, 1, 5, '=', 1, 0);
    init_game_tile(ew_ramp, 2, 0, 2, 1, '\\', 1, 0);
    init_game_tile(we_ramp, 3, 1, 3, 0, '/', 1, 0);
    init_game_tile(platform_a, 1, 1, 1, 1, 'A', 0, 1);
    init_game_tile(platform_b, 1, 1, 1, 1, 'B', 0, 1);
    init_game_tile(platform_c, 1, 1, 1, 1, 'C', 0, 1);
    init_game_tile(platform_d, 1, 1, 1, 1, 'D', 0, 1);
    init_game_tile(platform_e, 1, 1, 1, 1, 'E', 0, 1);
    init_game_tile(platform_f, 1, 1, 1, 1, 'F', 0, 1);
    init_game_tile(platform_g, 1, 1, 1, 1, 'G', 0, 1);
    init_game_tile(platform_h, 1, 1, 1, 1, 'H', 0, 1);
}

//This functions takes the coordinates of a special object (ie player, exit, switch, key) and inserts them onto the dynamic level
void insert_special_object(int x, int y, char character, char* name){
    int j = 0;
    char platforms[9] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
    //If any previous special objects have already been placed at the coordinates an error message is printed
    while(j < 9){
        if (dynamic_level[y][x] == valid_special_objects[j]){
            printf("Two special objects are placed at the same coordinates %d, %d.\n", x, y);
            exit(0);
        }

        if(j == 8){
            break;
        }
        j++;
    }

    //Similarly this tests for a wall at the given coordinates 
    if (dynamic_level[y][x] == '*'){
        printf("A special object was placed on a wall at %d, %d.\n", x, y);
        exit(0);
    }

    //This loop tests for a platform at the given coordinates 
    int i = 0;
    while(i < 9){
        if (dynamic_level[y][x] == platforms[i]){
            printf("%s was placed on a platform.\n", name);
            exit(0);
        }
        if(i == 8){
            dynamic_level[y][x] = character;
        }
        i++;
    } 
}


//This function interprets the level file and gets the coordinates of whichever special object is input
//If it returns -1, there are insufficient characters for the given special object
int get_coords(int size_of_special_object, int(*special_object)[size_of_special_object], char character, char* name) {
    char *token;
    char* buffer = special_objects[*special_object[0]];
    const char delimiter[2] = " ";
    int i = 0;
    //We never care about the first value as it is the letter of the special object which we already know
    token = strtok(buffer, delimiter);
    while (i < size_of_special_object){
        token = strtok(NULL, delimiter);
        if (token == NULL){
            return -1;
        }
        (*special_object)[i] = atoi(token);
        i++;
    }

    if(size_of_special_object == 2){
        insert_special_object((*special_object)[0], (*special_object)[1], character, name);
        
    }

    if(size_of_special_object == 5){
        char game_switch[9] = "Switch ";
        char key[6] = "Key ";
        strcat(game_switch, name);
        strcat(key, name);
        insert_special_object((*special_object)[0], (*special_object)[1], character, game_switch);
        insert_special_object((*special_object)[2], (*special_object)[3], '$', key);
        switch_state[character - 97] = (*special_object)[4];
    }
    return 0;
}

//This function sets platforms to inactive or active initally, depending on the special object information
void initial_platform_state(){
    int i = 0;
    char platform_letter = 'A';
    while(i < 8){
        if (switch_state[i] == 0){
            int y = 0; 
            while (y < column_length){
                int x = 0;
                while (x < row_length){
                    if(game_level[y][x].character == platform_letter){
                        dynamic_level[y][x] = '-';
                        game_level[y][x].north = 0;
                        game_level[y][x].east = 0;
                        game_level[y][x].south = 0;
                        game_level[y][x].west = 0;
                    }
                    x++;
                }
                y++;
            }
        }
        i++;
        platform_letter++;
    }
}

//This function sets the 2D level arrays and the positions of all special tiles while error checking the loading process
void game_load(char* user_file) {
    int x = 0;
    int y = 0;
    int game_exit[3];
    int is_special_objects = 0;
    int player_present = 0;
    int game_exit_present = 0;
    int platforms_present[9] = {0, 0, 0, 0, 0, 0, 0, 0};
    FILE *level_file;
    char valid_tiles[16] = {'\\', '*', '-', '+', '#', '=', '/', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
    char tile;  

//Initialises game tiles for movement
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

    level_file = fopen(user_file, "r");
    if (level_file == NULL){
        printf("File %s does not exist.\n", user_file);
        exit(0);
    }

    //This while loop loads the regular tiles
    while(1){
        tile = fgetc(level_file);

        if (tile == EOF){
            break;
        }

        //Once the regular tiles are loaded this if-statement switches to loading the special objects
        if (is_special_objects == 0){
            if (tile == '\n'){
				if (x == 0){
                    column_length = y;
                    y = 0;
                    is_special_objects = 1;
                    continue;
                }  

                if (y != 0){
                    if (x != row_length) {
                        printf("Row %d of the level had incorrect length %d; length should have been %d.\n", y, x, row_length);
                        fclose(level_file);
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
            //This loop checks each character in the level map to ensure they are valid
            int i = 0;
            while(i < 16){
                if (tile == valid_tiles[i]){
                    break;
                }
                if(i == 15){
                    printf("Found invalid tile at position %d, %d in the level.\n", x, y);
                    fclose(level_file);
                    exit(0);
                }
                i++;
            } 

            if (x > 59){
                printf("Row %d is longer than maximum row length 60.\n", y);
                exit(0); 
            }

            if (y > 20){
                printf("Level has more rows than maximum row number 20.\n");
                exit(0);
            }

            //This switch statement converts the characters from the level into game_tile structs in order to create a 2D array of game_tiles which acts as a static level map
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
            dynamic_level[y][x] = tile;
            x++;
        }
        //This loads, interprets and inserts the special object information onto the dynamic level
        else{
            if (y > 10){
                printf("Level file contained invalid object %c.", tile);
                fclose(level_file);
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
                    if (tile == valid_special_objects[j]){
                        break;
                    }
                    if(j == 10){
                        printf("Level file contained invalid object %c.\n", tile);
                        fclose(level_file);
                        exit(0);
                    }
                    j++;
                } 
            }
			special_objects[y][x] = tile;
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
	fclose(level_file);

    if (special_objects[0][0] == '\0'){
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

    //Checks that there is a switch for each platform and vice versa
    int k = 0;
    char failed_platform = 'A';
    char failed_switch = 'a';
    while (k < 8){
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
    current_tile = game_level[player[1]][player[0]].character;

    if (get_coords(2, &game_exit, 'X', "Exit X") == -1){
        printf("Could not find 2 integers for exit.\n");
        exit(0);
    }

    //This block of functions checks if a switch is present in the level file
    //If a switch is present it inserts it into the level
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

    initial_platform_state();

//This block checks that all the ramps begin and end correctly
    y = 0;
    while (y < column_length){
        x = 0;
        while (x < row_length){
            if (game_level[y][x].is_ramp == 1){
                switch(game_level[y][x].character){
                    case '\\':
                        if(x == 0 || x == row_length-1){
                            printf("Ramp at %d, %d leads off the board.\n", x, y);
                            exit(0);
                            break;
                        }
                        if((game_level[y][x-1].east != 1 && game_level[y][x-1].is_platform != 1) || (game_level[y][x+1].west != 0 && game_level[y][x+1].is_platform != 1)){
                            printf("Ramp at %d, %d does not start and end on the correct levels.\n", x, y);
                            exit(0);
                        }
                        break;
                    case '/':
                        if(x == 0 || x == row_length-1){
                            printf("Ramp at %d, %d leads off the board.\n", x, y);
                            exit(0);
                        }
                        if((game_level[y][x-1].east != 0 && game_level[y][x-1].is_platform != 1) || (game_level[y][x+1].west != 1 && game_level[y][x+1].is_platform != 1)){
                            printf("Ramp at %d, %d does not start and end on the correct levels.\n", x, y);  
                            exit(0);    
                        }
                        break;
                    case '=':
                        if(y == 0 || y == column_length-1){
                            printf("Ramp at %d, %d leads off the board.\n", x, y);
                            exit(0);
                        }
                        if ((game_level[y-1][x].south != 0 && game_level[y-1][x].is_platform != 1) || (game_level[y+1][x].north != 1 && game_level[y+1][x].is_platform != 1)){
                            printf("Ramp at %d, %d does not start and end on the correct levels.\n", x, y); 
                            exit(0);
                        }
                        break;
                    case '#':
                        if(y == 0 || y == column_length-1){
                            printf("Ramp at %d, %d leads off the board.\n", x, y);
                            exit(0);
                        }
                        if ((game_level[y-1][x].south != 1 && game_level[y-1][x].is_platform != 1) || (game_level[y+1][x].north != 0 && game_level[y+1][x].is_platform != 1)){
                            printf("Ramp at %d, %d does not start and end on the correct levels.\n", x, y);    
                            exit(0);       
                        }
                        break;
                }
            }
            x++;
        }
        y++;
    }
}

//The help function, C automatically concatenates the adjacent functions
void help() {
    printf("w: Move up one space.\n"
"s: Move down one space.\n"
"a: Move left one space.\n"
"d: Move right one space.\n"
"x: Toggle the state of the switch you are standing on.\n"
"k: See which keys you have collected.\n"
"e: See which platforms are on and which platforms are off.\n"
"h: Print this help message.\n"
"q: Quit the game.\n");
}

//This functions prints the board by printing each charcter of the dynamic level so that it includes special objects
void print_board(){
    int y = 0;
    while (y < column_length){
        int x = 0;
        while (x < row_length){
            if(dynamic_level[y][x] == '*'){
                printf(" ");
            }    
            else{
                printf("%c", dynamic_level[y][x]);    
            }
            x++;
        }
        y++;
        printf("\n");
    }
}

//moved_onto_key is given the position of an unknown key and find which switch the key is for, printing the relevant message
void moved_onto_key(int x, int y){
    if(x == a_platform[2] && y == a_platform[3]){
        keys_obtained[0] = 1;
        printf("You picked up the key for switch a!\n");
    }
    if(x == b_platform[2] && y == b_platform[3]){
        keys_obtained[1] = 1;
        printf("You picked up the key for switch b!\n");
    }
    if(x == c_platform[2] && y == c_platform[3]){
        keys_obtained[2] = 1;
        printf("You picked up the key for switch c!\n");
    }
    if(x == d_platform[2] && y == d_platform[3]){
        keys_obtained[3] = 1;
        printf("You picked up the key for switch d!\n");
    }
    if(x == e_platform[2] && y == e_platform[3]){
        keys_obtained[4] = 1;
        printf("You picked up the key for switch e!\n");
    }
    if(x == f_platform[2] && y == f_platform[3]){
        keys_obtained[5] = 1;
        printf("You picked up the key for switch f!\n");
    }
    if(x == g_platform[2] && y == g_platform[3]){
        keys_obtained[6] = 1;
        printf("You picked up the key for switch g!\n");
    }
    if(x == h_platform[2] && y == h_platform[3]){
        keys_obtained[7] = 1;
        printf("You picked up the key for switch h!\n");
    }
}

/*This function takes a direction and checks if the player can move in that directions
If it is a valid move, it will change the dynamic level such that P is in the new position and the tile the player was on is restored
The function also checks if the player has reached the exit or if they have moved onto a key*/
void movement_verification(char direction){
    char moving_to;
    if(direction == 'w'){
        moving_to = dynamic_level[player[1]-1][player[0]];
        
        if (player[1] == 0){
            return;
        }

        if (game_level[player[1]][player[0]].north == game_level[player[1]-1][player[0]].south){
            if (moving_to == 'X'){
                printf("Congratulations, you win!\n");
                exit(0);
            }

            dynamic_level[player[1]][player[0]] = current_tile;
            current_tile = dynamic_level[player[1]-1][player[0]];

            if (moving_to == '$'){
                moved_onto_key(player[0], player[1]-1);
                current_tile = game_level[player[1]-1][player[0]].character;
            }

            dynamic_level[player[1]-1][player[0]] = 'P';
            player[1] = player[1] - 1;
        }
    }
    else if(direction == 'd'){
        moving_to = dynamic_level[player[1]][player[0]+1];        
        
        if (player[1] == (row_length - 1)){
            return;
        }

        if (game_level[player[1]][player[0]].east == game_level[player[1]][player[0]+1].west){
            if (moving_to == 'X'){
                printf("Congratulations, you win!\n");
                exit(0);
            }

            
            dynamic_level[player[1]][player[0]] = current_tile;
            current_tile = dynamic_level[player[1]][player[0]+1];

            if (moving_to == '$'){
                moved_onto_key(player[0]+1, player[1]);
                current_tile = game_level[player[1]][player[0]+1].character;
            }

            dynamic_level[player[1]][player[0]+1] = 'P';
            player[0]++;
        }
        return;
    }
    else if(direction == 's'){
        moving_to = dynamic_level[player[1]+1][player[0]];
        if (player[1] == (row_length - 1)){
            return;
        }

        if (game_level[player[1]][player[0]].south == game_level[player[1]+1][player[0]].north){
            if (moving_to == 'X'){
                printf("Congratulations, you win!\n");
                exit(0);
            }

            dynamic_level[player[1]][player[0]] = current_tile;
            current_tile = dynamic_level[player[1]+1][player[0]];

            if (moving_to == '$'){
                moved_onto_key(player[0], player[1]+1);
                current_tile = game_level[player[1]+1][player[0]].character;
            }

            dynamic_level[player[1]+1][player[0]] = 'P';
            player[1]++;
        }
        return;
    }
    else{
        moving_to = dynamic_level[player[1]][player[0]-1];
        if (player[1] == (row_length - 1)){
            return;
        }

        if (game_level[player[1]][player[0]].west == game_level[player[1]][player[0]-1].east){
            if (moving_to == 'X'){
                printf("Congratulations, you win!\n");
                exit(0);
            }

            dynamic_level[player[1]][player[0]] = current_tile;
            current_tile = dynamic_level[player[1]][player[0]-1];

            if (moving_to == '$'){
                moved_onto_key(player[0]-1, player[1]);
                current_tile = game_level[player[1]][player[0]-1].character;
            }

            dynamic_level[player[1]][player[0]-1] = 'P';
            player[0] = player[0] - 1;
        }
        return;
    }
}

//check_for_switch checks for a switch at the player's current position
char check_for_switch(int x, int y){
    if(x == a_platform[0] && y == a_platform[1]){
        return 'a';
    }
    if(x == b_platform[0] && y == b_platform[1]){
        return 'b';
    }
    if(x == c_platform[0] && y == c_platform[1]){
        return 'c';
    }
    if(x == d_platform[0] && y == d_platform[1]){
        return 'd';
    }
    if(x == e_platform[0] && y == e_platform[1]){
        return 'e';
    }
    if(x == f_platform[0] && y == f_platform[1]){
        return 'f';
    }
    if(x == g_platform[0] && y == g_platform[1]){
        return 'g';
    }
    if(x == h_platform[0] && y == h_platform[1]){
        return 'h';
    }
    return 'z';
}

//This function toggles the state of the given platform 
//Platform number is the position in the switch_state array ie platform a = 0, plaform b = 1, etc
void toggle_platforms(int platform_number, char platform_letter){
    if(switch_state[platform_number] == 1){
        int y = 0;
        switch_state[platform_number] = 0;
        while (y < column_length){
            int x = 0;
            while (x < row_length){
                if(game_level[y][x].character == platform_letter){
                    dynamic_level[y][x] = '-';
                    game_level[y][x].north = 0;
                    game_level[y][x].east = 0;
                    game_level[y][x].south = 0;
                    game_level[y][x].west = 0;
                }
                x++;
            }
            y++;
        }
    }
    else{
        switch_state[platform_number] = 1;
        int y = 0;
        while (y < column_length){
            int x = 0;
            while (x < row_length){
                if(game_level[y][x].character == platform_letter){
                    dynamic_level[y][x] = platform_letter;
                    game_level[y][x].north = 1;
                    game_level[y][x].east = 1;
                    game_level[y][x].south = 1;
                    game_level[y][x].west = 1;
                }    
                x++;
            }
            y++;
        }
    }
}

//This is the function called by the 'x' command. It checks the player is standing on a switch and has the correct key before toggling the platforms
void toggle_switch(int x, int y){
    char switch_name;
    switch_name = check_for_switch(x, y);
    if (switch_name == 'z'){
        printf("You are not standing on a switch!\n");
        return;
    }
    switch(switch_name){
        case 'a':
            if (keys_obtained[0] == 1){
                toggle_platforms(0, 'A');
            }
            else{
                printf("You do not have the key for that switch!\n");
            }
            break;
        case 'b':
            if (keys_obtained[1] == 1){
                toggle_platforms(1, 'B');
            }
            else{
                printf("You do not have the key for that switch!\n");
            }
            break;
        case 'c':
            if (keys_obtained[2] == 1){
                toggle_platforms(2, 'C');
            }
            else{
                printf("You do not have the key for that switch!\n");
            }
            break;
        case 'd':
            if (keys_obtained[3] == 1){
                toggle_platforms(3, 'D');
            }
            else{
                printf("You do not have the key for that switch!\n");
            }
            break;
        case 'e':
            if (keys_obtained[4] == 1){
                toggle_platforms(4, 'E');
            }
            else{
                printf("You do not have the key for that switch!\n");
            }
            break;
        case 'f':
            if (keys_obtained[5] == 1){
                toggle_platforms(5, 'F');
            }
            else{
                printf("You do not have the key for that switch!\n");
            }
            break;
        case 'g':
            if (keys_obtained[6] == 1){
                toggle_platforms(6, 'G');
            }
            else{
                printf("You do not have the key for that switch!\n");
            }
            break;
        case 'h':
            if (keys_obtained[7] == 1){
                toggle_platforms(7, 'H');
            }
            else{
                printf("You do not have the key for that switch!\n");
            }
            break;
    }
    return;
}

//This function is called by the 'k' command, it checks which switches are present and prints whether the relevant key has been obtained or not
void print_keys(){
    int i = 0;
    char key_letter = 'a';
    while(i < 8){
        if(switches_present[i] == 1){
            printf("%c: %d\n", key_letter, keys_obtained[i]);
        }
        i++;
        key_letter++;  
    }
}

//This function is called by the 'e' command and prints the state of all platforms present in the level
void print_switches(){
    int i = 0;
    char key_letter = 'A';
    while(i < 8){
        if(switches_present[i] == 1){
            printf("%c: %d\n", key_letter, switch_state[i]);
        }
        i++;
        key_letter++;  
    }
}

//This is the command which accepts user input, checks it's valid and if so it then calls the relevant function
void user_command(){
    char input[3];
    int extra_data_found = 0;
    char extra_char;
    input[2] = 'x';
    if (fgets(input, 3, stdin) == NULL) {
    }
    if (input[2] == '\0' && input[1] != '\n') {
        // Cope with potential extra data in `stdin`: read and toss
        extra_char = fgetc(stdin);
        while (extra_char != '\n' && extra_char != EOF) {
            extra_char = fgetc(stdin);
            extra_data_found = 1;
        }
    }
    if (input[1] != '\n'){
        printf("Command not recognised.\n");
        return;
    }

    if(extra_data_found == 1){
        printf("Command not recognised.\n");
        return;
    }

    switch(input[0]){
        case 'h':
            help();
            break;
        case 'q':
            exit(0);
        case 'w':
            movement_verification('w');
            break;
        case 'd':
            movement_verification('d');
            break;
        case 's':
            movement_verification('s');
            break;
        case 'a':
            movement_verification('a');
            break;
        case 'x':
            toggle_switch(player[0], player[1]);
            break;
        case 'k':
            print_keys();
            break;
        case 'e':
            print_switches();
            break;
        default:
            printf("Command not recognised.\n");
            break;
    }
}


