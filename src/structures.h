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
 */
enum ball_angle {HORZ=0, MIN=2, MID=3, MAX=5};

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
 * @brief This strucutre represents the player.
 */
typedef struct player_struct{
    unsigned char pos_x; /* no y coordinate required */
    unsigned char score_digit1;
    unsigned char score_digit2;
    unsigned char score_digit3;
    unsigned char lives;
    unsigned char speed;
    unsigned int score;
};

/**
 * @brief This structure represents the brick, which was hit by the ball.
 */
typedef struct brickhit_struct{
    unsigned int tile_left;
    unsigned int tile_right;
};