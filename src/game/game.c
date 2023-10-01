#include "forces.h"
#include "scene.h"
#include "sdl_wrapper.h"
#include "shapes.h"
#include "state.h"

#include <emscripten.h>

#include <assert.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include "game.h"
#include "platform.c"
#include "player.c"
#include "interface.c"

/* generate a random floating point number from min to max */
double rand_double(double min, double max) {
  double range = (max - min); 
  double div = RAND_MAX / range;
  return min + (rand() / div);
}

body_type_t *make_type_info(body_type_t type) {
  body_type_t *info = malloc(sizeof(*info));
  *info = type;
  return info;
}

body_subtype_t *make_subtype_info(body_subtype_t subtype) {
  body_subtype_t *sub_info = malloc(sizeof(*sub_info));
  *sub_info = subtype;
  return sub_info;
}

body_type_t get_type(body_t *body) {
  return *(body_type_t *)body_get_info(body);
}

body_subtype_t get_subtype(body_t *body) {
  return *(body_subtype_t *)body_get_sub_info(body);
}

// font asset management
SDL_Texture *get_text_texture(body_subtype_t text_type, char *text, SDL_Color color) {
  static TTF_Font *score_font = NULL;

  switch (text_type) {
    case SCORE_TEXT: 
      if (score_font == NULL) {
        if (text == NULL) {
          TTF_CloseFont(score_font);
          return NULL;
        }
        score_font = sdl_get_font(SCORE_FONT_PATH, SCORE_FONT_SIZE);
      }
      return text_to_texture(score_font, text, color);
    default:
      return NULL;
  }
}

SDL_Texture *get_overlay_texture(game_state_t game_mode) {
  static SDL_Texture *menu = NULL;
  static SDL_Texture *shop_overlay = NULL;
  static SDL_Texture *death_menu = NULL;
  static SDL_Texture *play_overlay = NULL;

  switch (game_mode) {
    case MENU:
      if (menu == NULL) {
        menu = sdl_get_texture(MENU_PATH);
      }
      return menu;
    case SHOP:
      if (shop_overlay == NULL) {
        shop_overlay = sdl_get_texture(SHOP_PATH);
      }
      return shop_overlay;
    case DEATH_MENU:
      if (death_menu == NULL) {
        death_menu = sdl_get_texture(DEATH_MENU_PATH);
      }
      return death_menu;
    case PLAY:
      if (play_overlay == NULL) {
        play_overlay = sdl_get_texture(PLAY_OVERLAY_PATH);
      }
      return play_overlay;
    default:
      return NULL;
  }
}

