//
// Created by Георгий Имешкенов on 12.10.2023.
//
#pragma once

#ifndef FILTERPARSER_UNICODE_H
#define FILTERPARSER_UNICODE_H

/**
 * Unicode char consist of max 4 octets, 1 byte each.
 * Each octet consists of 8 bits, first 1, 3, 4, and 5 bits encode how octets are necessary
 * for encoding given Unicode character (1, 2, 3, and 4 accordingly).
 * Empty (not-used) octets have null-terminating nul value '\0', so the real size of Unicode character is always 4 bytes
 * in RAM, but it can be compressed while wring to disk / network by stripping these chars
 */
#pragma pack(push)
#pragma pack(1)
typedef struct UnicodeChar_s {
    uint8_t octet[4];
    uint8_t size;
} UnicodeChar;

/**
 * UnicodeString represents a string that is formally an array of UnicodeChar structs
 */
typedef struct UnicodeString_s {
    UnicodeChar *data;
    size_t len;
} UnicodeString;

typedef struct CompressedUnicodeString_s {
    uint8_t *data;
    size_t len;
} CompressedUnicodeString;

#pragma pack(pop)

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Reads one Unicode character from pStr. Does not move a pointer forward on read. To read next char, you should
 * call `read_unicode_char_with_offset(char *pStr, uint32_t offset)`. Offset can be obtained from calling
 * `unicode_significant_bytes(UnicodeChar uchar)` with before-extracted UnicodeChar.
 *
 * @param pStr char array pointer to read Unicode character from
 *
 * @return: UnicodeChar
 */
UnicodeChar
read_unicode_char(const uint8_t *pStr);

/**
 * Reads one Unicode character from pStr into pUstr. Faster analog on read_unicode_char because a result is not copied
 * between invocation and assignment but writing directly to allocated memory
 * @param pStr char array pointer to read Unicode character from
 * @param pUstr pointer to UnicodeChar array pointer to read value
 */
void
read_unicode_char_fast(const uint8_t *pStr, UnicodeChar **pUstr);

/**
 * Tried to read a Unicode character from pStr starting at the offset position. It is not guaranteed that such Unicode
 * char exists and can be read; in that case null-terminator returned.
 * To read Unicode character safely, use `read_unicode_char_with_offset_safe(char *pStr, uint32_t offset)`
 *
 * @param pStr char array pointer to read Unicode character from
 * @param offset offset to read Unicode character from
 *
 * @return UnicodeChar
 */
UnicodeChar
read_unicode_char_with_offset(const uint8_t *pStr, uint32_t offset);

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
read_unicode_char_with_offset_safe(const uint8_t *pStr, uint32_t offset);

/**
 * Main function to read `char *` string into an array of UnicodeChar. It reads input pStr in a given pUstr. If an invalid Unicode byte is met,
 *  it will be replaced with null-terminator, so be careful with it (known bug will be fixed)
 * Resulting string has length of Unicode chars + 1 - last is a null-terminating octet
 *
 *
 * @param pStr char array pointer to read Unicode sequence from
 * @param pUstr ptr to UnicodeChar array pointer to read Unicode sequence into
 *
 * @example
 * char *mix = "Привет, 😀ອັກສອນລາວ World";
 * UnicodeChar *string;
 * read_into_unicode_array(mix, &string);
 * free(string);
 *
 * @attention Invalid Unicode byte will be replaced with null-terminator
 * @attention Memory allocated for pUstr is not freed automatically! You have to utilize it by yourself when you
 * don't need that string anymore (see example)
 */
// TODO: edit null-terminator replacing with byte representation replace
void
read_into_unicode_array(const uint8_t *pStr, UnicodeChar **pUstr);

/**
 * Main function to read `char *` string into UnicodeString struct. It reads input pStr in a given pUstr. If an invalid Unicode byte is met,
 * it will be replaced with null-terminator, so be careful with it (known bug will be fixed)
 * Resulting string has length of Unicode chars + 1 - last is null-terminating octet
 *
 *
 * @param pStr char array pointer to read a Unicode sequence from
 *
 * @example
 * ```
 * char *mix = "Привет, 😀ອັກສອນລາວ World";
 * UnicodeString *string = read_into_unicode_array(mix);
 * free(string);
 * ```
 *
 * @return UnicodeString
 *
 * @attention In the worst case (ascii-symbols)
 * @attention Invalid Unicode byte will be replaced with null-terminator
 * @attention Memory allocated for returned string is not freed automatically! You have to utilize it by yourself when
 * you don't need that string anymore (see example)
 */
UnicodeString *
read_into_unicode_string(const uint8_t *pStr);

/**
 * Read UnicodeString into an array of `uint8_t`s. The resulting array is filled with only significant bytes of
 * UnicodeString and null-terminated
 * @param string
 * @return
 */
CompressedUnicodeString *
compress_into_bytes_array(UnicodeString *string);

/**
 * Return octets that a given ` chr ` is encoded with. If `chr` is not valid, Unicode start byte 0 is returned that will
 * lead to empty UnicodeChar, e.g., null-terminator
 *
 * @param chr char to calculate encoding octets at
 */
uint8_t
get_octets_num(const uint8_t *chr);

/**
 * Prints out UnicodeChar sequence
 * @param pUstr UnicodeChar sequence to print
 */
void
print_unicode_string(const UnicodeChar *pUstr);

/**
 * Prints a single UnicodeChar
 * @param uchar UnicodeChar to print
 */
void
print_unicode_char(UnicodeChar uchar);

/**
 * Returns a number of significant (non-zero-terminating) bytes (octets) in UnicodeChar. Can be useful while writing
 * a stream of UnicodeChar's in file / network to skip writing empty bytes
 *
 * @param uchar UnicodeChar to get significant octets from
 * @return number of significant (non-zero-terminating) bytes (octets) in UnicodeChar
 */
uint32_t
unicode_significant_bytes(const UnicodeChar *uchar);

/**
 * Return value in given octet with given shift
 * @param octet_raw raw octet value
 * @param shift bitwise shift
 * @return bit-shifted octet value without octet defining bits
 */
uint32_t
get_octet_value(uint32_t octet_raw, uint32_t shift);

/**
 * Read octet value right-to-left, giving you right-most octet value without an octet header
 * @attention Modifies ord in-place, e.g. shifting it shift bits right
 * @param ord - value ptr to get octet from
 * @param shift - number of bits to shift
 * @return
 */
uint32_t
get_next_octet(uint32_t *ord, uint32_t shift);

/**
 * Returns ordinal position in UTF-8 encoding of uchar
 * @param uchar UnicodeChar to get ordinal of
 * @return ordinal of UnicodeChar in UTF-8 encoding
 */
uint32_t
unicode_ord(UnicodeChar uchar);

/**
 * Returns UnicodeChar at ordinal position in UTF-8 encoding
 * @param char_ord ordinal of UnicodeChar
 * @return UnicodeChar from a given ordinal
 */
UnicodeChar
unicode_chr(uint32_t char_ord);

#ifdef __cplusplus
}
#endif

#endif //FILTERPARSER_UNICODE_H
