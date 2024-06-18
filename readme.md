# Unicode for C
A little library made to read Unicode characters from stream / char array 

### Why? 
Under the hood C does not have Unicode char support. This library add struct 
`UnicodeChar` that contains a valid Unicode character

### API
Main API consists of 4 functions that you should use:

`void read_into_unicode_array(char *array, UnicodeChar **string)` - read given `array` of chars into `string`

`void print_unicode_string(UnicodeChar *string)` - print given unicode character

`int unicode_ord(UnicodeChar uchar)` - returns ordinal of Unicode char

`UnicodeChar unicode_chr(int char_ord)` - returns UnicodeChar from given ordinal

### Examples 
Example usages with different languages: 

```c
#include "unicode.c"

int
main() {
    char *mix = "Привет, \xff \xbf \xd3 \x88ອັກສອນລາວ 🇷🇺World";
    UnicodeChar *string;
    UnicodeChar uchar;

    read_into_unicode_array(mix, &string);
    print_unicode_string(string);
    free(string);
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
```

### How to use?
Just copy `unicode.c` and `unicode.h` files into your project