// image asset management
SDL_Texture *get_subtype_texture(body_subtype_t subtype) {
  static SDL_Texture *cs_major = NULL;
  static SDL_Texture *dead_cs_major = NULL;
  static SDL_Texture *jetpack_cs_major = NULL;
  static SDL_Texture *physics_major = NULL;
  static SDL_Texture *dead_physics_major = NULL;
  static SDL_Texture *jetpack_physics_major = NULL;
  static SDL_Texture *bem_major = NULL;
  static SDL_Texture *dead_bem_major = NULL;
  static SDL_Texture *jetpack_bem_major = NULL;

  static SDL_Texture *standard_platform = NULL;
  static SDL_Texture *grass_platform = NULL;
  static SDL_Texture *breakable_platform = NULL;
  static SDL_Texture *moving_platform = NULL;
  static SDL_Texture *spring_platform = NULL;
  static SDL_Texture *save_platform = NULL;
  static SDL_Texture *bronze_coin = NULL;
  static SDL_Texture *silver_coin = NULL;
  static SDL_Texture *gold_coin = NULL;
  static SDL_Texture *jetpack = NULL;

  static SDL_Texture *select_btn = NULL;
  static SDL_Texture *selected_btn = NULL;
  static SDL_Texture *unlock_10_btn = NULL;
  static SDL_Texture *unlock_25_btn = NULL;
  static SDL_Texture *unlock_50_btn = NULL;
  static SDL_Texture *unlock_100_btn = NULL;
  static SDL_Texture *unlock_1000_btn = NULL;
  static SDL_Texture *backdrop = NULL;

  static SDL_Texture *ground = NULL;

  switch (subtype) {
    case CS_MAJOR:
      if (cs_major == NULL) {
        cs_major = sdl_get_texture(CS_MAJOR_PATH);
      }
      return cs_major;
    case DEAD_CS_MAJOR:
      if (dead_cs_major == NULL) {
        dead_cs_major = sdl_get_texture(DEAD_CS_MAJOR_PATH);
      }
      return dead_cs_major;
    case JETPACK_CS_MAJOR:
      if (jetpack_cs_major == NULL) {
        jetpack_cs_major = sdl_get_texture(JETPACK_CS_MAJOR_PATH);
      }
      return jetpack_cs_major;
    case PHYSICS_MAJOR:
      if (physics_major == NULL) {
        physics_major = sdl_get_texture(PHYSICS_MAJOR_PATH);
      }
      return physics_major;
    case DEAD_PHYSICS_MAJOR:
      if (dead_physics_major == NULL) {
        dead_physics_major = sdl_get_texture(DEAD_PHYSICS_MAJOR_PATH);
      }
      return dead_physics_major;
    case JETPACK_PHYSICS_MAJOR:
      if (jetpack_physics_major == NULL) {
        jetpack_physics_major = sdl_get_texture(JETPACK_PHYSICS_MAJOR_PATH);
      }
      return jetpack_physics_major;
    case BEM_MAJOR:
      if (bem_major == NULL) {
        bem_major = sdl_get_texture(BEM_MAJOR_PATH);
      }
      return bem_major;
    case DEAD_BEM_MAJOR:
      if (dead_bem_major == NULL) {
        dead_bem_major = sdl_get_texture(DEAD_BEM_MAJOR_PATH);
      }
      return dead_bem_major;
    case JETPACK_BEM_MAJOR:
      if (jetpack_bem_major == NULL) {
        jetpack_bem_major = sdl_get_texture(JETPACK_BEM_MAJOR_PATH);
      }
      return jetpack_bem_major;
    case STANDARD_PLATFORM:
      if (standard_platform == NULL) {
        standard_platform = sdl_get_texture(STANDARD_PLATFORM_PATH);
      }
      return standard_platform;
    case GRASS_PLATFORM:
      if (grass_platform == NULL) {
        grass_platform = sdl_get_texture(GRASS_PLATFORM_PATH);
      }
      return grass_platform;
    case BREAKABLE_PLATFORM:
      if (breakable_platform == NULL) {
        breakable_platform = sdl_get_texture(BREAKABLE_PLATFORM_PATH);
      }
      return breakable_platform;
    case MOVING_PLATFORM:
      if (moving_platform == NULL) {
        moving_platform = sdl_get_texture(MOVING_PLATFORM_PATH);
      }
      return moving_platform;
    case SPRING_PLATFORM:
      if (spring_platform == NULL) {
        spring_platform = sdl_get_texture(SPRING_PLATFORM_PATH);
      }
      return spring_platform;
    case SAVE_PLATFORM:
      if (save_platform == NULL) {
        save_platform = sdl_get_texture(SAVE_PLATFORM_PATH);
      }
      return save_platform;
    case BRONZE_COIN:
      if (bronze_coin == NULL) {
        bronze_coin = sdl_get_texture(BRONZE_COIN_PATH);
      }
      return bronze_coin;
    case SILVER_COIN:
      if (silver_coin == NULL) {
        silver_coin = sdl_get_texture(SILVER_COIN_PATH);
      }
      return silver_coin;
    case GOLD_COIN:
      if (gold_coin == NULL) {
        gold_coin = sdl_get_texture(GOLD_COIN_PATH);
      }
      return gold_coin;
    case JETPACK:
      if (jetpack == NULL) {
        jetpack = sdl_get_texture(STAR_PATH);
      }
      return jetpack;
    case SELECT_BTN:
      if (select_btn == NULL) {
        select_btn = sdl_get_texture(SELECT_BTN_PATH);
      }
      return select_btn;
    case SELECTED_BTN:
      if (selected_btn == NULL) {
        selected_btn = sdl_get_texture(SELECTED_BTN_PATH);
      }
      return selected_btn;
    case UNLOCK_10_BTN:
      if (unlock_10_btn == NULL) {
        unlock_10_btn = sdl_get_texture(UNLOCK_10_BTN_PATH);
      }
      return unlock_10_btn;
    case UNLOCK_25_BTN:
      if (unlock_25_btn == NULL) {
        unlock_25_btn = sdl_get_texture(UNLOCK_25_BTN_PATH);
      }
      return unlock_25_btn;
    case UNLOCK_50_BTN:
      if (unlock_50_btn == NULL) {
        unlock_50_btn = sdl_get_texture(UNLOCK_50_BTN_PATH);
      }
      return unlock_50_btn;
    case UNLOCK_100_BTN:
      if (unlock_100_btn == NULL) {
        unlock_100_btn = sdl_get_texture(UNLOCK_100_BTN_PATH);
      }
      return unlock_100_btn;
    case UNLOCK_1000_BTN:
      if (unlock_1000_btn == NULL) {
        unlock_1000_btn = sdl_get_texture(UNLOCK_1000_BTN_PATH);
      }
      return unlock_1000_btn;
    case BACKDROP:
      if (backdrop == NULL) {
        backdrop = sdl_get_texture(GAME_BACKDROP_PATH);
      }
      return backdrop;
    case GROUND_GRASS:
      if (ground == NULL) {
        ground = sdl_get_texture(GROUND_PATH);
      }
      return ground;
    default:
      return NULL;
  }
}

