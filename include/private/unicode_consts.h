//
// Created by Георгий Имешкенов on 22.04.2024.
//

#ifndef GENGINE_UNICODE_CONSTS_H
#define GENGINE_UNICODE_CONSTS_H

#include "unicode.h"

// define max int code for (i+1) octet Unicode char representation
const int MAX_UNICODE_CHAR[4] = {
        0x7F,
        0x7FF,
        0xFFFF,
        0x10FFFF
};

/**
 * Octets headers values
 */

const unsigned char START_ONE_OCTET = 0b00000000;
const unsigned char START_TWO_OCTET = 0b11000000;
const unsigned char START_THREE_OCTET = 0b11100000;
const unsigned char START_FOUR_OCTET = 0b11110000;
const unsigned char CONTINUE_OCTET = 0b10000000;

/**
 * Masks for Unicode characters start bytes
 * Mask itself leaves only significant for octet-defining bits
 * 0XXX XXXX
 */
const unsigned char ONE_OCTET_MASK = 0b10000000;
const unsigned char ONE_OCTET = 0b00000000;
// 110X XXXX
const unsigned char TWO_OCTET_MASK = 0b11100000;
const unsigned char TWO_OCTET = 0b11000000;
// 1110 XXXX
const unsigned char THREE_OCTET_MASK = 0b11110000;
const unsigned char THREE_OCTET = 0b11100000;
// 1111 0XXXX
const unsigned char FOUR_OCTET_MASK = 0b11111000;
const unsigned char FOUR_OCTET = 0b11110000;

char HEXES[16] = {
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
const size_t uc_size_t = sizeof(UnicodeChar);

/**
 * Size of UnicodeString struct in bytes
*/
const size_t us_size_t = sizeof(UnicodeString);

#endif //GENGINE_UNICODE_CONSTS_H
