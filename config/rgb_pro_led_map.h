/*
 * Per-key LED map for zmk-pro-rgb on the Eyelash Corne.
 *
 * Each half has its own 21-LED chain, one LED per key. Tables are indexed by
 * this half's key index, in keymap order:
 *
 *    0  1  2  3  4  5
 *    6  7  8  9 10 11
 *   12 13 14 15 16 17
 *            18 19 20
 *
 * Keymap positions are mapped to these indices in src/rgb_pro_eyelash.c.
 *
 * key_to_led is a first guess: the column-wise serpentine chain of the
 * Corne v3 per-key LEDs, mirrored on the right half. Fix it here if the
 * wrong keys light up.
 */

#pragma once

#include <zephyr/kernel.h>

#if defined(CONFIG_ZMK_SPLIT_ROLE_CENTRAL)

const uint8_t key_to_led[] = {
    18, 17, 12, 11,  4,  3,
    19, 16, 13, 10,  5,  2,
    20, 15, 14,  9,  6,  1,
                 8,  7,  0,
};

const uint8_t key_col[] = {
    0, 1, 2, 3, 4, 5,
    0, 1, 2, 3, 4, 5,
    0, 1, 2, 3, 4, 5,
             3, 4, 5,
};

#else

const uint8_t key_to_led[] = {
     3,  4, 11, 12, 17, 18,
     2,  5, 10, 13, 16, 19,
     1,  6,  9, 14, 15, 20,
     0,  7,  8,
};

/* Columns continue after the left half, skipping column 6 for the
 * five-way switch, so sweeping effects run across both halves. */
const uint8_t key_col[] = {
    7, 8, 9, 10, 11, 12,
    7, 8, 9, 10, 11, 12,
    7, 8, 9, 10, 11, 12,
    7, 8, 9,
};

#endif

const uint8_t key_row[] = {
    0, 0, 0, 0, 0, 0,
    1, 1, 1, 1, 1, 1,
    2, 2, 2, 2, 2, 2,
             3, 3, 3,
};
