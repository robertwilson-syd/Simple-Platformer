#include <stdio.h>

typedef struct game_tile {
    char* north;
    char* east;
    char* south;
    char* west;
    char c;
} tile;

void init_game_tile(tile* x, char* "north", char* "east", char* "south", char* "west", char c) {
    x->north = "north";
    x->east = "east";
    x->south = "south";
    x->west = "west";
    x->c = c;
}    

int main() {
    tile y;
    init_game_tile(&y, 1, 0, 'a');
    printf("%d\n %d\n %c\n", y.position[0], y.position[1], y.c);

    return 0;

}

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
//     tile ramp1;
//     init_game_tile(&high_ground, "hg", "hg", "hg", "hg", 'A', 0);
//     tile ramp2;
//     init_game_tile(&high_ground, "hg", "hg", "hg", "hg", 'B', 0);
//     tile ramp3;
//     init_game_tile(&high_ground, "hg", "hg", "hg", "hg", 'C', 0);
//     tile ramp4;
//     init_game_tile(&high_ground, "hg", "hg", "hg", "hg", 'D', 0);
//     tile ramp5;
//     init_game_tile(&high_ground, "hg", "hg", "hg", "hg", 'E', 0);
//     tile ramp6;
//     init_game_tile(&high_ground, "hg", "hg", "hg", "hg", 'F', 0);
//     tile ramp7;
//     init_game_tile(&high_ground, "hg", "hg", "hg", "hg", 'G', 0);
//     tile ramp8;
//     init_game_tile(&high_ground, "hg", "hg", "hg", "hg", 'H', 0);
// }


// typedef struct game_tile {
//     char* north;
//     char* east;
//     char* south;
//     char* west;
//     char c;
//     int is_ramp;
// } tile;

