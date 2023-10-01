#include "forces.h"
#include "scene.h"
#include "sdl_wrapper.h"
#include "shapes.h"
#include "state.h"

#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "game.h"

// this function can be modified to be more complicated, to take in player progress as variable etc.
bool random_create_death_platform(double death_prob){
  double rand = rand_double(0.0, (1.0/death_prob));
  return (rand <= 1.0);
}

// this function can be modified to be more complicated, to take in player progress as variable etc.
body_subtype_t random_platform_subtype(){
  double rand = rand_double(0,1);

  if(rand < MOVING_PLATFORM_PROB){
    return MOVING_PLATFORM;
  }
  else if (rand < MOVING_PLATFORM_PROB + BREAKABLE_PLATFORM_PROB){
    return BREAKABLE_PLATFORM;
  }
  else if (rand < MOVING_PLATFORM_PROB + BREAKABLE_PLATFORM_PROB + SPRING_PLATFORM_PROB) {
    return SPRING_PLATFORM;
  }
  return STANDARD_PLATFORM;
}

bool player_on_platform(body_t *platform, body_t *player, vector_t axis) {
  vector_t player_vel = body_get_velocity(player);

  if (player_vel.y < 0 && get_type(player) != DEAD_PLAYER) {
    double platform_y = body_get_centroid(platform).y;
    double player_y = body_get_centroid(player).y;

    double tolerance = 0.6 * PLATFORM_HEIGHT + 0.3 * PLAYER_HEIGHT;
    return axis.y > COLLISION_UP_THRESHOLD && player_y - platform_y > tolerance;
  }
  return false;
}

vector_t get_jump_speed(body_t *player, scene_t *scene) {
  vector_t jump_vel = body_get_velocity(player);
  jump_vel.y = PLAYER_JUMP_SPEED;
  body_t *button = scene_get_body(scene, FIRST_BTN_IDX + JUMP_SPEED_BUTTON);
  // jump boost item selected
  if (get_subtype(button) == SELECTED_BTN) {
    jump_vel.y += JUMP_BOOST;
  }
  return jump_vel;
}

void platform_collision_handler(body_t *platform, body_t *player, vector_t axis, void *aux) {
  if (player_on_platform(platform, player, axis)) {
    body_set_velocity(player, get_jump_speed(player, aux));
  }
}

void breakable_platform_collision_handler(body_t *platform, body_t *player, vector_t axis, void *aux) {
  if (player_on_platform(platform, player, axis)) {
    body_set_velocity(player, get_jump_speed(player, aux));
    body_remove(platform);
  }
}

void spring_platform_collision_handler(body_t *platform, body_t *player, vector_t axis, void *aux) {
  if (player_on_platform(platform, player, axis)) {
    vector_t player_vel = get_jump_speed(player, aux);
    player_vel.y *= SPRING_PLATFORM_ELASTICITY;
    body_set_velocity(player, player_vel);
  }
}

void death_platform_collision_handler(body_t *platform, body_t *player, vector_t axis, void *aux) {
  if (player_on_platform(platform, player, axis)) {
    body_set_info(player, make_type_info(DEAD_PLAYER));
  }
}

collision_handler_t get_subtype_collision(body_subtype_t subtype){
  switch(subtype){
    case STANDARD_PLATFORM:
      return platform_collision_handler;
    case MOVING_PLATFORM:
      return platform_collision_handler;
    case GRASS_PLATFORM:
      return death_platform_collision_handler;
    case BREAKABLE_PLATFORM:
      return breakable_platform_collision_handler;
    case SPRING_PLATFORM:
      return spring_platform_collision_handler;
    default:
      return NULL;
  }
}

