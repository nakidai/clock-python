#if 0
/usr/bin/env FILE="$0" sh -c '                      \
filename="/tmp/temp-$(date +%s%N)" &&               \
${CC:-/usr/bin/env cc} -w -o "$filename" "$FILE" && \
"$filename";                                        \
rm -f "$filename"'
exit
#endif

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define LOCALTImE "/etc/localtime" 
#define TIMEZONE_LENGTH 32
#define USERNAME_LENGTH 32
#define lengthof(array) (sizeof(array) / sizeof(*array))

char *weekdays[] = {
    "Monday",
    "Tuesday",
    "Wednesday",
    "Thursday",
    "Friday",
    "Saturday",
    "Sunday"
};

char *months[] = {
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

void gettimezone(char *timezone, size_t timezone_len)
{
    char linkbuf[128], *name, *p1, *p2;
    size_t link_len;

    if ((link_len = readlink(LOCALTImE, linkbuf, sizeof(linkbuf) - 1)) < 0)
    {
        fprintf(stderr, "cannot read link %s: %s", LOCALTImE, strerror(errno));
        exit(EXIT_FAILURE);
    }
    linkbuf[link_len] = '\0';

    name = linkbuf;
    if ((p1 = strstr(linkbuf, "zone")))
    {
        if ((p2 = strstr(p1 + 1, "zone")))
            p1 = p2;
        if ((p2 = strchr(p1, '/')))
            name = p2 + 1;
    }

    strncpy(timezone, name, timezone_len - 1);
}

int main(int argc, char **argv)
{
    char timezone[TIMEZONE_LENGTH], username[USERNAME_LENGTH];
    time_t rawtime;
    struct tm *timeinfo;

    gettimezone(timezone, lengthof(timezone));
    getlogin_r(username, lengthof(username));
    time(&rawtime);
    timeinfo = localtime(&rawtime);

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
        timezone,
        username
    );

    return EXIT_SUCCESS;
}
