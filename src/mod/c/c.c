#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "util.h"
#include "plugin_i.h"

struct plugin_interface plugin_interface;

static bool is_created = false;
static int current_id = 0;

//
// Plugin Interface
//
int do_something(float a, float b)
{
    printf("Doing some math from mod_c\n");
    return (int)(a*b);
}

//
// Loader Interface
//
int create(void)
{
    if (is_created) {
        return 0;
    }

    is_created = true;
    return ++current_id;
}

int destroy(int id)
{
    if (!is_created || id != current_id) {
        return 0;
    }
    is_created = false;
    return 1;
}

int argument(int id, UNUSED(char* key), UNUSED(char* value))
{
    // Ignore all arguments
    return id == current_id;
}

int start(int id)
{
    if (id != current_id) {
        return 0;
    }

    printf("Starting C plugin.\n");
    plugin_interface.register_func_c(plugin_interface.host, id, "do_something",
                                     "ff)i", (void (*)(void))&do_something);
    return 1;
}
