//
// Created by Георгий Имешкенов on 12.10.2023.
//

#include <stdio.h>
#include <stdlib.h>

#include "main.h"

int
main() {
    const char *mix = "Привет, \xff \xbf \xd3 \x88ອັກສອນລາວ 🇷🇺World";
    UnicodeChar *string;

    read_into_unicode_array((uint8_t *) mix, &string);
    print_unicode_char_array(string);
    free(string);
    putchar('\n');

    const UnicodeString *first = read_into_unicode_string("Привет, ");
    const UnicodeString *second = read_into_unicode_string("мир!");

    const UnicodeString *concatenated = concat_ustr(first, second);

    print_unicode_string(first);
    print_unicode_string(second);
    print_unicode_string(concatenated);

    const UnicodeChar uchar = read_unicode_char_with_offset_safe((uint8_t *) mix, 1);
    print_unicode_char(uchar);
    putchar('\n');

    printf("%i == ", unicode_ord(read_unicode_char("а"))); // 1072 (Russian "а")
    print_unicode_char(unicode_chr(1072));
    putchar('\n');
    printf("%i == ", unicode_ord(read_unicode_char("😀"))); // 128512
    print_unicode_char(unicode_chr(128512));
    putchar('\n');
    printf("%i == ", unicode_ord(read_unicode_char("🥹"))); // 129401
    print_unicode_char(unicode_chr(129401));
    putchar('\n');
    printf("%i == ", unicode_ord(read_unicode_char("ລ"))); // 3749
    print_unicode_char(unicode_chr(3749));
    putchar('\n');

    print_unicode_char(unicode_chr(1072)); // russian "а"
    putchar('\n');
    print_unicode_char(unicode_chr(128512)); // "😀"
    putchar('\n');
    print_unicode_char(unicode_chr(129401)); // "🥹"
    putchar('\n');
    print_unicode_char(unicode_chr(3749)); // "ລ"
}
