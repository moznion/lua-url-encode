local urlencode = require 'urlencode'

print(urlencode.encode_url("hello こんにちは")) -- => hello+%E3%81%93%E3%82%93%E3%81%AB%E3%81%A1%E3%81%AF
print(urlencode.decode_url("hello+%E3%81%93%E3%82%93%E3%81%AB%E3%81%A1%E3%81%AF")) -- => hello こんにちは
