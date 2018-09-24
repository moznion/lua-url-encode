local urlencode = require 'urlencode'
print (urlencode._VERSION)

local unreserved = '0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_.~-'

local test_cases = {}

table.insert(test_cases, {input = '', encoded = '', err = nil, comment = 'empty string'})
table.insert(test_cases, {input = ' ', encoded = '+', err = nil, comment = 'U+0020 SPACE'})
table.insert(test_cases, {input = unreserved, encoded = unreserved, err = nil, comment = 'unreserved characters'})
table.insert(test_cases, {input = 'å', encoded = '%C3%A5', err = nil, comment = 'U+00E5 in native encoding'})
table.insert(test_cases, {input = "こんにちは", encoded = '%E3%81%93%E3%82%93%E3%81%AB%E3%81%A1%E3%81%AF', err = nil, comment = 'multi-byte characters (Japanese)'})

for i, c in ipairs(test_cases) do
    local encoded, err = urlencode.encode_mb_url(c.input, 'ja_JP.UTF-8')
    assert(encoded == c.encoded, 'MB enc: ' .. c.comment)
    assert(err == c.err, 'enc: ' ..  c.comment .. ' (err)')
    assert(urlencode.decode_mb_url(encoded, 'ja_JP.UTF-8') == c.input, 'MB dec: ' .. c.comment)
end

print 'OK'

