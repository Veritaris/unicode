//
// Created by Георгий Имешкенов on 12.10.2023.
//

#include <stddef.h>

#ifndef FILTERPARSER_UNICODE_H
#define FILTERPARSER_UNICODE_H

#endif //FILTERPARSER_UNICODE_H

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

const char START_ONE_OCTET = 0b00000000;
const char START_TWO_OCTET = 0b11000000;
const char START_THREE_OCTET = 0b11100000;
const char START_FOUR_OCTET = 0b11110000;
const char CONTINUE_OCTET = 0b10000000;

/**
 * Masks for Unicode characters start bytes
 * Mask itself leaves only significant for octet-defining bits
 * 0XXX XXXX
 */
const char ONE_OCTET_MASK = 0b10000000;
const char ONE_OCTET = 0b00000000;
// 110X XXXX
const char TWO_OCTET_MASK = 0b11100000;
const char TWO_OCTET = 0b11000000;
// 1110 XXXX
const char THREE_OCTET_MASK = 0b11110000;
const char THREE_OCTET = 0b11100000;
// 1111 0XXXX
const char FOUR_OCTET_MASK = 0b11111000;
const char FOUR_OCTET = 0b11110000;

/**
 * Unicode char consist of max 4 octets, 1 byte each.
 * Each octet consist of 8 bits, first 1, 3, 4, and 5 bits encodes how octets are necessary
 * for encoding given Unicode character (1, 2, 3 and 4 accordingly).
 * Empty (not-used) octets have null-terminating nul value '\0', so real size of Unicode character is always 4 bytes
 * in RAM memory, but it can be compressed while wring to disk / network by stripping these chars
 */
typedef struct UnicodeChar_s {
    unsigned char octet[4];
} UnicodeChar;

/**
 * Size of UnicodeChar in bytes
 */
const size_t uc_size_t = sizeof(UnicodeChar);

/**
 * Reads one Unicode character from pStr. Does not move pointer forward on read. To read next char you should
 * call `read_unicode_char_with_offset(char *pStr, int offset)`. Offset can be obtained from calling
 * `unicode_significant_bytes(UnicodeChar uchar)` with before extracted UnicodeChar.
 *
 * @param pStr char array pointer to read Unicode character from
 *
 * @return: UnicodeChar
 */
UnicodeChar
read_unicode_char(const char *pStr);

/**
 * Tried to read a Unicode character from pStr starting at offset position. It is not guaranteed that such Unicode
 * char exists and can be read, in that case null-terminator returned.
 * To read Unicode character safely use `read_unicode_char_with_offset_safe(char *pStr, int offset)`
 *
 * @param pStr char array pointer to read Unicode character from
 * @param offset offset to read Unicode character from
 *
 * @return UnicodeChar
 */
UnicodeChar
read_unicode_char_with_offset(char *pStr, int offset);

/**
 * Safe version of read_unicode_char_with_offset - it moves pointer copy until get valid Unicode start byte and
 * then read a character.
 *
 * @param pStr char array pointer to read Unicode character from
 * @param offset offset to read Unicode character from
 *
 * @return UnicodeChar
 */
UnicodeChar
read_unicode_char_with_offset_safe(char *pStr, int offset);

/**
 * Main function to read `char *` string into Unicode-sequence. It reads input pStr in given pUstr. If invalid
 * Unicode byte met it will be replaced with null-terminator, so be careful with it (known bug, will be fixed)
 * Resulting string has length of unicode chars + 1 - last is null-terminating octet
 *
 *
 * @param pStr char array pointer to read Unicode sequence from
 * @param pUstr ptr to UnicodeChar array pointer to read Unicode sequence into
 *
 * @example
 * char *mix = "Привет, 😀ອັກສອນລາວ World";
 * UnicodeChar *string;
 * read_unicode_string(mix, &string);
 * free(string);
 *
 * @attention Invalid Unicode byte will be replaced with null-terminator
 * @attention Memory allocated for pUstr is not freed automatically! You have to utilize it by yourself when you
 * don't need that string anymore (see example)
 */
// TODO: edit null-terminator replacing with byte representation replace
void
read_unicode_string(const char *pStr, UnicodeChar **pUstr);

/**
 * Return octets that given `chr` is encoded with. If `chr` is not valid Unicode start byte 0 is returned that will
 * lead to empty UnicodeChar, e.g. null-terminator
 *
 * @param chr char to calculate encoding octets at
 */
char
get_octets_num(const char *chr);

/**
 * Prints out UnicodeChar sequence
 * @param pUstr UnicodeChar sequence to print
 */
void
print_unicode_string(UnicodeChar *pUstr);

/**
 * Prints single UnicodeChar
 * @param uchar UnicodeChar to print
 */
void
print_unicode_char(UnicodeChar uchar);

/**
 * Returns number of significant (non zero-terminating) bytes (octets) in UnicodeChar. Can be useful while writing
 * a stream of UnicodeChar's in file / network to skip writing emptry bytes
 *
 * @param uchar UnicodeChar to get significant octets from
 * @return number of significant (non zero-terminating) bytes (octets) in UnicodeChar
 */
int
unicode_significant_bytes(const UnicodeChar *uchar);

/**
 * Return value in given octet with given shift
 * @param octet_raw raw octet value
 * @param shift bitwise shift
 * @return bit-shifted octet value without octet defining bits
 */
int
get_octet_value(int octet_raw, int shift);

/**
 * Read octet value right-to-left, giving you right-most octet value without octet header
 * @attention Modifies ord in-place, e.g. shifting it shift bits right
 * @param ord - value ptr to get octet from
 * @param shift - number of bits to shift
 * @return
 */
int
get_next_octet(int *ord, int shift);

/**
 * Returns ordinal position in UTF-8 encoding of uchar
 * @param uchar UnicodeChar to get ordinal of
 * @return ordinal of UnicodeChar in UTF-8 encoding
 */
int
unicode_ord(UnicodeChar uchar);

/**
 * Returns UnicodeChar at ordinal position in UTF-8 encoding
 * @param char_ord ordinal of UnicodeChar
 * @return UnicodeChar from given ordinal
 */
UnicodeChar
unicode_chr(int char_ord);