vector_t get_move_speed(body_t *player, scene_t *scene, bool reverse) {
  vector_t move_vel = body_get_velocity(player);
  move_vel.x = PLAYER_MOVE_SPEED;
  body_t *button = scene_get_body(scene, FIRST_BTN_IDX + MOVE_SPEED_BUTTON);
  // jump boost item selected
  if (get_subtype(button) == SELECTED_BTN) {
    move_vel.x += MOVE_BOOST;
  }
  if (reverse) {
    move_vel.x *= -1.0;
  }
  return move_vel;
}

void on_key(char key, key_event_type_t type, double held_time, vector_t position, state_t *state) {
  body_t *player = scene_get_body(state->scene, PLAYER_IDX);

  if (type == KEY_PRESSED) {
    switch (key) {
      case LEFT_ARROW:
        body_set_velocity(player, get_move_speed(player, state->scene, true));
        break;
      case RIGHT_ARROW:
        body_set_velocity(player, get_move_speed(player, state->scene, false));
        break;
      case MOUSE_CLICK:
        handle_click(position, state);
        // printf("%f \t %f \n", position.x, position.y);
        break;
      case M_KEY:
        emscripten_free(state);
        emscripten_force_exit(1);
        break;
      default:
        break;
    }
  } else if (type == KEY_RELEASED) {
    switch (key) {
      case LEFT_ARROW:
      case RIGHT_ARROW: {
        vector_t player_vel = body_get_velocity(player);
        player_vel.x = 0;
        body_set_velocity(player, player_vel);
        break;
      }
    }
  }
}

void add_shop_buttons(scene_t *scene) {
  for (size_t row = 0; row < SHOP_NUM_ROWS; row++) {
    double top_right_y = SHOP_TOP_RIGHT.y - row * (SHOP_VERT_SPACING + SHOP_BUTTON_HEIGHT);
    double bottom_left_y = top_right_y - SHOP_BUTTON_HEIGHT;

    for (size_t col = 0; col < SHOP_NUM_COLS; col++) {
      double top_right_x = SHOP_TOP_RIGHT.x - col * (SHOP_HORIZ_SPACING + SHOP_BUTTON_WIDTH);          
      double bottom_left_x = top_right_x - SHOP_BUTTON_WIDTH;
      vector_t btn_center = (vector_t) {.x = 0.5 * (top_right_x + bottom_left_x), 0.5 * (top_right_y + bottom_left_y)};
      list_t *no_shape = make_rectangle(1, 1, btn_center);
      
      body_subtype_t btn_state = UNLOCK_10_BTN;
      button_t btn_type = SHOP_NUM_ROWS * row + col;
      switch (btn_type) {
        case CS_MAJOR_BUTTON:
          btn_state = SELECTED_BTN;
          break;
        case BEM_MAJOR_BUTTON:
        case PHYSICS_MAJOR_BUTTON:
          btn_state = UNLOCK_100_BTN;
          break;
        case DEATH_SAVE_BUTTON:
        case JUMP_SPEED_BUTTON:
        case MOVE_SPEED_BUTTON:
          btn_state = UNLOCK_50_BTN;
          break;
        case MORE_RAM_BUTTON:
          btn_state = UNLOCK_1000_BTN;
          break;
        case COIN_BUTTON:
        case JETPACK_BUTTON:
          btn_state = UNLOCK_25_BTN;
          break;
        default:
          break;
      }

      body_t *button = body_init_with_info_texture(no_shape, INFINITY, BLACK,
                                        make_type_info(BUTTON), make_subtype_info(btn_state), free, 
                                        NULL, BTN_TXT_SCALE);
      scene_add_body(scene, button);
    }
  }
}

