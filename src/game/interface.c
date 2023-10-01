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

void add_wealth_display(scene_t *scene, int32_t num_coins) {
  char str[128] = "";
  snprintf(str, sizeof(str), "%d", num_coins);
  SDL_Texture *new_text = get_text_texture(SCORE_TEXT, str, (SDL_Color) COIN_FONT_COLOR);
  vector_t display_center = WEALTH_DISPLAY_CENTER;
  if (10 <= num_coins && num_coins < 100) {
    display_center.x += 0.25 * SCORE_FONT_SIZE;
  }
  else if (100 <= num_coins && num_coins < 1000) {
    display_center.x += 0.5 * SCORE_FONT_SIZE;
  }
  else if (1000 <= num_coins && num_coins < 10000) {
    display_center.x += 0.75 * SCORE_FONT_SIZE;
  }

  list_t *display_shape = make_rectangle(1, 1, display_center);
  body_t *display = body_init_with_info_texture(display_shape, PLAYER_MASS, PLATFORM_COLOR,
                                      make_type_info(TEXT), make_subtype_info(SCORE_TEXT), free, new_text, 1.0);
  scene_add_body(scene, display);
}

Mix_Chunk *load_audio(sound_t audio_type) {
  static Mix_Chunk *cs_audio = NULL;
  static Mix_Chunk *bem_audio = NULL;
  static Mix_Chunk *physics_audio = NULL;
  static Mix_Chunk *scream_audio = NULL;
  switch (audio_type) {
    case CS_AUDIO: {
      if (cs_audio == NULL) {
        cs_audio = Mix_LoadWAV(CS_AUDIO_PATH);
      }
      return cs_audio;
    }
    case BEM_AUDIO: {
      if (bem_audio == NULL) {
        bem_audio = Mix_LoadWAV(BEM_AUDIO_PATH);
      }
      return bem_audio;
    }
    case PHYSICS_AUDIO: {
      if (physics_audio == NULL) {
        physics_audio = Mix_LoadWAV(PHYSICS_AUDIO_PATH);
      }
      return physics_audio;
    }
    case SCREAM_AUDIO: {
      if (scream_audio == NULL) {
        scream_audio = Mix_LoadWAV(SCREAM_PATH);
      }
      return scream_audio;
    }
    default:
      return NULL;
  }
}

void select_shop_item(state_t *state, button_t shop_item) {
  body_t *button = scene_get_body(state->scene, FIRST_BTN_IDX + shop_item);
  body_subtype_t btn_state = get_subtype(button);
  
  // purchase item
  size_t prev_loot = state->num_coins;
  switch (btn_state) {
    case UNLOCK_10_BTN:
      state->num_coins -= 10;
      break;
    case UNLOCK_25_BTN:
      state->num_coins -= 25;
      break;
    case UNLOCK_50_BTN:
      state->num_coins -= 50;
      break;
    case UNLOCK_100_BTN:
      state->num_coins -= 100;
      break;
    case UNLOCK_1000_BTN:
      state->num_coins -= 1000;
      break;
    default:
      break;
  }

  // quit if too expensive
  if (state->num_coins < 0) {
    state->num_coins = prev_loot;
    return;
  }
  update_wealth(state, 0);

  if (shop_item < 3 && btn_state != SELECTED_BTN) {
    // deselect current player
    for (size_t i = 0; i < SHOP_NUM_COLS; i++) {
      body_t *btn = scene_get_body(state->scene, FIRST_BTN_IDX + i);
      if (get_subtype(btn) == SELECTED_BTN) {
        body_set_sub_info(btn, make_subtype_info(SELECT_BTN));
        body_set_texture(btn, get_subtype_texture(SELECT_BTN));
        break;
      }
    }

    Mix_Chunk *player_audio = NULL;
    switch (shop_item) {
      case BEM_MAJOR_BUTTON: {
        player_audio = load_audio(BEM_AUDIO);
        change_major(state->scene, BEM_MAJOR);
        break;
      }
      case PHYSICS_MAJOR_BUTTON: {
        player_audio = load_audio(PHYSICS_AUDIO);
        change_major(state->scene, PHYSICS_MAJOR);
        break;
      }
      case CS_MAJOR_BUTTON: {
        player_audio = load_audio(CS_AUDIO);
        change_major(state->scene, CS_MAJOR);
        break;
      }
      default:
        break;
    }
    Mix_PlayChannel(-1, player_audio, 0);
  }
  
  // toggle off item
  if (shop_item >= 3 && btn_state == SELECTED_BTN) {
    body_set_sub_info(button, make_subtype_info(SELECT_BTN));
    body_set_texture(button, get_subtype_texture(SELECT_BTN));
  }
  else {
    body_set_sub_info(button, make_subtype_info(SELECTED_BTN));
    body_set_texture(button, get_subtype_texture(SELECTED_BTN));
  }
}

void show_shop_buttons(scene_t *scene) {
  for (size_t i = 0; i < SHOP_NUM_ROWS * SHOP_NUM_COLS; i++) {
    body_t *button = scene_get_body(scene, FIRST_BTN_IDX + i);
    body_subtype_t btn_state = get_subtype(button);
    body_set_texture(button, get_subtype_texture(btn_state));
  }
}

void hide_shop_buttons(scene_t *scene) {
  for (size_t i = 0; i < SHOP_NUM_ROWS * SHOP_NUM_COLS; i++) {
    body_t *button = scene_get_body(scene, FIRST_BTN_IDX + i);
    body_set_texture(button, NULL);
  }
}

