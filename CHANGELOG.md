# Changelog

## Develop

## v1.5.0

- Fix the platformio library package description
- Fix `LWUTIL_COMPILE_TIME_ASSERT` referencing an undefined macro, causing a compile error whenever it was used
- Fix undefined behavior (signed left-shift overflow) in `lwutil_ld_u16/u32_le/be` and their `_ex` variants
- Fix `lwutil_ld_u32_varint` not bounding the number of parsed bytes, causing undefined behavior on malformed input
- Fix incorrect minimum buffer size documented for `lwutil_ld_u32_le` and `lwutil_ld_u32_le_ex`
- Add warning notes to `LWUTIL_MIN`/`MAX`/`CONSTRAIN`/`MAP`/`ABS` docs about multiple evaluation of macro arguments

## 1.4.0

- Rework library CMake with removed INTERFACE type
- Add extended loading/storing option

## 1.3.0

- Add `LWUTIL_SET_VALUE_IF_PTR_NOT_NULL`
- Add `LWUTIL_MAP` and `LWUTIL_CONSTRAIN` functions
- Add `LWUTIL_ASZ`, a shortcut from `LWUTIL_ARRAYSIZE`

## 1.2.0

- Split CMakeLists.txt files between library and executable
- Change license year to 2022
- Update code style with astyle
- Add `.clang-format` draft

## v1.0.0

- Initial release
- Support for platform.io