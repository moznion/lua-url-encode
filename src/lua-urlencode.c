#include "lua-urlencode.h"

static char* _encode_url(const char* input) {
    static const char xdigit[16] = "0123456789ABCDEF";
    static const int url_unreserved[256] = {
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, /* 0x00-0x0F */
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, /* 0x10-0x1F */
        0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0, /* 0x20-0x2F */
        1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0, /* 0x30-0x3F */
        0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, /* 0x40-0x4F */
        1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1, /* 0x50-0x5F */
        0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, /* 0x60-0x6F */
        1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,0, /* 0x70-0x7F */
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, /* 0x80-0x8F */
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, /* 0x90-0x9F */
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, /* 0xA0-0xAF */
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, /* 0xB0-0xBF */
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, /* 0xC0-0xCF */
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, /* 0xD0-0xDF */
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, /* 0xE0-0xEF */
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, /* 0xF0-0xFF */
    };

    const long len = strlen(input);
    const char* endmarker = input + len;
    char* encoded;
    encoded = (char*)malloc(sizeof(char) * len * 3 + 1);

    for (int i = 0; input < endmarker; input++) {
        const int c = *input;

        if (url_unreserved[c]) {
            encoded[i++] = c;
            continue;
        }

        if (c == ' ') {
            encoded[i++] = '+';
            continue;
        }

        encoded[i++] = '%';
        encoded[i++] = xdigit[c >> 4];
        encoded[i++] = xdigit[c & 15];
    }

    return encoded;
}

static int encode_url (lua_State* L) {
    const char* input = luaL_checkstring(L, 1);
    const char* encoded = _encode_url(input);
    lua_pushstring(L, encoded);
    return 1;
}

static char* _decode_url(const char* input) {
#define __ 256
    static const int hexval[256] = {
        __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__, /* 00-0F */
        __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__, /* 10-1F */
        __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__, /* 20-2F */
         0, 1, 2, 3, 4, 5, 6, 7, 8, 9,__,__,__,__,__,__, /* 30-3F */
        __,10,11,12,13,14,15,__,__,__,__,__,__,__,__,__, /* 40-4F */
        __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__, /* 50-5F */
        __,10,11,12,13,14,15,__,__,__,__,__,__,__,__,__, /* 60-6F */
        __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__, /* 70-7F */
        __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__, /* 80-8F */
        __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__, /* 90-9F */
        __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__, /* A0-AF */
        __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__, /* B0-BF */
        __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__, /* C0-CF */
        __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__, /* D0-DF */
        __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__, /* E0-EF */
        __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__, /* F0-FF */
    };
#undef __

    const long len = strlen(input);
    const char* endmarker = input + len - 2;
    char* decoded;
    decoded = (char*)malloc(sizeof(char) * len + 1);

    int i = 0;
    for (; input < endmarker; input++, i++) {
        const int c = *input;

        if (c == '+') {
            decoded[i] = ' ';
            continue;
        }

        if (c != '%') {
            decoded[i] = c;
            continue;
        }

        const int v1 = hexval[(int)*++input];
        const int v2 = hexval[(int)*++input];
        if ((v1 | v2) != 0xFF) {
            decoded[i] = (v1 << 4) | v2;
            continue;
        }

        decoded[i] = c;
        input -= 2;
    }

    endmarker += 2;
    for (; input < endmarker; input++, i++) {
        const int c = *input;
        if (c == '+') {
            decoded[i] = ' ';
            continue;
        }

        decoded[i] = c;
    }

    return decoded;
}

static int decode_url (lua_State* L) {
    const char* input = luaL_checkstring(L, 1);
    const char* decoded = _decode_url(input);
    lua_pushstring(L, decoded);
    return 1;
}

static const struct luaL_Reg R[] = {
    {"encode_url", encode_url},
    {"decode_url", decode_url},
    {NULL, NULL},
};

#ifdef __cplusplus
extern "C" {
#endif

LUALIB_API int luaopen_urlencode(lua_State * L) {
    /*
     * Register module
     */
#if !defined(LUA_VERSION_NUM) || LUA_VERSION_NUM < 502 // lua < 5.2
    luaL_register(L, URL_ENCODE_LIBNAME, R);
#else
    lua_newtable(L);
    luaL_setfuncs(L, R, 0);
#endif

    /*
     * Register module information
     */
    lua_pushliteral(L, URL_ENCODE_VERSION);
    lua_setfield(L, -2, "_VERSION");

    lua_pushliteral(L, URL_ENCODE_COPYRIGHT);
    lua_setfield(L, -2, "_COPYRIGHT");

    lua_pushliteral(L, URL_ENCODE_DESCRIPTION);
    lua_setfield(L, -2, "_DESCRIPTION");

    return 1;
}

#ifdef __cplusplus
}
#endif
