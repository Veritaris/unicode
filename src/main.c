//
// Created by Георгий Имешкенов on 12.10.2023.
//

#include "unicode.c"

int
main() {
    char *mix = "Привет, ອັກສອນລາວ World";
    UnicodeChar *string;
    UnicodeChar uchar;

    read_unicode_string(mix, &string);
    print_unicode_string(string);
    putchar('\n');

    uchar = read_unicode_char_with_offset_safe(mix, 1);
    print_unicode_char(uchar);
    putchar('\n');

    printf("%i == ", unicode_ord(read_unicode_char("а")));  // 1072 (russian "а")
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

    print_unicode_char(unicode_chr(1072));           // russian "а"
    putchar('\n');
    print_unicode_char(unicode_chr(128512));         // "😀"
    putchar('\n');
    print_unicode_char(unicode_chr(129401));         // "🥹"
    putchar('\n');
    print_unicode_char(unicode_chr(3749));          // "ລ"

}