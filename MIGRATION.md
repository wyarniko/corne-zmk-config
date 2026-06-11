# Jorne to Eyelash Corne migration

This repository keeps the original `eyelash_corne` hardware support and ports
the six-layer Jorne keymap from `wyarniko/zmk-config`.

## Position mapping

The vendor transform has 48 logical positions, but only 42 of them are the
normal Corne keys. The other six center positions are not used by this keymap.

- The normal alpha positions and all six thumb keys retain their Jorne actions.
- The dedicated `LWIN` key is removed; `LGUI` remains available on `Q` and on
  the left thumb.
- The dedicated `RBKT` key is replaced by the `P + LBKT` combo. It produces
  `RBKT`, which is `ъ` with the standard Russian host layout.
- Pressing the outermost left and right thumb keys together toggles RGB
  underglow.
- All six center positions use `&none`.
- Both encoder bindings are retained from the vendor configuration.

All Jorne combos were translated to the Eyelash Corne matrix positions. The
vendor `Q+S+Z` soft-off combo is also retained.

## Firmware artifacts

GitHub Actions builds:

- `eyelash_corne_right` with `nice_view`
- `eyelash_corne_studio_left` with `nice_view` and ZMK Studio
- `settings_reset`

Flash the right and left firmware to their matching halves. Use
`settings_reset` only when Bluetooth settings need to be cleared, then flash the
normal firmware again.