void update_wealth(state_t *state, int32_t loot) {
  scene_t *scene = state->scene;
  body_t *wealth_display = scene_get_body(scene, WEALTH_DISPLAY_IDX);

  state->num_coins += loot;
  // Shift text based on number of digits
  vector_t display_center = WEALTH_DISPLAY_CENTER;
  if (10 <= state->num_coins && state->num_coins < 100) {
    display_center.x += 0.25 * SCORE_FONT_SIZE;
  }
  else if (100 <= state->num_coins && state->num_coins < 1000) {
    display_center.x += 0.5 * SCORE_FONT_SIZE;
  }
  else if (1000 <= state->num_coins && state->num_coins < 10000) {
    display_center.x += 0.75 * SCORE_FONT_SIZE;
  }
  body_set_centroid(wealth_display, display_center);

  char str[128] = "";
  snprintf(str, sizeof(str), "%d", state->num_coins);
  body_subtype_t text_type = get_subtype(wealth_display);
  SDL_Texture *new_text = get_text_texture(text_type, str, (SDL_Color) COIN_FONT_COLOR);

  SDL_DestroyTexture(body_get_texture(wealth_display));
  body_set_texture(wealth_display, new_text);
}

void coin_collision_handler(body_t *coin, body_t *player, vector_t axis, void *aux) {
  state_t *state = aux;
  body_remove(coin);
  size_t multiplier = 1;
  if (get_subtype(player) == BEM_MAJOR || get_subtype(player) == JETPACK_BEM_MAJOR) {
    multiplier = BEM_COIN_MULTIPLIER;
  }
  switch (get_subtype(coin)) {
    case (BRONZE_COIN):
      update_wealth(state, multiplier * BRONZE_VALUE);
      break;
    case (SILVER_COIN):
      update_wealth(state, multiplier * SILVER_VALUE);
      break;
    case (GOLD_COIN):
      update_wealth(state, multiplier * GOLD_VALUE);
      break;
    default:
      break;
  }
}

void add_coin(body_t *platform, state_t *state, body_subtype_t denomination) {
  scene_t *scene = state->scene;

  vector_t platform_center = body_get_centroid(platform);
  vector_t coin_center = (vector_t) {.x = platform_center.x, .y = platform_center.y + COIN_HEIGHT};
  list_t *coin_shape = make_circle(COIN_RADIUS, coin_center);
  // can later support coin subtypes
  body_t *coin = body_init_with_info_texture(coin_shape, INFINITY, COIN_COLOR, make_type_info(COIN), 
                                             make_subtype_info(denomination), free, get_subtype_texture(denomination), COIN_TXT_SCALE);
  // add collision handler
  body_t *player = scene_get_body(scene, PLAYER_IDX);
  create_collision(scene, coin, player, coin_collision_handler, state, NULL);
  scene_add_body(scene, coin);
}

void show_jetpack(body_t *player) {
  body_set_info(player, make_type_info(JETPACK_PLAYER));

  switch (get_subtype(player)) {
    case (CS_MAJOR):
      body_set_sub_info(player, make_subtype_info(JETPACK_CS_MAJOR));
      body_set_texture(player, get_subtype_texture(JETPACK_CS_MAJOR));
      break;
    case (PHYSICS_MAJOR):
      body_set_sub_info(player, make_subtype_info(JETPACK_PHYSICS_MAJOR));
      body_set_texture(player, get_subtype_texture(JETPACK_PHYSICS_MAJOR));
      break;
    case (BEM_MAJOR):
      body_set_sub_info(player, make_subtype_info(JETPACK_BEM_MAJOR));
      body_set_texture(player, get_subtype_texture(JETPACK_BEM_MAJOR));
      break;
    default:
      break;
  }
}

void hide_jetpack(body_t *player) {
  body_set_info(player, make_type_info(PLAYER));

  switch (get_subtype(player)) {
    case (JETPACK_CS_MAJOR):
      body_set_sub_info(player, make_subtype_info(CS_MAJOR));
      body_set_texture(player, get_subtype_texture(CS_MAJOR));
      break;
    case (JETPACK_PHYSICS_MAJOR):
      body_set_sub_info(player, make_subtype_info(PHYSICS_MAJOR));
      body_set_texture(player, get_subtype_texture(PHYSICS_MAJOR));
      break;
    case (JETPACK_BEM_MAJOR):
      body_set_sub_info(player, make_subtype_info(BEM_MAJOR));
      body_set_texture(player, get_subtype_texture(BEM_MAJOR));
      break;
    default:
      break;
  }
}

