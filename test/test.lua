local urlencode = require 'urlencode'
print ('VERSION: ' .. urlencode._VERSION)

local unreserved = '0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_.~-'

local test_cases = {}

table.insert(test_cases, {input = '', encoded = '', comment = 'empty string'})
table.insert(test_cases, {input = ' ', encoded = '+', comment = 'U+0020 SPACE'})
table.insert(test_cases, {input = unreserved, encoded = unreserved, comment = 'unreserved characters'})
table.insert(test_cases, {input = 'å', encoded = '%C3%A5', comment = 'U+00E5 in native encoding'})
table.insert(test_cases, {input = "hello こんにちは", encoded = 'hello+%E3%81%93%E3%82%93%E3%81%AB%E3%81%A1%E3%81%AF', comment = 'with multi-byte characters (Japanese)'})
table.insert(test_cases, {input = 'ttテeeストstst', encoded = 'tt%E3%83%86ee%E3%82%B9%E3%83%88stst', comment = 'native string'})


for cc = 0x01, 0x1F do
    local c = string.char(cc)
    if not unreserved:find(c, 0, true) then
        local code = '%' .. string.format("%02X", cc)
        local comment = string.format("ordinal %d", cc)
        table.insert(test_cases, {
            input = c, encoded = code, comment = comment
        })
    end
end

for cc = 0x21, 0xFF do
    local c = string.char(cc)
    if not unreserved:find(c, 0, true) then
        local code = '%' .. string.format("%02X", cc)
        local comment = string.format("ordinal %d", cc)
        table.insert(test_cases, {
            input = c, encoded = code, comment = comment
        })
    end
end

for i, c in ipairs(test_cases) do
    local encoded = urlencode.encode_url(c.input)
    assert(encoded == c.encoded, 'enc: ' .. c.comment)
    assert(urlencode.decode_url(encoded) == c.input, 'dec: ' .. c.comment)
end

do
    local encoded = urlencode.encode_url(string.char(0x00))
    assert(encoded == '')
    assert(urlencode.decode_url(encoded) == '')
end

do
    local encoded = urlencode.encode_url('test test' .. string.char(0x00) .. ' test')
    assert(encoded == 'test+test')
    assert(urlencode.decode_url(encoded) == 'test test')
end

print 'OK'