/** Creates an Earth-like mass to accelerate objects */
void add_gravity_body(scene_t *scene) {
  // Will be offscreen, so shape is irrelevant
  vector_t gravity_center = {.x = WINDOW.x / 2, .y = -EARTH_RADIUS};
  list_t *gravity_ball = make_rectangle(1, 1, gravity_center);
  body_t *body = body_init_with_info(gravity_ball, EARTH_MASS, BLACK,
                                     make_type_info(GRAVITY), free);

  scene_add_body(scene, body);
}

void load_dynamic_bodies(state_t *state){
  state->game_mode = MENU; // display start menu
  set_foreground(get_overlay_texture(MENU));
  state->altitude = 0;
  state->timer = -1;
  state->lives = 1;
  
  // create player in scene
  scene_t *scene = state->scene;
  add_player(scene);

  // initial platform
  vector_t platform_center = (vector_t){.x = WINDOW.x / 2, .y = INITIAL_PLATFORM_SPACING / 2};
  list_t *platform_shape = make_rectangle(PLATFORM_HEIGHT, PLATFORM_WIDTH, platform_center);
  body_t *platform = body_init_with_info_texture(platform_shape, INFINITY, BLUE, make_type_info(PLATFORM), 
                                                make_subtype_info(STANDARD_PLATFORM), free, 
                                                get_subtype_texture(STANDARD_PLATFORM), PLATFORM_TXT_SCALE);  
  create_collision(scene, platform, scene_get_body(scene, PLAYER_IDX), get_subtype_collision(STANDARD_PLATFORM), scene, NULL);
  scene_add_body(scene, platform);

  generate_platforms(0.15 * WINDOW.y, (BUFFER_COEFFICIENT) * WINDOW.y / INITIAL_PLATFORM_SPACING, state);
}

state_t *emscripten_init() {
  srand(time(NULL));
  TTF_Init(); // font backend

  vector_t min = (vector_t){.x = 0, .y = 0};
  vector_t max = WINDOW;
  sdl_init(min, max);
  sdl_on_key(on_key);
  Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, 2, 2048); // sound backend

  set_backdrop(get_subtype_texture(BACKDROP));

  // state_init
  state_t *state = malloc(sizeof(state_t));
  assert(state != NULL);

  SDL_Surface *cursor_surface = IMG_Load(STANDARD_CURSOR_PATH);
  SDL_Cursor *cursor = SDL_CreateColorCursor(cursor_surface,0,0);
  SDL_Surface *grabby_cursor_surface = IMG_Load(GRABBY_CURSOR_PATH);
  SDL_Cursor *grabby_cursor = SDL_CreateColorCursor(grabby_cursor_surface,0,0);
  SDL_SetCursor(cursor);
  SDL_FreeSurface(cursor_surface);
  SDL_FreeSurface(grabby_cursor_surface);
  state->standard_cursor = cursor;
  state->grabby_cursor = grabby_cursor;
  state->num_coins = 0;

  scene_t *scene = scene_init();
  state->scene = scene;
  add_gravity_body(scene); // create earth
  add_shop_buttons(scene); // add invisible button placeholders
  add_wealth_display(scene, state->num_coins); // add wealth display in scene
  
  load_dynamic_bodies(state); // bodies that change every round
  return state;
}