button_t get_button(vector_t mouse_coords, state_t *state) {
  bool menu_button_clicked = fabs(vec_magnitude(vec_subtract(mouse_coords, MENU_BUTTON_CENTER))) < MENU_BUTTON_RADIUS;

  vector_t play_lower_bound = vec_subtract(mouse_coords, PLAY_RECT_BOTTOM_LEFT);
  vector_t play_upper_bound = vec_subtract(mouse_coords, PLAY_RECT_TOP_RIGHT);
  bool play_button_clicked = play_lower_bound.x > 0 && play_lower_bound.y > 0 && play_upper_bound.x < 0 && play_upper_bound.y < 0;
  
  vector_t shop_lower_bound = vec_subtract(mouse_coords, SHOP_RECT_BOTTOM_LEFT);
  vector_t shop_upper_bound = vec_subtract(mouse_coords, SHOP_RECT_TOP_RIGHT);
  bool shop_button_clicked = shop_lower_bound.x > 0 && shop_lower_bound.y > 0 && shop_upper_bound.x < 0 && shop_upper_bound.y < 0;

  vector_t back_lower_bound = vec_subtract(mouse_coords, BACK_BOTTOM_LEFT);
  vector_t back_upper_bound = vec_subtract(mouse_coords, BACK_TOP_RIGHT);
  bool back_button_clicked = back_lower_bound.x > 0 && back_lower_bound.y > 0 && back_upper_bound.x < 0 && back_upper_bound.y < 0;

  switch (state->game_mode) {
    case PLAY: {
      if (menu_button_clicked) {
        return MENU_BUTTON;
      }
      break;
    }
    case MENU:
    case DEATH_MENU: {
      if (play_button_clicked) {
        return PLAY_BUTTON;
      }
      else if (shop_button_clicked) {
        return SHOP_BUTTON;
      }
      break;
    }
    case SHOP: {
      if (back_button_clicked) {
        return BACK_BUTTON;
      }
      for (size_t row = 0; row < SHOP_NUM_ROWS; row++) {
        double top_right_y = SHOP_TOP_RIGHT.y - row * (SHOP_VERT_SPACING + SHOP_BUTTON_HEIGHT);
        double bottom_left_y = top_right_y - SHOP_BUTTON_HEIGHT;

        for (size_t col = 0; col < SHOP_NUM_COLS; col++) {
          button_t cur_index = SHOP_NUM_ROWS * row + col;
          double top_right_x = SHOP_TOP_RIGHT.x - col * (SHOP_HORIZ_SPACING + SHOP_BUTTON_WIDTH);          
          double bottom_left_x = top_right_x - SHOP_BUTTON_WIDTH;
          vector_t bottom_left = (vector_t) {.x = bottom_left_x, .y = bottom_left_y};
          vector_t top_right = (vector_t) {.x = top_right_x, .y = top_right_y};
          vector_t lower_bound = vec_subtract(mouse_coords, bottom_left);
          vector_t upper_bound = vec_subtract(mouse_coords, top_right);
          bool button_clicked = lower_bound.x > 0 && lower_bound.y > 0 && upper_bound.x < 0 && upper_bound.y < 0;
          if (button_clicked) {
            return cur_index;
          }
        }
      }
      break;
    }
    default:
      break;
  }
  return NO_BUTTON;
}

void handle_click(vector_t mouse_coords, state_t *state) {
  button_t button = get_button(mouse_coords, state);
  switch (state->game_mode) {
    case PLAY: {
      if (button == MENU_BUTTON) {
        state->game_mode = MENU;
        set_foreground(get_overlay_texture(MENU));
      }
      break;
    }
    case MENU: {
      if (button == PLAY_BUTTON) {
        set_foreground(NULL);
        state->game_mode = PLAY;
        set_foreground(get_overlay_texture(PLAY));
      }
      else if (button == SHOP_BUTTON) {
        state->game_mode = SHOP;
        set_foreground(get_overlay_texture(SHOP));
        show_shop_buttons(state->scene);
      }
      break;
    }
    case DEATH_MENU: {
      if (button == PLAY_BUTTON) {
        // remove all dynamic bodies from scene
        for (size_t i = PLAYER_IDX; i < scene_bodies(state->scene); i++) {
          body_remove(scene_get_body(state->scene, i));
        }
        scene_tick(state->scene, 0);

        load_dynamic_bodies(state); // maybe go straight to play?
      }
      else if (button == SHOP_BUTTON) {
        state->game_mode = SHOP;
        set_foreground(get_overlay_texture(SHOP));
        show_shop_buttons(state->scene);
      }
      break;
    }
    case SHOP: {
      if (button == BACK_BUTTON) {
        body_t *player = scene_get_body(state->scene, PLAYER_IDX);
        if (get_type(player) == DEAD_PLAYER) {
          state->game_mode = DEATH_MENU;
          set_foreground(get_overlay_texture(DEATH_MENU));
          hide_shop_buttons(state->scene);
        }
        else {
          state->game_mode = MENU;
          set_foreground(get_overlay_texture(MENU));
          hide_shop_buttons(state->scene);
        }
      }
      else if (button != NO_BUTTON) {
        select_shop_item(state, button);
        // printf("%d \n", button);
      }
      break;
    }
    default:
      break;
  }
}

void mouse_hover(state_t *state) {
  int32_t x, y;
  SDL_GetGlobalMouseState(&x, &y);
  vector_t position = (vector_t) {.x = x, .y = y};
  position = scale_mouse_coords(position);
  SDL_Cursor *cursor = SDL_GetCursor();
  if (get_button(position, state) == NO_BUTTON) {
    if (cursor == state->grabby_cursor) {
      SDL_SetCursor(state->standard_cursor);
    }
  }
  else {
    if (cursor == state->standard_cursor) {
      SDL_SetCursor(state->grabby_cursor);
    }
  }
}