/*
 * Eyelash Corne glue for zmk-pro-rgb.
 *
 * - Maps keymap positions to this half's LEDs. The module's own listener
 *   assumes each half's positions are contiguous, which is not true for this
 *   48-position layout, so it is disabled via CONFIG_RGB_PRO_POSITION_OFFSET.
 * - Switches the LED power rail (EXT_POWER) off while RGB is off or the
 *   keyboard is idle, like stock underglow did. The module only knows about
 *   a regulator node, not ZMK ext-power.
 * - Sets the startup effect and hue, which the module has no Kconfig for.
 */

#include <zephyr/device.h>
#include <zephyr/init.h>
#include <zephyr/kernel.h>

#include <drivers/ext_power.h>
#include <zmk/activity.h>
#include <zmk/event_manager.h>
#include <zmk/events/activity_state_changed.h>
#include <zmk/events/position_state_changed.h>

#include <zmk_vfx_pro_rgb/effects.h>

#define NONE -1

/* Keymap position -> key index in config/rgb_pro_led_map.h. Positions of the
 * other half, the five-way switch and the extra center key map to NONE. */
#if defined(CONFIG_ZMK_SPLIT_ROLE_CENTRAL)
static const int8_t pos_to_key[] = {
     0,  1,  2,  3,  4,  5, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
     6,  7,  8,  9, 10, 11, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
    12, 13, 14, 15, 16, 17, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
    18, 19, 20, NONE, NONE, NONE,
};
#else
static const int8_t pos_to_key[] = {
    NONE, NONE, NONE, NONE, NONE, NONE, NONE,  0,  1,  2,  3,  4,  5,
    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,  6,  7,  8,  9, 10, 11,
    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, 12, 13, 14, 15, 16, 17,
    NONE, NONE, NONE, 18, 19, 20,
};
#endif

BUILD_ASSERT(ARRAY_SIZE(pos_to_key) == 48, "Keymap has 48 positions");

static int key_listener(const zmk_event_t *eh) {
    const struct zmk_position_state_changed *ev = as_zmk_position_state_changed(eh);
    if (ev == NULL || !ev->state || ev->position >= ARRAY_SIZE(pos_to_key)) {
        return ZMK_EV_EVENT_BUBBLE;
    }

    int8_t key = pos_to_key[ev->position];
    if (key == NONE) {
        return ZMK_EV_EVENT_BUBBLE;
    }

    uint8_t led = key_to_led[key];
    reactive[led] = 255;
    rgbp_reactive_note_press(led);
    return ZMK_EV_EVENT_BUBBLE;
}

ZMK_LISTENER(rgb_pro_eyelash_keys, key_listener);
ZMK_SUBSCRIPTION(rgb_pro_eyelash_keys, zmk_position_state_changed);

#if DT_HAS_COMPAT_STATUS_OKAY(zmk_ext_power_generic)

static const struct device *ext_power = DEVICE_DT_GET(DT_INST(0, zmk_ext_power_generic));

static void power_sync(struct k_work *work);
static K_WORK_DELAYABLE_DEFINE(power_work, power_sync);

/* Toggling RGB can arrive from the other half without a local key event, so
 * poll state.on while active; stop polling when idle. */
static void power_sync(struct k_work *work) {
    bool active = zmk_activity_get_state() == ZMK_ACTIVITY_ACTIVE;
    bool want = state.on && active;

    if (device_is_ready(ext_power) && (ext_power_get(ext_power) > 0) != want) {
        want ? ext_power_enable(ext_power) : ext_power_disable(ext_power);
    }

    if (active) {
        k_work_reschedule(&power_work, K_MSEC(250));
    }
}

static int activity_listener(const zmk_event_t *eh) {
    k_work_reschedule(&power_work, K_NO_WAIT);
    return ZMK_EV_EVENT_BUBBLE;
}

ZMK_LISTENER(rgb_pro_eyelash_activity, activity_listener);
ZMK_SUBSCRIPTION(rgb_pro_eyelash_activity, zmk_activity_state_changed);

#endif

/* Runs right after the module's init (CONFIG_APPLICATION_INIT_PRIORITY = 90);
 * SYS_INIT needs a literal priority. */
static int rgb_pro_eyelash_init(void) {
    state.effect = RGB_PRO_EFF_SOLID_REACTIVE_SIMPLE;
    state.hue = 160;
#if DT_HAS_COMPAT_STATUS_OKAY(zmk_ext_power_generic)
    k_work_reschedule(&power_work, K_NO_WAIT);
#endif
    return 0;
}

SYS_INIT(rgb_pro_eyelash_init, APPLICATION, 91);
