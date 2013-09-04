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

int call_do_something(float a, float b)
{
    printf("Beginning call to host\n");
    void* inv = plugin_interface.begin_host_call(plugin_interface.host, "do_something");
    plugin_interface.host_invoke.arg_float(inv, a);
    plugin_interface.host_invoke.arg_float(inv, b);
    int retval = plugin_interface.host_invoke.ret_int(inv);
    plugin_interface.end_host_call(plugin_interface.host, inv);
    return retval;
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
    plugin_interface.register_func_c(plugin_interface.host, id, "call_do_something",
                                     "ff)i", (void (*)(void))&call_do_something);
    return 1;
}
