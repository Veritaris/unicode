//
// Created by Георгий Имешкенов on 13.10.2023.
//

#include "unicode.c"
#include <time.h>

#if defined(_WIN32)
#define PLATFORM_NAME "windows" // Windows
#define HARDWARE_INFO_CALL "msinfo32"
#elif defined(_WIN64)
#define PLATFORM_NAME "windows" // Windows
#define HARDWARE_INFO_CALL "msinfo32"
#elif defined(__CYGWIN__) && !defined(_WIN32)
#define PLATFORM_NAME "windows" // Windows (Cygwin POSIX under Microsoft Window)
#define HARDWARE_INFO_CALL "msinfo32"
#elif defined(__ANDROID__)
#define PLATFORM_NAME "android" // Android (implies Linux, so it must come first)
#define HARDWARE_INFO_CALL NULL
#elif defined(__linux__)
#define PLATFORM_NAME "linux" // Debian, Ubuntu, Gentoo, Fedora, openSUSE, RedHat, Centos and other
#define HARDWARE_INFO_CALL "lscpu"
#elif defined(__unix__) || !defined(__APPLE__) && defined(__MACH__)
#include <sys/param.h>
#if defined(BSD)
#define PLATFORM_NAME "bsd" // FreeBSD, NetBSD, OpenBSD, DragonFly BSD
#endif
#elif defined(__hpux)
#define PLATFORM_NAME "hp-ux" // HP-UX
#define HARDWARE_INFO_CALL NULL
#elif defined(_AIX)
#define PLATFORM_NAME "aix" // IBM AIX
#define HARDWARE_INFO_CALL NULL
#elif defined(__APPLE__) && defined(__MACH__) // Apple OSX and iOS (Darwin)

#include <TargetConditionals.h>

#if TARGET_IPHONE_SIMULATOR == 1
#define PLATFORM_NAME "ios" // Apple iOS
#define HARDWARE_INFO_CALL NULL
#elif TARGET_OS_IPHONE == 1
#define PLATFORM_NAME "ios" // Apple iOS
#define HARDWARE_INFO_CALL NULL
#elif TARGET_OS_MAC == 1
#define PLATFORM_NAME "osx" // Apple OSX
#define HARDWARE_INFO_CALL "/usr/sbin/system_profiler SPHardwareDataType"
#endif
#elif defined(__sun) && defined(__SVR4)
#define PLATFORM_NAME "solaris" // Oracle Solaris, Open Indiana
#define HARDWARE_INFO_CALL NULL
#else
#define PLATFORM_NAME NULL
#define HARDWARE_INFO_CALL NULL
#endif

const char *mix = "OoZe9ab8 \07Eef9hooj \nweiph7iD\r OoMai8ra FaHeaf7g ingie2Ah\t Wei4abah uL3pheog тхеед4Бе еЦхамаЪ0 Привет, 😀ອັກສອນລາວ World";
// TODO benchmark for comparing Unicode chars
//const size_t cmp_str_one = u'<';
//const size_t cmp_str_two = u'Ъ';
//const size_t cmp_str_four = '😀';
__attribute__((unused)) unsigned char tmp = '\0';

void
read_benchmark() {
    UnicodeChar *string;
    read_unicode_string(mix, &string);
}

void
iterate_benchmark(UnicodeChar **string) {
    while ((*string)->octet[0] != '\0') {
        tmp = (*string)->octet[0];
        ++(*string);
    }
}

void
compare_benchmark() {

}

int
main(int argc, char **argv) {
    unsigned long runs[7] = {1, 100, 1000, 100000, 1000000, 10000000, 100000000};
    double reads_total_time[7] = {0};
    double iterates_total_time[7] = {0};
    double total_read_time;
    double total_iterate_time;
    long target_runs = 5;
    clock_t start_time;
    double exec_time;

    if (argc > 1) {
        target_runs = strtol(argv[1], (char **) NULL, 10);
    }

    printf("unicode lib benchmark\n");
    printf("compiler used: gcc\n");
    printf("compiler info:\n");
    system("gcc -v");
    printf("machine info:\n");
    if (HARDWARE_INFO_CALL != NULL) {
        system(HARDWARE_INFO_CALL);
    } else {
        printf("Sorry, gathering system info is unavailable for your platform: %s\n", PLATFORM_NAME);
    }
    printf("target string:\n");
    printf("\"\"\"\n");
    printf("%s\n", mix);
    printf("\"\"\"\n");

    for (int i = 0; i < target_runs; i++) {
        total_read_time = 0.0;
        printf("benchmarking %lu iterations\n", runs[i]);
        for (int j = 0; j < runs[i]; j++) {
            start_time = clock();
            read_benchmark();
            exec_time = (double) (clock() - start_time) / CLOCKS_PER_SEC;
            total_read_time += exec_time;
        }
        reads_total_time[i] = total_read_time;
    }

    UnicodeChar *string;
    read_unicode_string(mix, &string);

    for (int i = 0; i < target_runs; i++) {
        total_iterate_time = 0.0;
        printf("benchmarking %lu iterations\n", runs[i]);
        for (int j = 0; j < runs[i]; j++) {
            start_time = clock();
            iterate_benchmark(&string);
            exec_time = (double) (clock() - start_time) / CLOCKS_PER_SEC;
            total_iterate_time += exec_time;
        }
        iterates_total_time[i] = total_iterate_time;
    }
    printf("\nRead string to UnicodeChar sequence benchmark\n");
    printf(
            "| %10s | %14s | %14s | %18s | %10s | %10s |\n",
            "runs",
            "total time, s",
            "total time, μs",
            "total time, ns",
            "avg time per execution, μs",
            "avg time per execution, ns"
    );
    printf("|------------|----------------|----------------|--------------------|----------------------------|----------------------------|\n");

    for (int i = 0; i < 7; i++) {
        printf(
                "| %10lu | %14.3f | %14.3f | %18.3f | %26.3f | %26.3f |\n",
                runs[i],
                reads_total_time[i],
                reads_total_time[i] * 1e6,
                reads_total_time[i] * 1e9,
                reads_total_time[i] * 1e6 / ((double) runs[i]),
                reads_total_time[i] * 1e9 / ((double) runs[i])
        );
    }

    printf("\nIterate over UnicodeChar sequence benchmark\n");
    printf(
            "| %10s | %14s | %14s | %18s | %10s | %10s |\n",
            "runs",
            "total time, s",
            "total time, μs",
            "total time, ns",
            "avg time per execution, μs",
            "avg time per execution, ns"
    );
    printf("|------------|----------------|----------------|--------------------|----------------------------|----------------------------|\n");

    for (int i = 0; i < 7; i++) {
        printf(
                "| %10lu | %14.3f | %14.3f | %18.3f | %26.3f | %26.3f |\n",
                runs[i],
                iterates_total_time[i],
                iterates_total_time[i] * 1e6,
                iterates_total_time[i] * 1e9,
                iterates_total_time[i] * 1e6 / ((double) runs[i]),
                iterates_total_time[i] * 1e9 / ((double) runs[i])
        );
    }
}
