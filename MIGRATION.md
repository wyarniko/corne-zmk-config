# Jorne to Eyelash Corne migration

This repository keeps the original `eyelash_corne` hardware support and ports
the six-layer Jorne keymap from `wyarniko/zmk-config`.

## Position mapping

The vendor transform has 48 logical positions, but only 42 of them are the
normal Corne keys. The other six center positions are not used by this keymap.

- The normal alpha positions and all six thumb keys retain their Jorne actions.
- The dedicated `LWIN` key is removed; `LGUI` remains available on `Q` and on
  the left thumb.
- `N + M` produces `PIPE` (`|` on an English host layout).
- `M + Comma` produces `LBKT` (`[` on an English host layout).
- `Comma + Dot` produces `RBKT` (`]` on an English host layout and `ъ` on the
  standard Russian host layout).
- Pressing the outermost left and right thumb keys together toggles RGB
  underglow.
- While holding the `Esc` thumb key, use `Q/W` for hue, `E/R` for brightness,
  and `T` for the next RGB effect.
- The five-way switch on the right half controls the mouse pointer; pressing it
  sends a left click.
- The remaining center position uses `&none`.
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
