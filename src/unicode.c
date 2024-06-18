//
// Created by Георгий Имешкенов on 12.10.2023.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "unicode_consts.h"

UnicodeChar
read_unicode_char(const char *pStr) {
    char octets = get_octets_num(pStr);
    UnicodeChar uchar = {{0, 0, 0, 0}, octets};

    for (size_t i = 0; i < octets; i++) {
        uchar.octet[i] = *(pStr++);
    }

    return uchar;
}

void
read_unicode_char_fast(const char *pStr, UnicodeChar **pUstr) {
    char octets = get_octets_num(pStr);
    (*pUstr)->size = octets;
    if (!octets) {
//        we assume that we have 4 bytes because we want to print all 4 symbols of hex-representation
        (*pUstr)->size = 4;
        *(*pUstr)->octet = 92;          // '\'
        *((*pUstr)->octet + 1) = 120;   // 'x'
        *((*pUstr)->octet + 2) = HEXES[(15 - (~(*pStr) >> 4)) % 16];
        *((*pUstr)->octet + 3) = HEXES[-((~(*pStr & 15)) + 1) % 16];
        return;
    }
    for (size_t i = 0; i < octets; i++) {
        *((*pUstr)->octet + i) = *(pStr++);
    }
}

void
read_into_unicode_array(const char *pStr, UnicodeChar **pUstr) {
    *pUstr = (UnicodeChar *) calloc((strlen(pStr) + 1), uc_size_t);
    UnicodeChar *pInit = *pUstr;

    while ((*pStr) != '\0') {
        read_unicode_char_fast(pStr, pUstr);
        pStr += (*pUstr)->size;
        ++(*pUstr);
    }

    (*(*pUstr)) = (UnicodeChar) {0};
    *pUstr = pInit;
}

UnicodeString *
read_into_unicode_string(const char *pStr) {
    UnicodeString *str = (UnicodeString *) calloc(1, sizeof(struct UnicodeString_s));
    str->len = 1;
    read_into_unicode_array(pStr, &str->data);
    while ((*(str->data)++).size != 0) str->len++;
    *&str->data -= str->len;
    return str;
}

CompressedUnicodeString *
compress_into_bytes_array(UnicodeString *string) {
    int bytes_count = 0;

    unsigned char *compressed_string = calloc(string->len * 4, sizeof(unsigned char));
    if (compressed_string == NULL) {
        exit(3);
    }

    while ((*string->data).size != 0) {
        memcpy(compressed_string + bytes_count, string->data->octet, string->data->size);
        bytes_count += string->data->size;
        string->data++;
    }
    string->data -= string->len - 1;

    CompressedUnicodeString *compressed = calloc(1, sizeof(CompressedUnicodeString));
    if (compressed == NULL) {
        exit(-1);
    }

    compressed->data = malloc(bytes_count + 1);
    if (compressed->data == NULL) {
        exit(3);
    }

    compressed->len = bytes_count + 1;
    memmove(compressed->data, compressed_string, bytes_count);
    free(compressed_string);
    *(compressed->data + bytes_count + 1) = '\0';

    return compressed;
}

UnicodeChar
read_unicode_char_with_offset(char *pStr, int offset) {
    char *pStr_shifted = pStr + offset;
    return read_unicode_char(pStr_shifted);
}

UnicodeChar
read_unicode_char_with_offset_safe(char *pStr, int offset) {
    char *pStr_shifted = pStr + offset;
    while (!get_octets_num(pStr_shifted)) {
        pStr_shifted++;
    }
    return read_unicode_char(pStr_shifted);
}

char
get_octets_num(const char *c) {
    if (!((*c & ONE_OCTET_MASK) ^ ONE_OCTET)) {
        return 1;
    } else if (!((*c & TWO_OCTET_MASK) ^ TWO_OCTET)) {
        return 2;
    } else if (!((*c & THREE_OCTET_MASK) ^ THREE_OCTET)) {
        return 3;
    } else if (!((*c & FOUR_OCTET_MASK) ^ FOUR_OCTET)) {
        return 4;
    } else {
        return 0;
    }
}

void
print_unicode_string(UnicodeChar *pUstr) {
    while ((*pUstr).size != 0) {
        print_unicode_char(*pUstr);
        ++pUstr;
    }
}

void
print_unicode_char(UnicodeChar uchar) {
    for (size_t i = 0; i < uchar.size; i++) {
        putchar(uchar.octet[i]);
    }
}

int
unicode_significant_bytes(const UnicodeChar *uchar) {
    return uchar->size;
}

int
unicode_ord(UnicodeChar uchar) {
    size_t ord = 0;
    int oct_0;
    int oct_1;
    int oct_2;
    int oct_3;
    int sign_bytes = unicode_significant_bytes(&uchar);

    if (!sign_bytes) {
        return 0;
    }

    switch (sign_bytes) {
        case 1:
            ord = uchar.octet[0];
            break;
        case 2:
            oct_0 = get_octet_value(uchar.octet[0], 5);
            oct_1 = get_octet_value(uchar.octet[1], 7);
            ord = ((oct_0 << 6) + oct_1);
            break;
        case 3:
            oct_0 = get_octet_value(uchar.octet[0], 4);
            oct_1 = get_octet_value(uchar.octet[1], 7);
            oct_2 = get_octet_value(uchar.octet[2], 7);
            ord = (oct_0 << 12) + (oct_1 << 6) + oct_2;
            break;
        case 4:
            oct_0 = get_octet_value(uchar.octet[0], 3);
            oct_1 = get_octet_value(uchar.octet[1], 7);
            oct_2 = get_octet_value(uchar.octet[2], 7);
            oct_3 = get_octet_value(uchar.octet[3], 7);
            ord = (oct_0 << 18) + (oct_1 << 12) + (oct_2 << 6) + oct_3;
            break;
        default:
            break;
    }

    return (int) ord;
}

UnicodeChar
unicode_chr(int char_ord) {
    UnicodeChar uchr = {0};

    if (char_ord >= 0 && char_ord <= MAX_UNICODE_CHAR[0]) {
        uchr.octet[0] = char_ord;

    } else if (char_ord > MAX_UNICODE_CHAR[0] && char_ord < MAX_UNICODE_CHAR[1]) {
        uchr.octet[1] = CONTINUE_OCTET + get_next_octet(&char_ord, 6);
        uchr.octet[0] = START_TWO_OCTET + char_ord;

    } else if (char_ord > MAX_UNICODE_CHAR[1] && char_ord < MAX_UNICODE_CHAR[2]) {
        uchr.octet[2] = CONTINUE_OCTET + get_next_octet(&char_ord, 6);
        uchr.octet[1] = CONTINUE_OCTET + get_next_octet(&char_ord, 6);
        uchr.octet[0] = START_THREE_OCTET + char_ord;

    } else if (char_ord > MAX_UNICODE_CHAR[2] && char_ord < MAX_UNICODE_CHAR[3]) {
        uchr.octet[3] = CONTINUE_OCTET + get_next_octet(&char_ord, 6);
        uchr.octet[2] = CONTINUE_OCTET + get_next_octet(&char_ord, 6);
        uchr.octet[1] = CONTINUE_OCTET + char_ord;
        uchr.octet[0] = START_FOUR_OCTET;
    }

    return uchr;
}

int
get_next_octet(int *ord, int shift) {
    int ret_val = *ord - (((*ord) >> shift) << shift);
    *ord = *ord >> shift;
    return ret_val;
}

int
get_octet_value(int octet_raw, int shift) {
    return octet_raw - (((octet_raw) >> shift) << shift);
}