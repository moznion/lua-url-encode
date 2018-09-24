local bm = require 'benchmark'
local urlencode = require 'urlencode'

local reporter = bm.bm(6)

-- original code from -> https://gist.github.com/cgwxyz/6053d51e8d7134dd2e30
function encodeURI(str)
    if (str) then
        str = string.gsub (str, "\n", "\r\n")
        str = string.gsub (str, "([^%w ])",
        function (c) return string.format ("%%%02X", string.byte(c)) end)
        str = string.gsub (str, " ", "+")
    end
    return str
end

function decodeURI(s)
    if(s) then
        s = string.gsub(s, '%%(%x%x)',
        function (hex) return string.char(tonumber(hex,16)) end )
        s = string.gsub(s, "+", " ")
    end
    return s
end

reporter:report(function()
    for i = 1, 1e6 do
        encodeURI("hello こんにちは สวัสดี")
    end
end, 'pure lua: encode url')

reporter:report(function()
    for i = 1, 1e6 do
        urlencode.encode_url("hello こんにちは สวัสดี")
    end
end, 'encodeurl: encode url')

reporter:report(function()
    for i = 1, 1e6 do
        decodeURI("hello+%E3%81%93%E3%82%93%E3%81%AB%E3%81%A1%E3%81%AF+%E0%B8%AA%E0%B8%A7%E0%B8%B1%E0%B8%AA%E0%B8%94%E0%B8%B5")
    end
end, 'pure lua: decode url')

reporter:report(function()
    for i = 1, 1e6 do
        urlencode.decode_url("hello+%E3%81%93%E3%82%93%E3%81%AB%E3%81%A1%E3%81%AF+%E0%B8%AA%E0%B8%A7%E0%B8%B1%E0%B8%AA%E0%B8%94%E0%B8%B5")
    end
end, 'encodeurl: decode url')

