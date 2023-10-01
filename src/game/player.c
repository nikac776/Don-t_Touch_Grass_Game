#include "forces.h"
#include "scene.h"
#include "sdl_wrapper.h"
#include "shapes.h"
#include "state.h"

#include <assert.h>
#include <math.h>
#include <stdlib.h>

#include "game.h"

void change_major(scene_t *scene, body_subtype_t major) {
  body_t *player = scene_get_body(scene, PLAYER_IDX);
  body_set_sub_info(player, make_subtype_info(major));
  body_set_texture(player, get_subtype_texture(major));

  switch (major) {
    case CS_MAJOR:
    case BEM_MAJOR: {
      body_t *earth = scene_get_body(scene, EARTH_IDX);
      vector_t gravity_center = {.x = WINDOW.x / 2, .y = -EARTH_RADIUS};
      body_set_centroid(earth, gravity_center);
      break;
    }
    case PHYSICS_MAJOR: {
      body_t *earth = scene_get_body(scene, EARTH_IDX);
      vector_t gravity_center = {.x = WINDOW.x / 2, .y = -PHYSICS_MAJOR_GRAVITY * EARTH_RADIUS};
      body_set_centroid(earth, gravity_center);
      break;
    }
    default:
      break;
  }
}

void add_player(scene_t *scene) {
  list_t *player_shape = make_rectangle(PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_CENTER);  
  body_subtype_t major = CS_MAJOR;
  // check shop selection
  if (get_subtype(scene_get_body(scene, FIRST_BTN_IDX)) == SELECTED_BTN) {
    major = BEM_MAJOR;
  }
  else if (get_subtype(scene_get_body(scene, FIRST_BTN_IDX + 1)) == SELECTED_BTN) {
    major = PHYSICS_MAJOR;
  }

  body_t *player = body_init_with_info_texture(player_shape, PLAYER_MASS, PLAYER_COLOR,
                                       make_type_info(PLAYER), make_subtype_info(major), free, 
                                       get_subtype_texture(major), PLAYER_TXT_SCALE);

  size_t body_count = scene_bodies(scene);
  scene_add_body(scene, player);
  
  // Add force creators with other bodies
  for (size_t i = 0; i < body_count; i++) {
    body_t *body = scene_get_body(scene, i);
    switch (get_type(body)) {
      case GRAVITY:
        // Simulate earth's gravity acting on the ball
        create_newtonian_gravity(scene, G_CONST, body, player);
        break;
      default:
        break;
    }
  }
}

// player wraps around screen
void player_wrap_around(scene_t *scene) {
  body_t *player = scene_get_body(scene, PLAYER_IDX);
  vector_t centroid = body_get_centroid(player);
  if (centroid.x <= 0) {
    centroid.x = WINDOW.x;
  }
  else if (centroid.x >= WINDOW.x) {
    centroid.x = 0;
  }
  body_set_centroid(player, centroid);
}