//
// Created by Георгий Имешкенов on 12.10.2023.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "unicode.h"

UnicodeChar
read_unicode_char(const char *pStr) {
    UnicodeChar uchar = {0};
    char octets = get_octets_num(pStr);

    if (!octets) {
        return (UnicodeChar) {0};
    }

    for (size_t i = 0; i < octets; i++) {
        uchar.octet[i] = *(pStr++);
    }

    return uchar;
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
    if (((*c & ONE_OCTET_MASK)) == ONE_OCTET) {
        return 1;
    } else if (((*c & TWO_OCTET_MASK)) == TWO_OCTET) {
        return 2;
    } else if (((*c & THREE_OCTET_MASK)) == THREE_OCTET) {
        return 3;
    } else if (((*c & FOUR_OCTET_MASK)) == FOUR_OCTET) {
        return 4;
    } else {
        return 0;
    }
}

void
read_unicode_string(const char *pStr, UnicodeChar **pUstr) {
    *pUstr = (UnicodeChar *) malloc(uc_size_t * (strlen(pStr) + 1));
    UnicodeChar *pInit = *pUstr;

    while ((*pStr) != '\0') {
        *(*pUstr) = read_unicode_char(pStr);
        pStr += unicode_significant_bytes(*pUstr);
        ++(*pUstr);
    }

    (*(*pUstr)) = (UnicodeChar) {0};
    *pUstr = pInit;
}

void
print_unicode_string(UnicodeChar *pUstr) {
    while ((*pUstr).octet[0] != '\0') {
        print_unicode_char(*pUstr);
        ++pUstr;
    }
}

void
print_unicode_char(UnicodeChar uchar) {
    for (size_t i = 0, c; (c = uchar.octet[i]) != '\0' && i < 4; i++) {
        putchar((char) c);
    }
}

int
unicode_significant_bytes(const UnicodeChar *uchar) {
    size_t i = 0;
    while (uchar->octet[i] != '\0' && i < 4) i++;
    return (int) i;
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