void ground_collision_handler(body_t *ground, body_t *player, vector_t axis, void *aux) {
  state_t *state = aux;
  state->game_mode = DEATH_MENU;
  set_foreground(get_overlay_texture(DEATH_MENU));
  body_set_info(player, make_type_info(DEAD_PLAYER));
  // cut off sound
  Mix_HaltChannel(-1);
}

void add_ground_grass(state_t *state) {
  // create platform
  list_t *ground_shape = make_rectangle(2 * PLAYER_HEIGHT, WINDOW.x, GROUND_CENTER);
  body_t *ground = body_init_with_info_texture(ground_shape, INFINITY, PLATFORM_COLOR,
                                        make_type_info(GROUND), NULL, free, 
                                        get_subtype_texture(GROUND_GRASS), GROUND_SCALE);

  // add collision handler
  body_t *player = scene_get_body(state->scene, PLAYER_IDX);
  create_collision(state->scene, ground, player, ground_collision_handler, state, NULL);

  scene_add_body(state->scene, ground);
}

void save_platform_collision_handler(body_t *platform, body_t *player, vector_t axis, void *aux) {
  state_t *state = aux;
  state->game_mode = PLAY;
  if (player_on_platform(platform, player, axis)) {
    vector_t player_vel = get_jump_speed(player, state->scene);
    player_vel.y *= SPRING_PLATFORM_ELASTICITY;
    body_set_velocity(player, player_vel);
    body_remove(platform);
    // remove ground if saved
    for (size_t i = 0; i < scene_bodies(state->scene); i++) {
      body_t *body = scene_get_body(state->scene, i);
      if (get_type(body) == GROUND) {
        body_remove(body);
      }
    }
  }
}

void check_game_over(state_t *state) {
  scene_t *scene = state->scene;
  body_t *player = scene_get_body(scene, PLAYER_IDX);
  vector_t player_center = body_get_centroid(player);

  // add death save platform
  if (get_type(player) != DEAD_PLAYER && player_center.y <= 0 ) {
    body_t *death_save_btn = scene_get_body(state->scene, FIRST_BTN_IDX + DEATH_SAVE_BUTTON);
    if (get_subtype(death_save_btn) == SELECTED_BTN) {
      body_set_velocity(player, VEC_ZERO);
      vector_t platform_center = (vector_t){.x = player_center.x, .y = player_center.y - SAVE_PLATFORM_BUFFER};
      list_t *platform_shape = make_rectangle(PLATFORM_HEIGHT, 1.75 * PLATFORM_WIDTH, platform_center);
      body_t *platform = body_init_with_info_texture(platform_shape, INFINITY, BLUE, make_type_info(GROUND), 
                                                    make_subtype_info(SAVE_PLATFORM), free, 
                                                    get_subtype_texture(SAVE_PLATFORM), PLATFORM_TXT_SCALE);  
      create_collision(scene, platform, player, save_platform_collision_handler, state, NULL);
      scene_add_body(scene, platform);
      add_ground_grass(state);
      // consume/reset item once used
      body_set_sub_info(death_save_btn, make_subtype_info(UNLOCK_50_BTN));
      state->game_mode = GAME_OVER;
      return;
    }
  }

  // player has been killed or fell off map
  if (get_type(player) == DEAD_PLAYER || player_center.y <= 0) {
    state->game_mode = GAME_OVER;

    //TODO: switch to dead texture
    switch (get_subtype(player)) {
      case (CS_MAJOR):
        body_set_sub_info(player, make_subtype_info(DEAD_CS_MAJOR));
        body_set_texture(player, get_subtype_texture(DEAD_CS_MAJOR));
        break;
      case (PHYSICS_MAJOR):
        body_set_sub_info(player, make_subtype_info(DEAD_PHYSICS_MAJOR));
        body_set_texture(player, get_subtype_texture(DEAD_PHYSICS_MAJOR));
        break;
      case (BEM_MAJOR):
        body_set_sub_info(player, make_subtype_info(DEAD_BEM_MAJOR));
        body_set_texture(player, get_subtype_texture(DEAD_BEM_MAJOR));
        break;
      default:
        break;
    }
    add_ground_grass(state);

    Mix_Chunk *scream = load_audio(SCREAM_AUDIO);
    Mix_PlayChannel(-1, scream, 0);
  }
}