void star_collision_handler(body_t *star, body_t *player, vector_t axis, void *aux) {
  state_t *state = aux;
  if (get_type(player) != JETPACK_PLAYER) {
    body_remove(star);
    show_jetpack(player);
    if (state->timer <= 0) {
      state->timer = 0;
    }
  }
}

void add_star(body_t *platform, state_t *state) {
  scene_t *scene = state->scene;

  vector_t platform_center = body_get_centroid(platform);
  vector_t star_center = (vector_t) {.x = platform_center.x, .y = platform_center.y + STAR_HEIGHT};
  list_t *star_shape = make_circle(STAR_RADIUS, star_center);
  // can later support coin subtypes
  body_t *star = body_init_with_info_texture(star_shape, INFINITY, STAR_COLOR, make_type_info(STAR), 
                                             make_subtype_info(JETPACK), free, get_subtype_texture(JETPACK), COIN_TXT_SCALE);
  // add collision handler
  body_t *player = scene_get_body(scene, PLAYER_IDX);
  create_collision(scene, star, player, star_collision_handler, state, NULL);
  scene_add_body(scene, star);
}

// can be made more complex later
vector_t platform_velocity() {
  double velocity = rand_double(MOVING_PLATFORM_VEL_MIN, MOVING_PLATFORM_VEL_MAX);
  double coin = rand_double(0.0, 1.0);
  if(coin > 0.5){
    velocity *= -1.0;
  }
  return (vector_t){.x = velocity, .y = 0};
}

void add_collectable(body_t *platform, state_t *state) {
  double random = rand_double(0,1);

  body_t *coin_btn = scene_get_body(state->scene, FIRST_BTN_IDX + COIN_BUTTON);
  if (get_subtype(coin_btn) == SELECTED_BTN) {
    if (random <= BRONZE_SPAWN_RATE) {
      add_coin(platform, state, BRONZE_COIN);
    }
    else if (random <= BRONZE_SPAWN_RATE + SILVER_SPAWN_RATE) {
      add_coin(platform, state, SILVER_COIN);
    }
    else if (random <= COIN_SPAWN_RATE) {
      add_coin(platform, state, GOLD_COIN);
    }
  }
  else if (random <= COIN_SPAWN_RATE) {
    add_coin(platform, state, BRONZE_COIN);
  }

  body_t *jetpack_btn = scene_get_body(state->scene, FIRST_BTN_IDX + JETPACK_BUTTON);
  if (get_subtype(jetpack_btn) == SELECTED_BTN) {
    if (COIN_SPAWN_RATE < random && random <= COIN_SPAWN_RATE + STAR_SPAWN_RATE) {
      add_star(platform, state);
    }
  }
}

double get_platform_spacing(double distance_traveled) {
  // inverse ReLU!
  double distance_fraction = distance_traveled / (20 * WINDOW.y);
  double delta_spacing = fmin(distance_fraction * PLATFORM_SPACING_CHANGE, PLATFORM_SPACING_CHANGE);
  return INITIAL_PLATFORM_SPACING + delta_spacing;
}

