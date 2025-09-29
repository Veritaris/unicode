//
// Created by Георгий Имешкенов on 22.04.2024.
//

#ifndef GENGINE_UNICODE_CONSTS_H
#define GENGINE_UNICODE_CONSTS_H

#include "unicode.h"

#define NEW_USTR_DEFAULT_LEN 16
#define NEW_USTR_NULL_VALUE -1

// define max int code for (i+1) octet Unicode char representation
const uint32_t MAX_UNICODE_CHAR[4] = {
    0x7F,
    0x7FF,
    0xFFFF,
    0x10FFFF
};

/**
 * Octets headers values
 */

const uint8_t START_ONE_OCTET = 0b00000000;
const uint8_t START_TWO_OCTET = 0b11000000;
const uint8_t START_THREE_OCTET = 0b11100000;
const uint8_t START_FOUR_OCTET = 0b11110000;
const uint8_t CONTINUE_OCTET = 0b10000000;

/**
 * Masks for Unicode characters start bytes
 * Mask itself leaves only significant for octet-defining bits
 * 0XXX XXXX
 */
const uint8_t ONE_OCTET_MASK = 0b10000000;
const uint8_t ONE_OCTET = 0b00000000;
// 110X XXXX
const uint8_t TWO_OCTET_MASK = 0b11100000;
const uint8_t TWO_OCTET = 0b11000000;
// 1110 XXXX
const uint8_t THREE_OCTET_MASK = 0b11110000;
const uint8_t THREE_OCTET = 0b11100000;
// 1111 0XXXX
const uint8_t FOUR_OCTET_MASK = 0b11111000;
const uint8_t FOUR_OCTET = 0b11110000;

const uint8_t HEXES[16] = {
    '0',
    '1',
    '2',
    '3',
    '4',
    '5',
    '6',
    '7',
    '8',
    '9',
    'a',
    'b',
    'c',
    'd',
    'e',
    'f',
};

/**
 * Size of UnicodeChar struct in bytes
 */
const size_t uchar_size_t = sizeof(UnicodeChar);

/**
 * Size of UnicodeString struct in bytes
*/
const size_t ustr_size_t = sizeof(UnicodeString);

#endif //GENGINE_UNICODE_CONSTS_H
