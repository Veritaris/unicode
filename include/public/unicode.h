//
// Created by Георгий Имешкенов on 12.10.2023.
//
#pragma once

#ifndef FILTERPARSER_UNICODE_H
#define FILTERPARSER_UNICODE_H

/**
 * Unicode char consist of max 4 octets, 1 byte each.
 * Each octet consist of 8 bits, first 1, 3, 4, and 5 bits encodes how octets are necessary
 * for encoding given Unicode character (1, 2, 3 and 4 accordingly).
 * Empty (not-used) octets have null-terminating nul value '\0', so real size of Unicode character is always 4 bytes
 * in RAM memory, but it can be compressed while wring to disk / network by stripping these chars
 */
#pragma pack(push, 1)
typedef struct UnicodeChar_s {
    unsigned char octet[4];
    char size;
} UnicodeChar;

/**
 * UnicodeString represents a string that is formally array of UnicodeChar structs
 */
typedef struct UnicodeString_s {
    UnicodeChar *data;
    size_t len;
} UnicodeString;

typedef struct CompressedUnicodeString_s {
    unsigned char *data;
    size_t len;
} CompressedUnicodeString;

#pragma pack(pop)

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Reads one Unicode character from pStr. Does not move pointer forward on read. To read next char you should
 * call `read_unicode_char_with_offset(char *pStr, int offset)`. Offset can be obtained from calling
 * `unicode_significant_bytes(UnicodeChar uchar)` with before-extracted UnicodeChar.
 *
 * @param pStr char array pointer to read Unicode character from
 *
 * @return: UnicodeChar
 */
UnicodeChar
read_unicode_char(const char *pStr);

/**
 * Reads one Unicode character from pStr into pUstr. Faster analog on read_unicode_char because result is not copied
 * between invocation and assignment but writing directly to allocated memory
 * @param pStr char array pointer to read Unicode character from
 * @param pUstr pointer to UnicodeChar array pointer to read value
 */
void
read_unicode_char_fast(const char *pStr, UnicodeChar **pUstr);

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
read_unicode_char_with_offset(const char *pStr, int offset);

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
read_unicode_char_with_offset_safe(const char *pStr, int offset);

/**
 * Main function to read `char *` string into array of UnicodeChar. It reads input pStr in given pUstr. If invalid
 * Unicode byte met it will be replaced with null-terminator, so be careful with it (known bug, will be fixed)
 * Resulting string has length of Unicode chars + 1 - last is null-terminating octet
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
read_into_unicode_array(const char *pStr, UnicodeChar **pUstr);

/**
 * Main function to read `char *` string into UnicodeString struct. It reads input pStr in given pUstr. If invalid
 * Unicode byte met it will be replaced with null-terminator, so be careful with it (known bug, will be fixed)
 * Resulting string has length of Unicode chars + 1 - last is null-terminating octet
 *
 *
 * @param pStr char array pointer to read Unicode sequence from
 * @param pUstr ptr to UnicodeChar array pointer to read Unicode sequence into
 *
 * @example
 * char *mix = "Привет, 😀ອັກສອນລາວ World";
 * UnicodeString *string = read_into_unicode_array(mix);
 * free(string);
 *
 * @return UnicodeString
 *
 * @attention In worst case (ascii-symbols)
 * @attention Invalid Unicode byte will be replaced with null-terminator
 * @attention Memory allocated for returned string is not freed automatically! You have to utilize it by yourself when
 * you don't need that string anymore (see example)
 */
UnicodeString *
read_into_unicode_string(const char *pStr);

/**
 * Read UnicodeString into array of `unsigned char`s. Resulting array is filled with only significant bytes of
 * UnicodeString and null-terminated
 * @param string
 * @return
 */
CompressedUnicodeString *
compress_into_bytes_array(UnicodeString *string);

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
print_unicode_string(const UnicodeChar *pUstr);

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

#ifdef __cplusplus
}
#endif

#endif //FILTERPARSER_UNICODE_H