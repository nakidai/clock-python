#if 0
/usr/bin/env FILE="$0" sh -c '                      \
filename="/tmp/temp-$(date +%s%N)" &&               \
${CC:-/usr/bin/env cc} -w -o "$filename" "$FILE" && \
"$filename";                                        \
rm -f "$filename"'
exit
#endif

#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <time.h>
#include <unistd.h>

static const char *const weekdays[] = {
    "Monday",
    "Tuesday",
    "Wednesday",
    "Thursday",
    "Friday",
    "Saturday",
    "Sunday"
};

static const char *const months[] = {
    "January",
    "February",
    "March",
    "April",
    "May",
    "June",
    "July",
    "August",
    "September",
    "October",
    "November",
    "December"
};

int main(void)
{
    const time_t            rawtime  = time(NULL);
    const struct tm *const  timeinfo = localtime(&rawtime);
    tzset();
    printf(
        "class Clock:\n"
        "    hour: int = %d\n"
        "    minute: int = %d\n"
        "    second: int = %d\n"
        "    day: list[int, str] = [%d, \"%s\"]\n"
        "    month: str = \"%s\"\n"
        "    year: int = %d\n"
        "    timezone: str = \"%s\"\n"
        "    username: str = \"%s\"\n",
        timeinfo->tm_hour,
        timeinfo->tm_min,
        timeinfo->tm_sec,
        timeinfo->tm_mday, weekdays[timeinfo->tm_wday],
        months[timeinfo->tm_mon],
        timeinfo->tm_year + 1900,
        tzname[0],
        getlogin()
    );
}
