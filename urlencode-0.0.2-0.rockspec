package = "UrlEncode"
version = "0.0.2-0"

source = {
    url = "git://github.com/moznion/lua-url-encode.git",
    branch = "v0.0.2-0",
}

description = {
   summary = "A URL encoder/decoder with native extension",
   homepage = "https://github.com/moznion/lua-url-encode",
   license = "MIT/X11",
   maintainer = "moznion <moznion@gmail.com>",
}

dependencies = {
   "lua >= 5.1, <= 5.4",
}

build = {
   type = "builtin",
   modules = {
      urlencode = "src/lua-urlencode.c"
   },
}
