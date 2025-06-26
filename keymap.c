/*
Copyright 2019 @foostan
Copyright 2020 Drashna Jaelre <@drashna>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H
#include <stdio.h>

// Timer to track the last keypress for idle time detection (in milliseconds)
static uint32_t last_keypress_timer = 0;

// Timer to trigger the OLED clear action periodically when idle (in milliseconds)
static uint32_t oled_clear_timer = 0;

// Defines the idle time threshold in milliseconds
#define OLED_IDLE_TIMEOUT 3000

// Defines the interval for clearing the screen after the timeout is met
#define OLED_CLEAR_INTERVAL 3000

// Defines the row on the OLED from which to start clearing.
// (0-indexed, so 2 means the 3rd row)
#define OLED_CLEAR_START_ROW 3

#define BASE 0
#define NAVIGATION 1
#define MOUSE 2
#define BUTTON 3
#define MEDIA 4
#define NUMERIC 5
#define SYMBOL 6
#define FUNCTION 7
#define ADDITIONAL_FEATURES 8

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case MT(MOD_RSFT,KC_BSPC):
            return TAPPING_TERM - 40;
        default:
            return TAPPING_TERM;
    }
}

bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LT(SYMBOL,KC_TAB):
        case LT(NUMERIC,KC_ESC):
        case MT(MOD_RSFT,KC_BSPC):
            // Immediately select the hold action when another key is tapped.
            return true;
        default:
            // Do not select the hold action when another key is tapped.
            return false;
    }
}

// bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
//     switch (keycode) {
//         case MT(KC_RSFT,KC_BSPC):
//             // Immediately select the hold action when another key is pressed.
//             return true;
//         default:
//             // Do not select the hold action when another key is pressed.
//             return false;
//     }
// }

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {  
  [BASE] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
    MO(MEDIA),    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                         KC_Y,    KC_U,    KC_I,    KC_O,   KC_P,  KC_BSPC,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
  KC_LSFT,MT(MOD_LGUI,KC_A),MT(MOD_LALT,KC_S),MT(MOD_LCTL,KC_D),KC_F,KC_G,          KC_H,KC_J,MT(MOD_RCTL,KC_K),MT(MOD_RALT,KC_L),MT(MOD_RGUI,KC_SCLN),KC_QUOT,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
   MO(BUTTON),    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                         KC_N,    KC_M, KC_COMM,  KC_DOT,KC_SLASH,MO(BUTTON),
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
     LT(NUMERIC,KC_ESC),  LT(NAVIGATION,KC_SPC),  LT(SYMBOL,KC_TAB),    LT(MOUSE,KC_ENT), MT(MOD_RSFT,KC_BSPC), LT(FUNCTION,KC_DEL)
                                      //`--------------------------'  `--------------------------'
  ),

  [NAVIGATION] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX,   KC_UP, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_MCTL, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, CW_TOGG,                      KC_CAPS, KC_LEFT, KC_DOWN, KC_RGHT, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                       KC_INS, KC_HOME, KC_PGDN, KC_PGUP,  KC_END, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          XXXXXXX, _______, XXXXXXX,     KC_ENT, KC_BSPC,  KC_DEL
                                      //`--------------------------'  `--------------------------'
  ),

  [MOUSE] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      XXXXXXX, XXXXXXX, XXXXXXX,   MS_UP, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, MS_LEFT, MS_DOWN, MS_RGHT, XXXXXXX,                      XXXXXXX, KC_RSFT, KC_RCTL, KC_RALT, KC_RGUI, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX, MS_WHLL, MS_WHLD, MS_WHLU, MS_WHLR, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          MS_BTN3, MS_BTN1, MS_BTN2,    _______, XXXXXXX, XXXXXXX
                                      //`--------------------------'  `--------------------------'
  ),

  [BUTTON] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      XXXXXXX, KC_UNDO,  KC_CUT, KC_COPY,KC_PASTE,KC_AGAIN,                     KC_AGAIN,KC_PASTE, KC_COPY,  KC_CUT, KC_UNDO, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX,                      XXXXXXX, KC_RSFT, KC_RCTL, KC_RALT, KC_RGUI, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX, KC_UNDO,  KC_CUT, KC_COPY,KC_PASTE,KC_AGAIN,                     KC_AGAIN,KC_PASTE, KC_COPY,  KC_CUT, KC_UNDO, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          MS_BTN3, MS_BTN1, MS_BTN2,    MS_BTN2, MS_BTN1, MS_BTN3
                                      //`--------------------------'  `--------------------------'
  ),

  [MEDIA] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      RM_TOGG, RM_NEXT, RM_HUEU, RM_SATU, RM_VALU, RM_SPDU,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LSFT, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX,                      KC_MUTE, KC_MPRV, KC_VOLD, KC_VOLU, KC_MNXT, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          XXXXXXX, XXXXXXX, XXXXXXX,    KC_MSTP, KC_MPLY, XXXXXXX
                                      //`--------------------------'  `--------------------------'
  ),

  [NUMERIC] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      KC_LBRC,    KC_7,    KC_8,    KC_9, KC_RBRC, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      QK_LLCK, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX,                      KC_SCLN,    KC_4,    KC_5,    KC_6,  KC_EQL, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                       KC_GRV,    KC_1,    KC_2,    KC_3, KC_NUHS, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          _______, XXXXXXX, XXXXXXX,    KC_MINS,    KC_0,  KC_DOT
                                      //`--------------------------'  `--------------------------'
  ),

  [SYMBOL] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      KC_LCBR, KC_AMPR, KC_ASTR, KC_LPRN, KC_RCBR, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX,                      KC_COLN,  KC_DLR, KC_PERC, KC_CIRC, KC_PLUS, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      KC_TILD, KC_EXLM,   KC_AT, KC_HASH, KC_PIPE, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          XXXXXXX, XXXXXXX, _______,    KC_UNDS, KC_LPRN, KC_RPRN
                                      //`--------------------------'  `--------------------------'
  ),

  [FUNCTION] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      XXXXXXX,  KC_F12,   KC_F7,   KC_F8,   KC_F9, KC_PSCR,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, MO(ADDITIONAL_FEATURES),
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX,  KC_F11,   KC_F4,   KC_F5,   KC_F6, KC_SCRL,                      XXXXXXX, KC_RSFT, KC_RCTL, KC_RALT, KC_RGUI, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX,  KC_F10,   KC_F1,   KC_F2,   KC_F3, KC_PAUS,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                           KC_APP,  KC_SPC,  KC_TAB,    XXXXXXX, XXXXXXX, _______
                                      //`--------------------------'  `--------------------------'
  ),

  [ADDITIONAL_FEATURES] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      QK_BOOT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, _______
                                      //`--------------------------'  `--------------------------'
  )
};

#ifdef OLED_ENABLE
    oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    if (!is_keyboard_master()) {
        return OLED_ROTATION_180;  // flips the display 180 degrees if offhand
    } else {
        return OLED_ROTATION_270;
    }
    return rotation;
    }

    void oled_render_layer_state(void) {
        oled_set_cursor(0, 0);
        oled_write_P(PSTR("Layer"), false);
        oled_set_cursor(0, 2);
        switch (get_highest_layer(layer_state)) {
            case BASE:
                oled_write_ln_P(PSTR("BASE"), false);
                break;
            case NAVIGATION:
                oled_write_ln_P(PSTR("NAV"), false);
                break;
            case MOUSE:
                oled_write_ln_P(PSTR("MOUSE"), false);
                break;
            case BUTTON:
                oled_write_ln_P(PSTR("BTN"), false);
                break;
            case MEDIA:
                oled_write_ln_P(PSTR("MEDIA"), false);
                break;
            case NUMERIC:
                oled_write_ln_P(PSTR("NUM"), false);
                break;
            case SYMBOL:
                oled_write_ln_P(PSTR("SYMB"), false);
                break;
            case FUNCTION:
                oled_write_ln_P(PSTR("FUNC"), false);
                break;
            case ADDITIONAL_FEATURES:
                oled_write_ln_P(PSTR("ADDTN"), false);
                break;
        }
    }

    // A custom helper function to map the RGB Matrix mode ID to a readable string.
    // PSTR() is used to store strings in PROGMEM (flash memory) to save RAM.
    const char* get_rgb_matrix_mode_name(uint8_t mode_id) {
        switch (mode_id) {
            case RGB_MATRIX_NONE:                   return PSTR("OFF");
            case RGB_MATRIX_SOLID_COLOR:            return PSTR("SOLID");
            case RGB_MATRIX_ALPHAS_MODS:            return PSTR("ALPHAS MODS");
            case RGB_MATRIX_GRADIENT_UP_DOWN:       return PSTR("GRD UD");
            case RGB_MATRIX_GRADIENT_LEFT_RIGHT:    return PSTR("GRD LR");
            case RGB_MATRIX_BREATHING:              return PSTR("BREATHING");
            case RGB_MATRIX_BAND_SAT:               return PSTR("BAND SAT");
            case RGB_MATRIX_BAND_VAL:               return PSTR("BAND VAL");
            case RGB_MATRIX_BAND_PINWHEEL_SAT:      return PSTR("PWHEEL SAT");
            case RGB_MATRIX_BAND_PINWHEEL_VAL:      return PSTR("PWHEEL VAL");
            case RGB_MATRIX_BAND_SPIRAL_SAT:        return PSTR("SPIRAL SAT");
            case RGB_MATRIX_BAND_SPIRAL_VAL:        return PSTR("SPIRAL VAL");
            case RGB_MATRIX_CYCLE_ALL:              return PSTR("CYCLE ALL");
            case RGB_MATRIX_CYCLE_LEFT_RIGHT:       return PSTR("LR CYCLE");
            case RGB_MATRIX_CYCLE_UP_DOWN:          return PSTR("UD CYCLE");
            case RGB_MATRIX_CYCLE_OUT_IN:           return PSTR("IO CYCLE");
            case RGB_MATRIX_CYCLE_OUT_IN_DUAL:      return PSTR("IO DUAL");
            case RGB_MATRIX_RAINBOW_MOVING_CHEVRON: return PSTR("CHEVRON");
            case RGB_MATRIX_CYCLE_PINWHEEL:         return PSTR("PINWHEEL");
            case RGB_MATRIX_CYCLE_SPIRAL:           return PSTR("SPIRAL");
            case RGB_MATRIX_DUAL_BEACON:            return PSTR("DUAL BEACON");
            case RGB_MATRIX_RAINBOW_BEACON:         return PSTR("RNBW BEACON");
            case RGB_MATRIX_RAINBOW_PINWHEELS:      return PSTR("RNBW PWHEEL");
            case RGB_MATRIX_FLOWER_BLOOMING:        return PSTR("FLOWER");
            case RGB_MATRIX_RAINDROPS:              return PSTR("RAINDROPS");
            case RGB_MATRIX_JELLYBEAN_RAINDROPS:    return PSTR("J-DROPS");
            case RGB_MATRIX_HUE_BREATHING:          return PSTR("HUE BR");
            case RGB_MATRIX_HUE_PENDULUM:           return PSTR("HUE PEND");
            case RGB_MATRIX_HUE_WAVE:               return PSTR("HUE WAVE");
            case RGB_MATRIX_PIXEL_FRACTAL:          return PSTR("FRACTAL");
            case RGB_MATRIX_PIXEL_FLOW:             return PSTR("PIXEL FLOW");
            case RGB_MATRIX_PIXEL_RAIN:             return PSTR("PIXEL RAIN");
            case RGB_MATRIX_TYPING_HEATMAP:         return PSTR("HEATMAP");
            case RGB_MATRIX_DIGITAL_RAIN:           return PSTR("DIGI RAIN");
            case RGB_MATRIX_SOLID_REACTIVE_SIMPLE:  return PSTR("REAC SIMP");
            case RGB_MATRIX_SOLID_REACTIVE:         return PSTR("REACTIVE");
            case RGB_MATRIX_SOLID_REACTIVE_WIDE:    return PSTR("REAC WIDE");
            case RGB_MATRIX_SOLID_REACTIVE_MULTIWIDE: return PSTR("REAC MULTI");
            case RGB_MATRIX_SOLID_REACTIVE_CROSS:   return PSTR("REAC CROSS");
            case RGB_MATRIX_SOLID_REACTIVE_MULTICROSS: return PSTR("REAC MCROSS");
            case RGB_MATRIX_SOLID_REACTIVE_NEXUS:   return PSTR("REAC NEXUS");
            case RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS: return PSTR("REAC MNEXUS");
            case RGB_MATRIX_SPLASH:                 return PSTR("SPLASH");
            case RGB_MATRIX_MULTISPLASH:            return PSTR("M-SPLASH");
            case RGB_MATRIX_SOLID_SPLASH:           return PSTR("S-SPLASH");
            case RGB_MATRIX_SOLID_MULTISPLASH:      return PSTR("SM-SPLASH");
            case RGB_MATRIX_STARLIGHT:              return PSTR("STARLIGHT");
            case RGB_MATRIX_STARLIGHT_SMOOTH:       return PSTR("STAR SMTH");
            case RGB_MATRIX_STARLIGHT_DUAL_HUE:     return PSTR("STAR HUE");
            case RGB_MATRIX_STARLIGHT_DUAL_SAT:     return PSTR("STAR SAT");
            case RGB_MATRIX_RIVERFLOW:              return PSTR("RIVERFLOW");
            
            default:                                return PSTR("N/A");
        }
    }

    char keylog_str[24] = {};
    char rgb_effect[24] = {};

    void set_keylog(uint16_t keycode) {
        const char *key_name = get_keycode_string(keycode);
        
        // Update keylog_str
        // Use %s to print the string name.
        snprintf(keylog_str, sizeof(keylog_str), "%s            ", key_name);
    }

    void set_rgb_effect_string(void) {
        // --- Get and display the RGB Matrix Effect Name (Mode) ---
        // 1. Get the current mode ID (integer)
        uint8_t current_mode = rgb_matrix_get_mode();

        // 2. Use the helper function to get the name string
        const char *mode_name_P = get_rgb_matrix_mode_name(current_mode);

        // Update keylog_str
        // Use %s to print the string name.
        snprintf(rgb_effect, sizeof(rgb_effect), "%s     ", mode_name_P);
    }

    void oled_render_keylog(void) {
        oled_set_cursor(0, 5);
        oled_write_ln_P(keylog_str, false);

        // 3. Write the name to the OLED
        oled_set_cursor(0, 12);
        oled_write_ln_P(rgb_effect, false);
    }

    void render_bootmagic_status(bool status) {
        /* Show Ctrl-Gui Swap options */
        static const char PROGMEM logo[][2][3] = {
            {{0x97, 0x98, 0}, {0xb7, 0xb8, 0}},
            {{0x95, 0x96, 0}, {0xb5, 0xb6, 0}},
        };
        if (status) {
            oled_write_ln_P(logo[0][0], false);
            oled_write_ln_P(logo[0][1], false);
        } else {
            oled_write_ln_P(logo[1][0], false);
            oled_write_ln_P(logo[1][1], false);
        }
    }

    void oled_render_logo(void) {
        static const unsigned char PROGMEM wheat_logo [] = {
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x80, 0x80,
            0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0,
            0xC0, 0x80, 0xC0, 0xE0, 0xF0, 0xF0,
            0x70, 0x70, 0xF0, 0xF0, 0xE0, 0xE0,
            0xE0, 0xF0, 0xF8, 0xF8, 0xF8, 0x78,
            0x78, 0x70, 0x70, 0xF0, 0xE0, 0xE0,
            0xF0, 0xF0, 0xF0, 0xF0, 0x70, 0x70,
            0xF0, 0xF0, 0xE0, 0xE0, 0xC0, 0xE0,
            0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xC0,
            0xC0, 0x80, 0x00, 0x00, 0x80, 0x80,
            0x80, 0x80, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x80, 0xC0, 0xC0, 0xE0, 0xE0, 0xE0,
            0xE7, 0xEF, 0xFF, 0xFF, 0xF9, 0xF1,
            0xF1, 0xE3, 0xE3, 0xE3, 0xE7, 0xEF,
            0xEF, 0xFF, 0xFF, 0xFE, 0xFC, 0xF8,
            0xF8, 0xF0, 0xF1, 0xF3, 0xF7, 0xFF,
            0xFF, 0xFF, 0xFE, 0xFC, 0xF8, 0xF8,
            0xF0, 0x71, 0x73, 0xFF, 0xFF, 0xFF,
            0xFE, 0xFC, 0xF8, 0xF0, 0xF1, 0xE1,
            0xE3, 0xEF, 0xFF, 0xFF, 0xFE, 0xF9,
            0xF1, 0xE1, 0xC3, 0xC7, 0x8F, 0xBF,
            0xFF, 0xFF, 0xF7, 0xC7, 0x87, 0x0F,
            0x1E, 0x3E, 0xFC, 0xF8, 0xE0, 0x80,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x1E, 0x1F, 0x3F, 0x3F, 0x3B, 0x39,
            0x39, 0x38, 0x38, 0x3C, 0x1C, 0xFE,
            0xFF, 0xFF, 0x9F, 0x8F, 0xC7, 0xC3,
            0xE1, 0xF1, 0xF9, 0xFE, 0xFF, 0xEF,
            0xE7, 0xE3, 0xF1, 0xF0, 0x78, 0x7C,
            0x7E, 0xFF, 0xFF, 0xF7, 0xF3, 0x71,
            0x70, 0x78, 0x78, 0x7C, 0xFE, 0xFF,
            0xEF, 0xE7, 0xE3, 0xF3, 0xF1, 0x71,
            0xF8, 0xF8, 0xFC, 0xFF, 0xCF, 0xCF,
            0xC7, 0xC7, 0xE3, 0xE3, 0xE3, 0xF3,
            0x7B, 0x7F, 0x3F, 0x1F, 0x1F, 0x1F,
            0x1F, 0x9F, 0x9E, 0xDE, 0xFC, 0xFF,
            0x7F, 0x3F, 0x78, 0x70, 0xF0, 0xE0,
            0xE0, 0xC0, 0xC0, 0x80, 0x80, 0x80,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x03, 0x03, 0x03, 0x03, 0x03,
            0x03, 0x01, 0x01, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x01, 0x01, 0x01,
            0x01, 0x01, 0x01, 0x01, 0x01, 0x03,
            0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
            0x07, 0x07, 0x07, 0x07, 0x03, 0x03,
            0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x01, 0x01, 0x03, 0x03,
            0x07, 0x07, 0x0F, 0x0E, 0x1E, 0x1C,
            0x3C, 0x38, 0x38, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00,
        };

        oled_write_raw_P((const char *)wheat_logo, sizeof(wheat_logo));
    }

    bool oled_task_user(void) {
        if (is_keyboard_master()) {
            oled_render_layer_state();
            set_rgb_effect_string();
            // Check if the keyboard has been idle for longer than the timeout
            if (timer_elapsed32(last_keypress_timer) > OLED_IDLE_TIMEOUT) {
                // Check if it's time to clear the screen again (every 5 seconds of idle time)
                if (timer_elapsed32(oled_clear_timer) > OLED_CLEAR_INTERVAL) {
                    oled_set_cursor(0, OLED_CLEAR_START_ROW);

                    // Clear the rest of the screen by writing empty characters (spaces)
                    // Adjust the number of spaces to fill the remaining lines on your OLED.
                    // A common 128x32 OLED has about 21 characters per line.
                    for (int i = OLED_CLEAR_START_ROW; i < 16; i++) {
                        oled_set_cursor(0, i);
                        oled_write_P(PSTR("     "), false); // 5 spaces
                    }

                    // Reset the clear timer to trigger again after the interval
                    oled_clear_timer = timer_read32();
                }
            } else {
                oled_render_keylog();
            }
        } else {
            oled_render_logo();
        }
        return false;
    }

    bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        set_keylog(keycode);
        // Reset the last keypress timer on every key press
        last_keypress_timer = timer_read32();
        // Also reset the clear timer, so the screen doesn't clear immediately
        oled_clear_timer = timer_read32();
    }
    return true;
    }
#endif // OLED_ENABLE