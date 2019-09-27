#include "lua-urlencode.h"

#define UTF8_LEAD(c) ((uint8_t)(c) < 0x80 || ((uint8_t)(c) > 0xC1 && (uint8_t)(c) < 0xF5))
#define UTF8_TRAIL(c) (((uint8_t)(c) & 0xC0) == 0x80)

uint8_t utf8_len(const uint8_t* str);

static const uint8_t xdigit[16] = "0123456789ABCDEF";
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

static uint8_t* _encode_url(const uint8_t* input) {
    if (input[0] == '\0') {
        return "";
    }

    const long len = strlen(input);
    const uint8_t* endmarker = input + len;
    uint8_t* encoded;
    encoded = (uint8_t*)malloc(sizeof(uint8_t) * len * 3 + 1);

    int in_cursor = 0;
    int out_cursor = 0;
    while (input[in_cursor] != '\0') {
        const uint8_t charlen = utf8_len(&input[in_cursor]);

        if (charlen == 0) {
            continue;
        }

        if (charlen <= 1) {
            const uint8_t c = input[in_cursor];
            in_cursor += charlen;
            if (url_unreserved[c]) {
                encoded[out_cursor++] = c;
            } else if (c == ' ') {
                encoded[out_cursor++] = '+';
            } else {
                encoded[out_cursor++] = '%';
                encoded[out_cursor++] = xdigit[c >> 4];
                encoded[out_cursor++] = xdigit[c & 15];
            }
            continue;
        }

        int i;
        for (i = 0; i < charlen; i++, in_cursor++) {
            const uint8_t c = input[in_cursor];
            encoded[out_cursor++] = '%';
            encoded[out_cursor++] = xdigit[c >> 4];
            encoded[out_cursor++] = xdigit[c & 15];
        }
    }
    encoded[out_cursor] = '\0';

    return encoded;
}

static int encode_url (lua_State* L) {
    const uint8_t* input = luaL_checkstring(L, 1);
    const uint8_t* encoded = _encode_url(input);
    lua_pushstring(L, encoded);
    return 1;
}

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

static uint8_t* _decode_url(const uint8_t* input) {
    const long len = strlen(input);
    const uint8_t* endmarker = input + len;
    uint8_t* decoded;
    decoded = (uint8_t*)malloc(sizeof(uint8_t) * len + 1);

    int in_cursor = 0;
    int out_cursor = 0;
    while (input[in_cursor] != '\0') {
        const uint8_t charlen = utf8_len(&input[in_cursor]);

        if (charlen == 0) {
            continue;
        }

        if (charlen <= 1) {
            const uint8_t c = input[in_cursor++];

            if (c == '+') {
                decoded[out_cursor++] = ' ';
                continue;
            }

            if (c != '%') {
                decoded[out_cursor++] = c;
                continue;
            }

            const unsigned int v1raw = input[in_cursor++];
            const unsigned int v2raw = input[in_cursor++];
            if (v1raw == 0x30 && v2raw == 0x30) {
                // null char termination (%00)
                return decoded;
            }

            const unsigned int v1 = hexval[v1raw];
            const unsigned int v2 = hexval[v2raw];
            if ((v1 | v2) != 0xFF) {
                decoded[out_cursor++] = (v1 << 4) | v2;
                continue;
            }

            decoded[out_cursor] = c;

            continue;
        }

        int i;
        for (i = 0; i < charlen; i++, in_cursor++) {
            const uint8_t c = input[in_cursor];
            decoded[out_cursor++] = c;
        }
    }
    decoded[out_cursor] = '\0';

    return decoded;
}

static int decode_url (lua_State* L) {
    const uint8_t* input = luaL_checkstring(L, 1);
    const uint8_t* decoded = _decode_url(input);

    lua_pushstring(L, decoded);

    return 1;
}

#define __ 0xFF
/*
 * 0x00: 0
 * 0x01-0xC1: 1
 * 0xF5-: 1
 */
static const uint8_t utf8_immediate_len[256] = {
     0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, /* 0x00-0x0F */
     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, /* 0x10-0x1F */
     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, /* 0x20-0x2F */
     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, /* 0x30-0x3F */
     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, /* 0x40-0x4F */
     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, /* 0x50-0x5F */
     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, /* 0x60-0x6F */
     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, /* 0x70-0x7F */
     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, /* 0x80-0x8F */
     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, /* 0x90-0x9F */
     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, /* 0xA0-0xAF */
     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, /* 0xB0-0xBF */
     1, 1,__,__,__,__,__,__,__,__,__,__,__,__,__,__, /* 0xC0-0xCF */
    __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__, /* 0xD0-0xDF */
    __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__, /* 0xE0-0xEF */
    __,__,__,__,__, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, /* 0xF0-0xFF */
};
#undef __

/*
 * 0xC2-0xDF: 2
 * 0xE0-0xEF: 3
 * 0xF0-0xF4: 4
 */
static const uint8_t utf8_count_len[256] = {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, /* 0x00-0x0F */
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, /* 0x10-0x1F */
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, /* 0x20-0x2F */
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, /* 0x30-0x3F */
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, /* 0x40-0x4F */
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, /* 0x50-0x5F */
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, /* 0x60-0x6F */
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, /* 0x70-0x7F */
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, /* 0x80-0x8F */
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, /* 0x90-0x9F */
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, /* 0xA0-0xAF */
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, /* 0xB0-0xBF */
    0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2, /* 0xC0-0xCF */
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2, /* 0xD0-0xDF */
    3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3, /* 0xE0-0xEF */
    4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0, /* 0xF0-0xFF */
};

uint8_t utf8_len(const uint8_t* str) {
    const uint8_t lead = *str;

    const uint8_t immediate_len = utf8_immediate_len[lead];
    if (immediate_len != 0xFF) {
        return immediate_len;
    }

    const uint8_t count = utf8_count_len[lead];
    uint8_t trail = *(++str);

    if (count == 3) {
        if ((lead == 0xE0 && 0xA0 > trail) || (lead == 0xED && trail > 0x9F)) {
            return 1;
        }
    } else if (count == 4) {
        if ((lead == 0xF0 && 0x90 > trail) || (lead == 0xF4 && trail > 0x8F)) {
            return 1;
        }
    }

    uint8_t size = 1;
    for (; size < count; ++size) {
        if (!UTF8_TRAIL(trail)) {
            return size;
        }
        trail = *(++str);
    }
    return size;
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

