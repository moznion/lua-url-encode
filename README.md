lua-url-encode
==

A URL encoder/decoder library for lua with native extension.

This library aims to use within `application/x-www-form-urlencoded`.

Synopsis
--

```lua
local urlencode = require 'urlencode'

urlencode.encode_url("hello こんにちは") -- => hello+%E3%81%93%E3%82%93%E3%81%AB%E3%81%A1%E3%81%AF
urlencode.decode_url("hello+%E3%81%93%E3%82%93%E3%81%AB%E3%81%A1%E3%81%AF") -- => hello こんにちは
```

Functions
--

### `encode_url(str)`

This function encodes the string as URL encoding. The string **MUST** be utf8.

### `decode_url(str)`

This function decodes the string as URL encoding. The string **MUST** be utf8.

Disclaimer
--

This library only works certainly with utf8 string.

Benchmark
--

Benchmark code is [here](./benchmark/bench.lua):

```
pure lua: encode url system: 15.360     user: 0.100     total: 15.460   real: 16.125
encodeurl: encode url system: 0.240     user: 0.040     total: 0.280    real: 0.312
pure lua: decode url system: 10.160     user: 0.060     total: 10.220   real: 10.625
encodeurl: decode url system: 0.260     user: 0.040     total: 0.300    real: 0.375
```

- urlencode's encoding is about **64 times faster** than pure lua implementation
- urlencode's decoding is about **39 times faster** than pure lua implementation

Original code
--

This library takes lots of code from [p5-url-encode-xs](https://github.com/chansen/p5-url-encode-xs).

License
--

```
The MIT License (MIT)
Copyright © 2018 moznion, http://moznion.net/ <moznion@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the “Software”), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
```

