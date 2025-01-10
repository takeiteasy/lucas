#define LUCAS_IMPL
#include "lucas.h"
#include <stdlib.h>

static int l_test_ref(lua_State *L) {
    struct ref_sub {
        int    x;
        int    y;
    } *s;
    struct ref_main {
        int         a;
        int         b;
        struct ref_sub    *ref;
    } *m;
    
    luacs_newstruct(L, ref_sub);
    luacs_int_field(L, ref_sub, x, 0);
    luacs_int_field(L, ref_sub, y, 0);
    
    luacs_newstruct(L, ref_main);
    luacs_int_field(L, ref_main, a, 0);
    luacs_int_field(L, ref_main, b, 0);
    luacs_objref_field(L, ref_main, ref_sub, ref, 0);
    lua_pop(L, 2);
    
    m = calloc(1, sizeof(struct ref_main));
    s = calloc(1, sizeof(struct ref_sub));
    m->a = 1;
    m->b = 2;
    m->ref = s;
    s->x = 3;
    s->y = 4;
    
    luacs_newobject(L, "ref_main", m);

    return 1;
}

int main(int argc, const char *argv[]) {
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    
    lua_register(L, "test_ref", l_test_ref);
    
    int result = luaL_dofile(L, "test.lua");
    if (result) {
        fprintf(stderr, "LUA ERROR: %s\n", lua_tostring(L, -1));
        return 1;
    }
    return 0;
}
