# oatpp-bob

*(oatpp-BinaryOBject - BOB)*  
Object-Mapper for binary serialization/deserialization.

## BOB Format

BOB is basically a regular JSON in which values are written in binary format.
It supports JSON data types only - [`object`, `array`, `string`, `number`(ints and floats), `true`, `false`, `null`].
Thus, JSON and oatpp-BOB are 100% interchangeable.

### Format Structure

BOB uses big-endian format (network byte order) to store binary values.

| type      | description                                | format                    |
|-----------|--------------------------------------------|---------------------------|
| `int1`    | 1 - byte signed integer                    | `'1'<1-byte value>`       |
| `int2`    | 2 - byte signed integer                    | `'2'<2-byte value>`       |
| `int4`    | 4 - byte signed integer                    | `'4'<4-byte value>`       |
| `int8`    | 8 - byte signed integer                    | `'8'<8-byte value>`       |                      
| `uint1`   | 1 - byte unsigned integer                  | `'b'<1-byte value>`       |
| `uint2`   | 2 - byte unsigned integer                  | `'i'<2-byte value>`       |
| `uint4`   | 4 - byte unsigned integer                  | `'I'<4-byte value>`       |
| `uint8`   | 8 - byte unsigned integer                  | `'L'<8-byte value>`       |
| `float4`  | 4 - byte float                             | `'f'<4-byte value>`       |
| `float8`  | 8 - byte float                             | `'d'<8-byte value>`       |
| `true`    | boolean true                               | `'+'`                     |
| `false`   | boolean false                              | `'-'`                     |
| `null`    | null value                                 | `'0'`                     |
| `string1` | string with max length of `2^8 - 1` chars  | `'s'<1-byte size><data>`  |
| `string2` | string with max length of `2^16 - 1` chars | `'S'<2-byte size><data>`  |
| `string4` | string with max length of `2^32 - 1` chars | `'$'<4-byte size><data>`  |
| `object`  | sequence of key-value pairs                | `'{'<key-value pairs>')'` |
| `array`   | sequence of values                         | `'['<values>')'`          |

- **Note**: `<key-value>` pairs in object are stored without any delimiters.
Each key is encoded as a null-terminated string.
- **Note**: `<values>` in array are stored without any delimiters. Each value begins with the type-designating byte (char).


Example - JSONs and their equivalent BOBs

```
json = '{"key":"value"}'; // size: 15 bytes
bob  = "{key\0s\7value)"; // size: 13 bytes 
```

```
json = '{"key1":"value1","key2":5}'; // size: 26 bytes 
bob  = "{key1\0s\6value1key2\0b\5)"; // size: 22 bytes 
```

Examples in C++

```cpp
oatpp::String bob("{key\0s\5value)", 13); // <- You have to provide length explicitly because of the '\0' char in the middle of string
auto obj = bobMapper.readFromString<oatpp::Any>(bob);
auto json = jsonMapper.writeToString(obj);
OATPP_LOGD(TAG, "json='%s'", json->c_str()) // <- json='{"key":"value"}'
```

```cpp
oatpp::String bob("{key1\0s\6value1key2\0b\5)", 22); // <- You have to provide length explicitly because of the '\0' char in the middle of string
auto obj = bobMapper.readFromString<oatpp::Any>(bob);
auto json = jsonMapper.writeToString(obj);
OATPP_LOGD(TAG, "json='%s'", json->c_str()) // <- json='{"key1":"value1","key2":5}'
```
