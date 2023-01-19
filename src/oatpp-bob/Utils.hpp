/***************************************************************************
 *
 * Project         _____    __   ____   _      _
 *                (  _  )  /__\ (_  _)_| |_  _| |_
 *                 )(_)(  /(__)\  )( (_   _)(_   _)
 *                (_____)(__)(__)(__)  |_|    |_|
 *
 *
 * Copyright 2018-present, Leonid Stryzhevskyi <lganzzzo@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ***************************************************************************/

#ifndef OATPP_BOB_UTILS_HPP
#define OATPP_BOB_UTILS_HPP

#include "oatpp/core/parser/Caret.hpp"
#include "oatpp/core/data/stream/Stream.hpp"
#include "oatpp/core/Types.hpp"

namespace oatpp { namespace bob {

class Utils {
public:

  static constexpr const v_char8 TYPE_NULL = '0';

  static constexpr const v_char8 TYPE_STRING_1 = 's'; // string length 1 byte
  static constexpr const v_char8 TYPE_STRING_2 = 'S'; // string length 2 bytes
  static constexpr const v_char8 TYPE_STRING_4 = '$'; // string length 4 bytes;

  static constexpr const v_char8 TYPE_BOOL_FALSE = '-';
  static constexpr const v_char8 TYPE_BOOL_TRUE = '+';

  static constexpr const v_char8 TYPE_INT_1  = 'b'; // 1 byte int
  static constexpr const v_char8 TYPE_UINT_1 = '1'; // 1 byte int
  static constexpr const v_char8 TYPE_INT_2  = 'i'; // 2 byte int
  static constexpr const v_char8 TYPE_UINT_2 = '2'; // 2 byte int
  static constexpr const v_char8 TYPE_INT_4  = 'I'; // 4 byte int
  static constexpr const v_char8 TYPE_UINT_4 = '4'; // 4 byte int
  static constexpr const v_char8 TYPE_INT_8  = 'L'; // 8 byte int
  static constexpr const v_char8 TYPE_UINT_8 = '8'; // 8 byte int

  static constexpr const v_char8 TYPE_FLOAT_4 = 'f'; // 4 byte float
  static constexpr const v_char8 TYPE_FLOAT_8 = 'd'; // 8 byte float

  static constexpr const v_char8 CONTROL_MAP_BEGIN = '{';
  static constexpr const v_char8 CONTROL_ARRAY_BEGIN = '[';
  static constexpr const v_char8 CONTROL_SECTION_END = ')'; // end of a map or end of an array

public:
  typedef oatpp::data::stream::ConsistentOutputStream ConsistentOutputStream;
  typedef oatpp::data::share::StringKeyLabel StringKeyLabel;
public:

  enum BO_TYPE : v_int32 {
    LITTLE = 1,
    NETWORK = 2
  };

  union BO_CHECK {
    v_uint64 i64;
    v_float64 f64;
    v_char8 bytes[8];
  };

  static BO_TYPE detectIntBO();
  static BO_TYPE detectFloatBO();

public:
  static BO_TYPE MY_INT_BO;
  static BO_TYPE MY_FLOAT_BO;
public:

  static oatpp::String readCString(oatpp::parser::Caret& caret);
  static void writeCString(ConsistentOutputStream* stream, const oatpp::String& data);

  static v_int8 readInt8(oatpp::parser::Caret& caret);
  static void writeInt8(ConsistentOutputStream* stream, v_int8 value);

  static v_int16 readInt16(oatpp::parser::Caret& caret, BO_TYPE readBO);
  static void writeInt16(ConsistentOutputStream* stream, v_int16 value, BO_TYPE writeBO);

  static v_int32 readInt32(oatpp::parser::Caret& caret, BO_TYPE readBO);
  static void writeInt32(ConsistentOutputStream* stream, v_int32 value, BO_TYPE writeBO);

  static v_int64 readInt64(oatpp::parser::Caret& caret, BO_TYPE readBO);
  static void writeInt64(ConsistentOutputStream* stream, v_int64 value, BO_TYPE writeBO);

  static v_float32 readFloat32(oatpp::parser::Caret& caret, BO_TYPE readBO);
  static void writeFloat32(ConsistentOutputStream* stream, v_float32 value, BO_TYPE writeBO);

  static v_float64 readFloat64(oatpp::parser::Caret& caret, BO_TYPE readBO);
  static void writeFloat64(ConsistentOutputStream* stream, v_float64 value, BO_TYPE writeBO);

};

}}

#endif //OATPP_BOB_UTILS_HPP
