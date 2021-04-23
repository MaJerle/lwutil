/**
 * \file            lwutil.c
 * \brief           Lightweight utility library
 */

/*
 * Copyright (c) 2020 Tilen MAJERLE
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * This file is part of LwUTIL - Lightweight utility library.
 *
 * Author:          Tilen MAJERLE <tilen@majerle.eu>
 * Version:         $_version_$
 */
#include <string.h>
#include "lwutil.h"

/**
 * \brief           Store `16-bit` value to bytes array in little-endian format
 * \param[in]       val: Value to write to output array
 * \param[out]      ptr: Minimum `2-bytes` long output array to write value to
 */
void
lwutil_st_u16_le(uint16_t val, void* ptr) {
    uint8_t* p = ptr;

    p[0] = (uint8_t)((val >> 0) & 0xFF);
    p[1] = (uint8_t)((val >> 8) & 0xFF);
}

/**
 * \brief           Store `32-bit` value to bytes array in little-endian format
 * \param[in]       val: Value to write to output array
 * \param[out]      ptr: Minimum `4-bytes` long output array to write value to
 */
void
lwutil_st_u32_le(uint32_t val, void* ptr) {
    uint8_t* p = ptr;

    p[0] = (uint8_t)((val >> 0) & 0xFF);
    p[1] = (uint8_t)((val >> 8) & 0xFF);
    p[2] = (uint8_t)((val >> 16) & 0xFF);
    p[3] = (uint8_t)((val >> 24) & 0xFF);
}

/**
 * \brief           Load `16-bit` value from bytes array in little-endian format
 * \param[in]       ptr: Minimum `2-bytes` long input array to extract bytes from
 * \return          `16-bit` value extracted from input array
 */
uint16_t
lwutil_ld_u16_le(const void* ptr) {
    const uint8_t* p = ptr;
    return p[1] << 8 | p[0];
}

/**
 * \brief           Load `32-bit` value from bytes array in little-endian format
 * \param[in]       ptr: Minimum `2-bytes` long input array to extract bytes from
 * \return          `32-bit` value extracted from input array
 */
uint32_t
lwutil_ld_u32_le(const void* ptr) {
    const uint8_t* p = ptr;
    return p[3] << 24 | p[2] << 16 | p[1] << 8 | p[0];
}

/**
 * \brief           Store `16-bit` value to bytes array in big-endian format
 * \param[in]       val: Value to write to output array
 * \param[out]      ptr: Minimum `2-bytes` long output array to write value to
 */
void
lwutil_st_u16_be(uint16_t val, void* ptr) {
    uint8_t* p = ptr;

    p[0] = (uint8_t)((val >> 8) & 0xFF);
    p[1] = (uint8_t)((val >> 0) & 0xFF);
}

/**
 * \brief           Store `32-bit` value to bytes array in big-endian format
 * \param[in]       val: Value to write to output array
 * \param[out]      ptr: Minimum `4-bytes` long output array to write value to
 */
void
lwutil_st_u32_be(uint32_t val, void* ptr) {
    uint8_t* p = ptr;

    p[0] = (uint8_t)((val >> 24) & 0xFF);
    p[1] = (uint8_t)((val >> 16) & 0xFF);
    p[2] = (uint8_t)((val >> 8) & 0xFF);
    p[3] = (uint8_t)((val >> 0) & 0xFF);
}

/**
 * \brief           Load `16-bit` value from bytes array in big-endian format
 * \param[in]       ptr: Minimum `2-bytes` long input array to extract bytes from
 * \return          `16-bit` value extracted from input array
 */
uint16_t
lwutil_ld_u16_be(const void* ptr) {
    const uint8_t* p = ptr;
    return p[0] << 8 | p[1];
}

/**
 * \brief           Load `32-bit` value from bytes array in big-endian format
 * \param[in]       ptr: Minimum `4-bytes` long input array to extract bytes from
 * \return          `32-bit` value extracted from input array
 */
uint32_t
lwutil_ld_u32_be(const void* ptr) {
    const uint8_t* p = ptr;
    return p[0] << 24 | p[1] << 16 | p[2] << 8 | p[3];
}

/**
 * \brief           Makes ascii char array from `unsigned 8-bit` value
 * \param[in]       hex: Hexadecimal data to be converted
 * \param[out]      ascii: Minimum `3-bytes` long array to write value to
 */
void
lwutil_u8_to_2asciis(uint8_t hex, char* ascii) {
    for (uint8_t i = 2; i != 0; --i) {
        uint8_t aux = (hex >> (4 * (i - 1))) & 0x0F;

        aux = (aux <= 9) ? (aux + 0x30) : (aux + 0x57);
        ascii[2 - i] = aux;
    }
    ascii[2] = '\0';
}