// start_height not inclusive
void generate_platforms(int start_height, size_t num_platforms, state_t *state) {
  double platform_spacing = get_platform_spacing(state->altitude);
  scene_t *scene = state->scene;
  body_t *player = scene_get_body(scene, PLAYER_IDX);

  for (size_t row = 0; row < num_platforms; row++) {
    int32_t x_min = PLATFORM_WIDTH / 2;
    int32_t x_max = WINDOW.x - PLATFORM_WIDTH / 2;
    int32_t platform_x = x_min + rand_double(0, x_max - x_min);
    int32_t platform_y = start_height + (row + 1) * platform_spacing;
    // add randomness to platform_y
    int32_t wiggle_room = (platform_spacing - PLATFORM_HEIGHT) / WIGGLE_ROOM_FACTOR;
    platform_y += (int32_t) rand_double(-1.0 * wiggle_room, wiggle_room);

    // choose subtype of platform (standard, breakable, or moving)
    body_subtype_t platform_subtype = random_platform_subtype();

    // create useable platform 
    vector_t platform_center = (vector_t){.x = platform_x, .y = platform_y};
    list_t *platform_shape = make_rectangle(PLATFORM_HEIGHT, PLATFORM_WIDTH, platform_center);
    body_t *platform = body_init_with_info_texture(platform_shape, INFINITY, PLATFORM_COLOR, make_type_info(PLATFORM), 
                                                   make_subtype_info(platform_subtype), free, get_subtype_texture(platform_subtype), 
                                                   PLATFORM_TXT_SCALE);

    // add movement to moving platform
    if(platform_subtype == MOVING_PLATFORM){
      body_set_velocity(platform, platform_velocity());
    }

    add_collectable(platform, state);

    // add collision handler
    create_collision(scene, platform, player, get_subtype_collision(platform_subtype), scene, NULL);
    scene_add_body(scene, platform);

    // create grass platform (instant game over) given some probability
    if(random_create_death_platform(DEATH_PLATFORM_PROB)){
      int32_t death_platform_x = x_min + rand_double(0, x_max - x_min);
      int32_t death_platform_y = platform_y + 0.5 * platform_spacing;
      // add randomness to death_platform_y
      death_platform_y += rand_double(-1.0 * wiggle_room, wiggle_room);

      // create death_platform
      vector_t death_platform_center = (vector_t){.x = death_platform_x, .y = death_platform_y};
      list_t *death_platform_shape = make_rectangle(PLATFORM_HEIGHT, PLATFORM_WIDTH, death_platform_center);
      body_t *death_platform = body_init_with_info_texture(death_platform_shape, INFINITY, PLATFORM_COLOR, make_type_info(PLATFORM), 
                                                    make_subtype_info(GRASS_PLATFORM), free, get_subtype_texture(GRASS_PLATFORM), 
                                                    PLATFORM_TXT_SCALE);
      
      // add death collision handler
      create_collision(scene, death_platform, player, get_subtype_collision(GRASS_PLATFORM), scene, NULL);

      scene_add_body(scene, death_platform);
    }
  }
}

void scroll(state_t *state, double dt) {
  scene_t *scene = state->scene;
  game_state_t game_mode = state->game_mode;

  body_t *player_body = scene_get_body(scene, PLAYER_IDX);
  vector_t player_center = body_get_centroid(player_body);
  double player_y_velocity = body_get_velocity(player_body).y;

  // ensure player is never in top half of screen
  if ((player_center.y - CENTER.y > 0 && player_y_velocity > 0) || game_mode == GAME_OVER) {
    vector_t translation = (vector_t){.x = 0, .y = (player_y_velocity + player_center.y - CENTER.y) * dt};
    vector_t new_centroid = vec_subtract(player_center, translation);
    body_set_centroid(player_body, new_centroid);

    size_t num_removed = 0;
    double max_platform_y = 0;
    for (size_t i = 0; i < scene_bodies(scene); i++) {
      body_t *body = scene_get_body(scene, i);
      switch (get_type(body)) {
        case PLATFORM: {
          vector_t new_centroid = vec_subtract(body_get_centroid(body), translation);
          body_set_centroid(body, new_centroid);
          max_platform_y = fmax(max_platform_y, new_centroid.y);
          if (new_centroid.y + PLATFORM_HEIGHT / 2 < 0) {
            body_remove(body);
            if (get_subtype(body) != GRASS_PLATFORM) {
              num_removed++;
            }
          }
          break;
        }
        case COIN: 
        case STAR: {
          vector_t new_centroid = vec_subtract(body_get_centroid(body), translation);
          body_set_centroid(body, new_centroid);
          int radius = get_type(body) == COIN ? COIN_RADIUS : STAR_RADIUS; 
          if (new_centroid.y + radius < 0) {
            body_remove(body);
          }
          break;
        }
        case GROUND: {
          vector_t new_centroid = vec_subtract(body_get_centroid(body), translation);
          body_set_centroid(body, new_centroid);
          break;
        }
        default:
          break;
      }
    }
    if (num_removed > 0) {
      generate_platforms(max_platform_y, num_removed, state);
    }
  }
}
