#include <stdio.h>
#include "util.h"

#ifdef __cplusplus
extern "C" {
#endif

static int _nextid = 0;

int create(void)
{
    return _nextid++;
}

void destroy(int id)
{
    printf("%d - destroyed\n", id);
}

void argument(int id, char* key, char* value)
{
    printf("%d - argument: %s = %s\n", id, key, value);
}

void start(UNUSED(int id))
{
    printf("%d - started\n", id);
}

#ifdef __cplusplus
}
#endif
