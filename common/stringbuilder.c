#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stringbuilder.h"

static const size_t sb_default_size = 64;

struct StringBuilder {
    char *str;
    size_t allocated;
    size_t len;
};

StringBuilder *sb_create(void) {
    StringBuilder *sb;
    sb = calloc(1, sizeof(*sb));
    sb->str = malloc(sb_default_size);
    *sb->str = '\0';
    sb->allocated = sb_default_size;
    sb->len = 0;
    return sb;
}

void sb_free(StringBuilder *sb) {
    if (sb == NULL) {
        return;
    }
    free(sb->str);
    free(sb);
}

static void sb_check_space(StringBuilder *sb, size_t req_len) {
    if (sb == NULL || req_len == 0) {
        return;
    }

    if (sb->allocated >= sb->len + req_len + 1) {
        return;
    }

    while (sb->allocated < sb->len + req_len + 1) {
        sb->allocated <<= 1;
        if (sb->allocated == 0) {
            sb->allocated--;
        }
    }
    sb->str = realloc(sb->str, sb->allocated);
}

void sb_append_str(StringBuilder *sb, const char *str, size_t len) {
    if (sb == NULL || sb == NULL || *str == '\0') {
        return;
    }

    if (len == 0) {
        len = strlen(str);
    }

    sb_check_space(sb, len);
    memmove(sb->str + sb->len, str, len);
    sb->len += len;
    sb->str[sb->len] = '\0';
}

void sb_append_char(StringBuilder *sb, char c) {
    if (sb == NULL) {
        return;
    }

    sb_check_space(sb, 1);
    sb->str[sb->len] = c;
    sb->len++;
    sb->str[sb->len] = '\0';
}

void sb_append_int(StringBuilder *sb, int i) {

    if (sb == NULL) {
        return;
    }

    char str[12];

    snprintf(str, sizeof(str), "%d", i);
    sb_append_str(sb, str, 0);
}

void sb_erase(StringBuilder *sb) {
    if (sb == NULL) {
        return;
    }

    sb_truncate(sb, 0);
}

void sb_truncate(StringBuilder *sb, size_t len) {
    if (sb == NULL || len >= sb->len) {
        return;
    }

    sb->len = len;
    sb->str[sb->len] = '\0';
}

size_t sb_length(const StringBuilder *sb) {
    if (sb == NULL) {
        return 0;
    } else {
        return sb->len;
    }
}

char *sb_get(const StringBuilder *sb) {

    if (sb == NULL) {
        return NULL;
    }

    char *str = malloc(sb->len + 1);
    memcpy(str, sb->str, sb->len + 1);
    return str;
}