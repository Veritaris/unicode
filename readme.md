# Unicode for C
A little library made to read Unicode characters from stream / char array 

### Why? 
Under the hood C does not have Unicode char support. This library add struct 
`UnicodeChar` that contains a valid Unicode character

### API
Main API consists of 4 functions that you should use:

`void read_unicode_string(char *array, UnicodeChar **string)` - read given `array` of chars into `string`

`void print_unicode_string(UnicodeChar *string)` - print given unicode character

`int unicode_ord(UnicodeChar uchar)` - returns ordinal of Unicode char

`UnicodeChar unicode_chr(int char_ord)` - returns UnicodeChar from given ordinal

### Examples 
You can see usage examples in `src/main.c`

### How to use?
Just copy `unicode.c` and `unicode.h` files into your project