/**
 * \brief           Makes ascii char array from `unsigned 16-bit` value
 * \param[in]       hex: Hexadecimal data to be converted
 * \param[out]      ascii: Minimum `5-bytes` long array to write value to
 */
void
lwutil_u16_to_4asciis(uint16_t hex, char* ascii) {
    for (uint8_t i = 4; i != 0; --i) {
        uint8_t aux = (hex >> (4 * (i - 1))) & 0x0F;

        aux = (aux <= 9) ? (aux + 0x30) : (aux + 0x57);
        ascii[4 - i] = aux;
    }
    ascii[4] = '\0';
}

/**
 * \brief           Makes ascii char array from `unsigned 32-bit` value
 * \param[in]       hex: Hexadecimal data to be converted
 * \param[out]      ascii: Minimum `9-bytes` long array to write value to
 */
void
lwutil_u32_to_8asciis(uint32_t hex, char* ascii) {
    for (uint8_t i = 8; i != 0; --i) {
        uint8_t aux = (hex >> (4 * (i - 1))) & 0x0F;

        aux = (aux <= 9) ? (aux + 0x30) : (aux + 0x57);
        ascii[8 - i] = aux;
    }
    ascii[8] = '\0';
}

/**
 * \brief           Check if bit is set in a '8-bit' number
 * \param[in]       num: Number in which the bit needs to be checked
 * \param[in]       pos: position of the bit which needs to be checked
  * \return         returns 1 if bit is set, 0 if bit is cleared
 */

uint8_t lwutil_u8_check_bit_sts(const uint8_t num, const uint8_t pos)
{
	if(num & (1<< pos))
		return(1);
	else
		return(0);
}

/**
 * \brief           Check if bit is set in a '16-bit' number
 * \param[in]       num: Number in which the bit needs to be checked
 * \param[in]       pos: position of the bit which needs to be checked
  * \return         returns 1 if bit is set, 0 if bit is cleared
 */

uint8_t lwutil_u16_check_bit_sts(const uint16_t num, const uint8_t pos)
{
	if(num & (1<< pos))
		return(1);
	else
		return(0);
}

/**
 * \brief           Check if bit is set in a '32-bit' number
 * \param[in]       num: Number in which the bit needs to be checked
 * \param[in]       pos: position of the bit which needs to be checked
  * \return         returns 1 if bit is set, 0 if bit is cleared
 */

uint8_t lwutil_u32_check_bit_sts(const uint32_t num, const uint8_t pos)
{
	if(num & (1<< pos))
		return(1);
	else
		return(0);
}

/**
 * \brief           Get the upper nibble of an '8-bit' number
 * \param[in]       num: Number from which upper nibble is to be extracted
 * \return         returns upper nibble data, lower nibble will be set to 0 
 * \example			if num = 0x83, return = 0x80 
 */

uint8_t lwutil_u8_get_upper_nibble(const uint8_t num)
{
	return(num & (0xF0));
}

/**
 * \brief           Get the lower nibble of an '8-bit' number
 * \param[in]       num: Number from which lower nibble is to be extracted
 * \return         returns lower nibble data, upper nibble will be set to 0 
 * \example			if num = 0x83, return = 0x03 
 */

uint8_t lwutil_u8_get_lower_nibble(const uint8_t num)
{
	return(num & (0x0F));
}

/**
 * \brief           Get the upper byte of an '16-bit' number
 * \param[in]       num: Number from which upper byte is to be extracted
 * \return          returns upper byte data
 * \example			if num = 0x8123, return = 0x81 
 */

uint8_t lwutil_u16_get_upper_byte(const uint16_t num)
{
	return((num & (0xFF00)) >> 8);
}

/**
 * \brief           Get the lower byte of an '16-bit' number
 * \param[in]       num: Number from which lower byte is to be extracted
 * \return          returns lower byte data, upper byte will be set to 0 
 * \example			if num = 0x8123, return = 0x23 
 */

uint8_t lwutil_u16_get_lower_byte(const uint16_t num)
{
	return(num & (0xFF));
}

/**
 * \brief           Get the upper word of an '32-bit' number
 * \param[in]       num: Number from which upper word is to be extracted
 * \return          returns upper word data
 * \example			if num = 0x81234567, return = 0x8123 
 */

uint16_t lwutil_u32_get_upper_word(const uint32_t num)
{
	return((num & (0xFFFF0000)) >> 16);
}

/**
 * \brief           Get the lower byte of an '16-bit' number
 * \param[in]       num: Number from which lower byte is to be extracted
 * \return          returns lower byte data, upper byte will be set to 0 
 * \example			iif num = 0x81234567, return = 0x4567 
 */

uint16_t lwutil_u16_get_lower_word(const uint32_t num)
{
	return(num & (0x0000FFFF));
}

