#pragma once

// window constants
#define WINDOW (vector_t){.x = 1000, .y = 800}
#define CENTER (vector_t){.x = 500, .y = 400}

// backdrop constants
#define GAME_BACKDROP_PATH "assets/backdrop.png"

// color constants
#define COLOR_BRIGHTNESS 0.7
#define BLACK (rgb_color_t){.r = 0.0, .g = 0.0, .b = 0.0}
#define WHITE (rgb_color_t){.r = 1.0, .g = 1.0, .b = 1.0}

#define RED (rgb_color_t){.r = 1.0, .g = 0.0, .b = 0.0}
#define GREEN (rgb_color_t){.r = 0.0, .g = 1.0, .b = 0.0}
#define BLUE (rgb_color_t){.r = 0.0, .g = 0.0, .b = 1.0}

// gravity constants
#define G_CONST 6.67E-11               // N m^2 / kg^2
#define EARTH_MASS 6E24                // kg
#define ACC_GRAV 9.8                   // m / s^2
#define EARTH_RADIUS 6.048E5           // m
#define EARTH_IDX 0
#define GROUND_CENTER (vector_t){.x = CENTER.x, .y = -3000.0}

// text display constants
#define SCORE_FONT_PATH "assets/ARCADECLASSIC.TTF"
#define SCORE_FONT_SIZE 35
#define COIN_FONT_COLOR {255, 255, 255}
#define WEALTH_DISPLAY_IDX 10
#define WEALTH_DISPLAY_CENTER (vector_t){.x = 85, .y = WINDOW.y - 34}

// player constants
#define PLAYER_HEIGHT 120.0
#define PLAYER_WIDTH 100.0
#define PLAYER_CENTER (vector_t){.x = CENTER.x, .y = 100}
#define PLAYER_COLOR BLACK
#define PLAYER_IDX 11
#define PLAYER_MOVE_SPEED 400.0
#define PLAYER_JUMP_SPEED 720.0
#define PLAYER_MASS 10.0
#define PLAYER_TXT_SCALE 0.45
#define PHYSICS_MAJOR_GRAVITY 1.3 
#define CS_MAJOR_PATH "assets/cs_major.png"
#define DEAD_CS_MAJOR_PATH "assets/dead_cs_major.png"
#define JETPACK_CS_MAJOR_PATH "assets/jetpack_cs_major.png"
#define PHYSICS_MAJOR_PATH "assets/physics_major.png"
#define DEAD_PHYSICS_MAJOR_PATH "assets/dead_physics_major.png"
#define JETPACK_PHYSICS_MAJOR_PATH "assets/jetpack_physics_major.png"
#define BEM_MAJOR_PATH "assets/bem_major.png"
#define DEAD_BEM_MAJOR_PATH "assets/dead_bem_major.png"
#define JETPACK_BEM_MAJOR_PATH "assets/jetpack_bem_major.png"
#define BEM_COIN_MULTIPLIER 2

// platform constants
#define BUFFER_COEFFICIENT 2
#define INITIAL_PLATFORM_SPACING 50
#define PLATFORM_SPACING_CHANGE 50
#define PLATFORM_WIDTH 120
#define PLATFORM_HEIGHT 20
#define PLATFORM_COLOR GREEN
#define PLATFORM_TXT_SCALE 0.38
#define WIGGLE_ROOM_FACTOR 5
#define COLLISION_UP_THRESHOLD 0.98
#define MOVING_PLATFORM_VEL_MIN 100
#define MOVING_PLATFORM_VEL_MAX 160
#define SPRING_PLATFORM_ELASTICITY 2

// platform probs
#define MOVING_PLATFORM_PROB 0.1 // TEST VALUE
#define BREAKABLE_PLATFORM_PROB 0.1 // TEST VALUE
#define SPRING_PLATFORM_PROB 0.1 // TEST VALUE
#define DEATH_PLATFORM_PROB 0.05 // TEST VALUE

// platform paths
#define STANDARD_PLATFORM_PATH "assets/platform_standard.png"
#define MOVING_PLATFORM_PATH "assets/platform_moving.png"
#define BREAKABLE_PLATFORM_PATH "assets/platform_breakable.png"
#define GRASS_PLATFORM_PATH "assets/platform_grass.png"
#define SPRING_PLATFORM_PATH "assets/platform_spring.png"
#define SAVE_PLATFORM_PATH "assets/platform_death_save.png"

// coin constants
#define BRONZE_SPAWN_RATE 0.25
#define SILVER_SPAWN_RATE 0.05
#define GOLD_SPAWN_RATE 0.02
#define COIN_SPAWN_RATE BRONZE_SPAWN_RATE + SILVER_SPAWN_RATE + GOLD_SPAWN_RATE
#define BRONZE_VALUE 1
#define SILVER_VALUE 2
#define GOLD_VALUE 5
#define COIN_HEIGHT 75
#define COIN_RADIUS 25
#define COIN_COLOR RED
#define COIN_TXT_SCALE 0.5
#define BRONZE_COIN_PATH "assets/bronze_dogecoin.png"
#define SILVER_COIN_PATH "assets/silver_dogecoin.png"
#define GOLD_COIN_PATH "assets/gold_dogecoin.png"

// jetpack constants
#define STAR_SPAWN_RATE 0.03
#define STAR_HEIGHT 75
#define STAR_RADIUS 20
#define STAR_COLOR RED
#define PLAYER_FLIGHT_SPEED 800
#define FLIGHT_TIME 5
#define STAR_PATH "assets/star.png"

// wall collision constants
typedef enum { R_IDX, L_IDX, U_IDX, D_IDX } wall_idx_t;

// menu button constants
#define MENU_BUTTON_RADIUS 20
#define MENU_BUTTON_CENTER (vector_t){.x = WINDOW.x - 40, .y = WINDOW.y - 40}

