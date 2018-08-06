#include <string.h>

/*
 * Standard C string function: tokenize a string splitting based on a
 * list of separator characters. Reentrant version.
 *
 * The "context" argument should point to a "char *" that is preserved
 * between calls to strtok_r that wish to operate on same string.
 */
char* strtokf(char *string, const char *seps, char **context)
{
        char *head;  /* start of word */
        char *tail;  /* end of word */

        /* If we're starting up, initialize context */
        if (string) {
                *context = string;
        }

        /* Get potential start of this next word */
        head = *context;
        if (head == NULL) {
                return NULL;
        }

        /* Skip any leading separators */
        while (*head && strchr(seps, *head)) {
                head++;
        }

        /* Did we hit the end? */
        if (*head == 0) {
                /* Nothing left */
                *context = NULL;
                return NULL;
        }

        /* skip over word */
        tail = head;
        while (*tail && !strchr(seps, *tail)) {
                tail++;
        }

        /* Save head for next time in context */
        if (*tail == 0) {
                *context = NULL;
        }
        else {
                *tail = 0;
                tail++;
                *context = tail;
        }

        /* Return current word */
        return head;
}
