//
// Created by Георгий Имешкенов on 12.10.2023.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "unicode_consts.h"
#include "../../dsa/include/public/mallocs.h"

UnicodeChar
read_unicode_char(const uint8_t *pStr) {
    const uint8_t octets = get_octets_num(pStr);
    UnicodeChar uchar = {{0, 0, 0, 0}, octets};

    for (size_t i = 0; i < octets; i++) {
        uchar.octet[i] = *pStr++;
    }

    return uchar;
}

void
read_unicode_char_fast(const uint8_t *pStr, UnicodeChar **pUstr) {
    const uint8_t octets = get_octets_num(pStr);
    (*pUstr)->size = octets;
    if (!octets) {
        // we assume that we have 4 bytes because we want to print all 4 symbols of hex-representation
        (*pUstr)->size = 4;
        *(*pUstr)->octet = 92; // '\'
        *((*pUstr)->octet + 1) = 120; // 'x'
        *((*pUstr)->octet + 2) = HEXES[(15 - (~*pStr >> 4)) % 16];
        *((*pUstr)->octet + 3) = HEXES[-(~(*pStr & 15) + 1) % 16];
        return;
    }
    for (size_t i = 0; i < octets; i++) {
        *((*pUstr)->octet + i) = *pStr++;
    }
}

void
read_into_unicode_array(const uint8_t *pStr, UnicodeChar **pUstr) {
    *pUstr = (UnicodeChar *) calloc(strlen((char *) pStr) + 1, uchar_size_t);
    UnicodeChar *pInit = *pUstr;

    while (*pStr != '\0') {
        read_unicode_char_fast(pStr, pUstr);
        pStr += (*pUstr)->size;
        ++*pUstr;
    }

    **pUstr = (UnicodeChar){0};
    *pUstr = pInit;
}

UnicodeString *
read_into_unicode_string(const uint8_t *pStr) {
    UnicodeString *ccalloc_safe(str, 1, ustr_size_t);
    str->len = 1;
    read_into_unicode_array(pStr, &str->data);
    while (str->data++->size != 0) str->len++;
    str->data -= str->len;
    return str;
}

UnicodeString *
new_ustr(const size_t *size) {
    size_t string_len = 16;
    if (size != NULL) {
        string_len = (size_t) *size;
    }

    UnicodeString *ccalloc_safe(str, 1, ustr_size_t);
    ccalloc_safe(str->data, string_len, uchar_size_t);

    return str;
}

UnicodeString *
concat_ustr(const UnicodeString *self, const UnicodeString *other) {
    const size_t string_len = self->len + other->len;
    UnicodeString *ccalloc_safe(str, 1, ustr_size_t);
    ccalloc_safe(str->data, string_len, uchar_size_t);
    memcpy(str->data, self->data, self->len * uchar_size_t);
    memcpy(str->data + self->len * uchar_size_t, other->data, other->len * uchar_size_t);
    return str;
}

UnicodeString *
push_char(UnicodeString *self, const char chr) {
    const UnicodeChar uchar = unicode_chr(chr);
    return push_uchar(self, uchar);
}

UnicodeString *
push_uchar(UnicodeString *self, const UnicodeChar chr) {
    const size_t string_len = self->len + 1;

    UnicodeString *ccalloc_safe(tmp_str, 1, ustr_size_t);
    ccalloc_safe(tmp_str->data, string_len, ustr_size_t);
    memcpy(tmp_str->data, self->data, self->len * uchar_size_t);

    ccalloc_safe(self->data, string_len, uchar_size_t);
    memcpy(self->data, tmp_str->data, self->len * uchar_size_t);
    self->data[self->len] = chr;

    free_ustr(tmp_str);

    return self;
}

void
free_ustr(UnicodeString *self) {
    free(self->data);
    free(self);
}