// menu display constants
#define PLAY_RECT_TOP_RIGHT (vector_t){.x = 605, .y = 439}
#define PLAY_RECT_BOTTOM_LEFT (vector_t){.x = 396, .y = 361}
#define SHOP_RECT_TOP_RIGHT (vector_t){.x = 605, .y = 320}
#define SHOP_RECT_BOTTOM_LEFT (vector_t){.x = 395, .y = 235}
#define MENU_PATH "assets/menu.png"
#define DEATH_MENU_PATH "assets/death_menu.png"
#define PLAY_OVERLAY_PATH "assets/play_overlay.png"

// shop constants
#define SHOP_PATH "assets/shop.png"
#define SHOP_TOP_RIGHT (vector_t){.x = 810 , .y = 510}
#define SHOP_BUTTON_WIDTH 145
#define SHOP_BUTTON_HEIGHT 44
#define SHOP_HORIZ_SPACING 93
#define SHOP_VERT_SPACING 127
#define BACK_TOP_RIGHT (vector_t){.x = 280, .y = 690}
#define BACK_BOTTOM_LEFT (vector_t){.x = 160, .y = 635}
#define SHOP_NUM_ROWS 3
#define SHOP_NUM_COLS 3
#define BTN_TXT_SCALE 1.0
#define FIRST_BTN_IDX 1
#define JUMP_BOOST 0.3 * PLAYER_JUMP_SPEED
#define MOVE_BOOST 0.4 * PLAYER_MOVE_SPEED
#define SAVE_PLATFORM_BUFFER 210
#define UNLOCK_10_BTN_PATH "assets/unlock10.png"
#define UNLOCK_25_BTN_PATH "assets/unlock25.png"
#define UNLOCK_50_BTN_PATH "assets/unlock50.png"
#define UNLOCK_100_BTN_PATH "assets/unlock100.png"
#define UNLOCK_1000_BTN_PATH "assets/unlock1000.png"
#define SELECT_BTN_PATH "assets/select_button.png"
#define SELECTED_BTN_PATH "assets/selected_button.png"

// cursor constants
#define STANDARD_CURSOR_PATH "assets/standard_cursor.png"
#define GRABBY_CURSOR_PATH "assets/grabby_cursor.png"

// sound constants
#define SCREAM_PATH "assets/Lynn-screaming.wav"
#define CS_AUDIO_PATH "assets/I-have-sets-to-do.wav"
#define PHYSICS_AUDIO_PATH "assets/im-a-real-scientist.wav"
#define BEM_AUDIO_PATH "assets/money-money-money.wav"

// ground constant
#define GROUND_PATH "assets/ground.png"
#define GROUND_SCALE 0.72


double rand_double(double min, double max);

typedef enum {
  PLAYER,
  DEAD_PLAYER,
  JETPACK_PLAYER,
  PLATFORM,
  GRAVITY,
  GROUND,
  TEXT,
  COIN,
  STAR,
  BUTTON,
} body_type_t;

typedef enum {
  CS_MAJOR,
  DEAD_CS_MAJOR,
  JETPACK_CS_MAJOR,
  PHYSICS_MAJOR,
  DEAD_PHYSICS_MAJOR,
  JETPACK_PHYSICS_MAJOR,
  BEM_MAJOR,
  DEAD_BEM_MAJOR,
  JETPACK_BEM_MAJOR,
  STANDARD_PLATFORM,
  GRASS_PLATFORM,
  BREAKABLE_PLATFORM,
  MOVING_PLATFORM,
  SPRING_PLATFORM,
  SAVE_PLATFORM,
  BRONZE_COIN,
  SILVER_COIN,
  GOLD_COIN,
  SELECT_BTN,
  SELECTED_BTN,
  UNLOCK_10_BTN,
  UNLOCK_25_BTN,
  UNLOCK_50_BTN,
  UNLOCK_100_BTN,
  UNLOCK_1000_BTN,
  JETPACK,
  BACKDROP,
  GROUND_GRASS,
  SCORE_TEXT,
  // add subtypes here
} body_subtype_t;

typedef enum {
  BEM_MAJOR_BUTTON,
  PHYSICS_MAJOR_BUTTON,
  CS_MAJOR_BUTTON,
  DEATH_SAVE_BUTTON,
  JUMP_SPEED_BUTTON,
  MOVE_SPEED_BUTTON,
  MORE_RAM_BUTTON,
  COIN_BUTTON,
  JETPACK_BUTTON,
  MENU_BUTTON,
  PLAY_BUTTON,
  SHOP_BUTTON,
  BACK_BUTTON,
  NO_BUTTON,
} button_t;

typedef enum {
  BEM_AUDIO,
  CS_AUDIO,
  PHYSICS_AUDIO,
  SCREAM_AUDIO,
} sound_t;

typedef enum {
  PLAY,
  GAME_OVER,
  MENU,
  DEATH_MENU,
  SHOP,
} game_state_t;

typedef struct state {
  scene_t *scene;
  game_state_t game_mode;
  int32_t num_coins;
  int32_t lives;
  double altitude;
  double timer;
  SDL_Cursor *standard_cursor;
  SDL_Cursor *grabby_cursor;
} state_t;

void load_dynamic_bodies(state_t *state);

body_type_t *make_type_info(body_type_t type);

body_subtype_t *make_subtype_info(body_subtype_t subtype);

body_type_t get_type(body_t *body);

body_subtype_t get_subtype(body_t *body);

SDL_Texture *get_subtype_texture(body_subtype_t subtype);

SDL_Texture *get_text_texture(body_subtype_t text_type, char *text, SDL_Color color);

SDL_Texture *get_overlay_texture(game_state_t game_mode);