/**
 * @file structures.h
 * @brief This file defines data structures of the game, especially the ball.
 * It also contains enums for elements of those structures.
 * @author Sebastian Dine
 */

/**
 * @brief This enum defines the movement direction of the ball.
 */
enum ball_dir {UP=1, DOWN=2};

/**
 * @brief This enum defines, if the current @link ball_struct.angle @endlink is positive or negative.
 */
enum ball_angle_dir {LEFT=1, RIGHT=2};

/**
 * @brief This enumeration describes the movement angle of a ball.
 * The values describe y axis values, the ball will move once per frame.
 * <br>
 * HORZ = 0 px -> 0 decree <br>
 * MIN = 1  px -> aprox. 25 decree <br>
 * MID = 2 px -> aprox. 50 decree <br>
 * MAX = 4 px -> aprox. 60 decree <br>
 */
enum ball_angle {HORZ=0, MIN=1, MID=2, MAX=4};

/**
 * @brief This struct represents the games ball.
 */
typedef struct ball_struct{
    unsigned char pos_x;
    unsigned char pos_y;
    unsigned char speed;  /* pixels per frame, which the ball moves */
    enum ball_dir dir;
    enum ball_angle angle;
    enum ball_angle_dir angle_dir;
};

/**
 * @brief This structure represents the volume of the playfield by defining
 * its edges.
 */
typedef struct playfield_struct{
    unsigned char edge_top;
    unsigned char edge_bottom;
    unsigned char edge_left;
    unsigned char edge_right;
};

/**
 * @brief This strucutre represents the player.
 */
typedef struct player_struct{
    unsigned char pos_x; /* no y coordinate required */
    unsigned char score;
    unsigned char lives;
    unsigned char speed;

};