CompressedUnicodeString *
compress_into_bytes_array(UnicodeString *string) {
    uint32_t bytes_count = 0;

    uint8_t * ccalloc_safe(compressed_string, string->len * 4, sizeof(uint8_t));

    while (string->data->size != 0) {
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
read_unicode_char_with_offset(const uint8_t *pStr, const uint32_t offset) {
    const uint8_t *pStr_shifted = pStr + offset;
    return read_unicode_char(pStr_shifted);
}

UnicodeChar
read_unicode_char_with_offset_safe(const uint8_t *pStr, const uint32_t offset) {
    const uint8_t *pStr_shifted = pStr + offset;
    while (!get_octets_num(pStr_shifted)) {
        pStr_shifted++;
    }
    return read_unicode_char(pStr_shifted);
}

uint8_t
get_octets_num(const uint8_t *chr) {
    if (!((*chr & ONE_OCTET_MASK) ^ ONE_OCTET)) {
        return 1;
    }
    if (!((*chr & TWO_OCTET_MASK) ^ TWO_OCTET)) {
        return 2;
    }
    if (!((*chr & THREE_OCTET_MASK) ^ THREE_OCTET)) {
        return 3;
    }
    if (!((*chr & FOUR_OCTET_MASK) ^ FOUR_OCTET)) {
        return 4;
    }
    return 0;
}

void
print_unicode_char_array(const UnicodeChar *uCharArray) {
    while (uCharArray->size != 0) {
        print_unicode_char(*uCharArray);
        ++uCharArray;
    }
}

void
print_unicode_string(const UnicodeString *pUstr) {
    for (int i = 0; i < pUstr->len; i++) {
        print_unicode_char(pUstr->data[i]);
    }
}

void
print_unicode_char(const UnicodeChar uchar) {
    for (size_t i = 0; i < uchar.size; i++) {
        putchar(uchar.octet[i]);
    }
}

uint32_t
unicode_significant_bytes(const UnicodeChar *uchar) {
    return uchar->size;
}

uint32_t
unicode_ord(const UnicodeChar uchar) {
    size_t ord = 0;
    uint8_t oct_0;
    uint8_t oct_1;
    uint8_t oct_2;
    uint8_t oct_3;
    const uint32_t sign_bytes = unicode_significant_bytes(&uchar);

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
            ord = (oct_0 << 6) + oct_1;
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

    return (uint32_t) ord;
}

UnicodeChar
unicode_chr(uint32_t char_ord) {
    UnicodeChar uchr = {0};

    if (char_ord > 0 && char_ord <= MAX_UNICODE_CHAR[0]) {
        uchr.octet[0] = char_ord;
        uchr.size = 1;
    } else if (char_ord > MAX_UNICODE_CHAR[0] && char_ord < MAX_UNICODE_CHAR[1]) {
        uchr.octet[1] = CONTINUE_OCTET + get_next_octet(&char_ord, 6);
        uchr.octet[0] = START_TWO_OCTET + char_ord;
        uchr.size = 2;
    } else if (char_ord > MAX_UNICODE_CHAR[1] && char_ord < MAX_UNICODE_CHAR[2]) {
        uchr.octet[2] = CONTINUE_OCTET + get_next_octet(&char_ord, 6);
        uchr.octet[1] = CONTINUE_OCTET + get_next_octet(&char_ord, 6);
        uchr.octet[0] = START_THREE_OCTET + char_ord;
        uchr.size = 3;
    } else if (char_ord > MAX_UNICODE_CHAR[2] && char_ord < MAX_UNICODE_CHAR[3]) {
        uchr.octet[3] = CONTINUE_OCTET + get_next_octet(&char_ord, 6);
        uchr.octet[2] = CONTINUE_OCTET + get_next_octet(&char_ord, 6);
        uchr.octet[1] = CONTINUE_OCTET + char_ord;
        uchr.octet[0] = START_FOUR_OCTET;
        uchr.size = 4;
    }

    return uchr;
}

uint32_t
get_next_octet(uint32_t *ord, const uint32_t shift) {
    const uint32_t next_octet = *ord - (*ord >> shift << shift);
    *ord = *ord >> shift;
    return next_octet;
}

uint32_t
get_octet_value(const uint32_t octet_raw, const uint32_t shift) {
    return octet_raw - (octet_raw >> shift << shift);
}