void update_moving_platforms(scene_t *scene){
  for (size_t i = 0; i < scene_bodies(scene); i++) {
    body_t *body = scene_get_body(scene, i);

    if(get_type(body) == PLATFORM && get_subtype(body) == MOVING_PLATFORM) {
      list_t *body_shape = body_get_shape(body);
      size_t *wall_collide = is_touching_wall(body_shape, WINDOW);
      // if colliding with wall, reverse speed
      // absolute value is used instead of negation to prevent glitches
      if(wall_collide[R_IDX]){
        double curr_velocity_abs = fabs(body_get_velocity(body).x);
        body_set_velocity(body, (vector_t){.x = -curr_velocity_abs, .y = 0});
      } else if (wall_collide[L_IDX]){
        double curr_velocity_abs = fabs(body_get_velocity(body).x);
        body_set_velocity(body, (vector_t){.x = curr_velocity_abs, .y = 0});
      }
      list_free(body_shape);
      free(wall_collide);
    }
  }
}

void player_jetpack(state_t *state, double dt) {
  body_t *player = scene_get_body(state->scene, PLAYER_IDX);
  if (get_type(player) == JETPACK_PLAYER) {
    double player_x_velocity = body_get_velocity(player).x;
    body_set_velocity(player, (vector_t){.x = player_x_velocity, .y = PLAYER_FLIGHT_SPEED});
    if (state->timer >= 0) {
      state->timer += dt;
      if (state->timer > FLIGHT_TIME) {
        state->timer = -1;
      }
    }
    else {
      state->game_mode = PLAY;
      set_foreground(get_overlay_texture(PLAY));
      hide_jetpack(player);
    }
  }
}

void update_altitude(state_t *state, double dt) {
  vector_t player_vel = body_get_velocity(scene_get_body(state->scene, PLAYER_IDX));
  double altitude_change = player_vel.y * dt;
  state->altitude += altitude_change;
}

void emscripten_main(state_t *state) {
  double dt = time_since_last_tick();
  mouse_hover(state);

  switch (state->game_mode) {
    case PLAY: {
      scroll(state, dt);
      update_moving_platforms(state->scene);
      player_wrap_around(state->scene);
      update_altitude(state, dt);
      player_jetpack(state, dt);

      check_game_over(state);
      break;
    }
    case GAME_OVER: {
      scroll(state, dt);
      update_moving_platforms(state->scene);
      player_wrap_around(state->scene);
      break;
    }
    // Don't tick scene when paused
    case SHOP:
    case MENU:
    case DEATH_MENU: {
      sdl_render_scene_with_images(state->scene);
      return;
    }

    default:
      break;
  }

  scene_tick(state->scene, dt);
  sdl_render_scene_with_images(state->scene);
}

void sdl_free_assets() {
  // free body textures
  for (body_subtype_t type = CS_MAJOR; type < GROUND_GRASS; type++) {
    SDL_Texture *texture = get_subtype_texture(type);
    if (texture != NULL) {
      SDL_DestroyTexture(texture);
    }
  }
  // free overlay textures
  for (game_state_t mode = PLAY; mode < SHOP; mode++) {
    SDL_Texture *texture = get_overlay_texture(mode);
    if (texture != NULL) {
      SDL_DestroyTexture(texture);
    }
  }
  // free fonts
  for (body_subtype_t font = SCORE_TEXT; font < SCORE_TEXT; font++) {
    get_text_texture(font, NULL, (SDL_Color) COIN_FONT_COLOR);
  }
  // free audios
  for (sound_t type = BEM_AUDIO; type < SCREAM_AUDIO; type++) {
    Mix_Chunk *audio = load_audio(type);
    if (audio != NULL) {
      Mix_FreeChunk(audio);
    }
  }
}

void state_free(state_t *state) {
  scene_free(state->scene);
  SDL_FreeCursor(state->standard_cursor);
  SDL_FreeCursor(state->grabby_cursor);
  free(state);
}

void emscripten_free(state_t *state) {
  sdl_free_assets();
  state_free(state);
  sdl_free();
}