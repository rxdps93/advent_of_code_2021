#include <stddef.h>

/*
 * Highly inspired by https://nachtimwald.com/2017/02/26/efficient-c-string-builder/
 * All credit to him :)
 */

struct StringBuilder;
typedef struct StringBuilder StringBuilder;

// Create string builder object
StringBuilder *sb_create(void);

// Free string builder object
void sb_free(StringBuilder *sb);

// Append string to end
void sb_append_str(StringBuilder *sb, const char *str, size_t len);

// Append char to end
void sb_append_char(StringBuilder *sb, char c);

// Append int to end
void sb_append_int(StringBuilder *sb, int i);

// Erase data
void sb_erase(StringBuilder *sb);

void sb_truncate(StringBuilder *sb, size_t len);

// Current length
size_t sb_length(const StringBuilder *sb);

char *sb_get(const StringBuilder *sb);