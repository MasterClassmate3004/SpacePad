// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        KC_W,   KC_A, KC_S,
        KC_D,   KC_UP, KC_DOWN
    )
};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [0] = {
        ENCODER_CCW_CW(KC_VOLD, KC_VOLU),
        ENCODER_CCW_CW(MS_WHLU, MS_WHLD)
    }
};
#endif

// The encoder push switches are wired directly to ground rather than into
// the 2x3 diode matrix. They are handled here with the XIAO's pull-ups.
#define ENCODER_BUTTON_DEBOUNCE 20

static bool     left_encoder_pressed;
static bool     right_encoder_pressed;
static uint16_t left_encoder_changed;
static uint16_t right_encoder_changed;

static void scan_encoder_button(pin_t pin, bool *state, uint16_t *changed, uint16_t keycode) {
    bool pressed = !gpio_read_pin(pin);

    if (pressed != *state && timer_elapsed(*changed) >= ENCODER_BUTTON_DEBOUNCE) {
        *state   = pressed;
        *changed = timer_read();

        if (pressed) {
            tap_code(keycode);
        }
    }
}

void keyboard_post_init_user(void) {
    gpio_set_pin_input_high(GP1); // XIAO D7: left encoder push
    gpio_set_pin_input_high(GP3); // XIAO D10: right encoder push

    left_encoder_changed  = timer_read();
    right_encoder_changed = timer_read();
}

void matrix_scan_user(void) {
    scan_encoder_button(GP1, &left_encoder_pressed, &left_encoder_changed, KC_MUTE);
    scan_encoder_button(GP3, &right_encoder_pressed, &right_encoder_changed, KC_MPLY);
}
