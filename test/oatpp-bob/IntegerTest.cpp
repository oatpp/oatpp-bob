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


#include "IntegerTest.hpp"

#include "oatpp-bob/ObjectMapper.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"

namespace oatpp { namespace bob { namespace test {

void IntegerTest::onRun() {

  oatpp::bob::ObjectMapper binMapper;
  oatpp::parser::json::mapping::ObjectMapper jsonMapper;

  {
    v_int8 val1 = std::numeric_limits<v_int8>::max();
    OATPP_LOGD(TAG, "Test: Int8. Val=%d", val1)
    auto bob = binMapper.writeToString(oatpp::Int8(val1));
    {
      auto val2 = binMapper.readFromString<oatpp::Int8>(bob);
      OATPP_ASSERT(val1 == val2 && "Int8 from Int8")
    }
    {
      auto val2 = binMapper.readFromString<oatpp::UInt8>(bob);
      OATPP_ASSERT(val1 == val2 && "UInt8 from Int8")
    }
    {
      auto val2 = binMapper.readFromString<oatpp::Int16>(bob);
      OATPP_ASSERT(val1 == val2 && "Int16 from Int8")
    }
    {
      auto val2 = binMapper.readFromString<oatpp::UInt16>(bob);
      OATPP_ASSERT(val1 == val2 && "UInt16 from Int8")
    }
    {
      auto val2 = binMapper.readFromString<oatpp::Int32>(bob);
      OATPP_ASSERT(val1 == val2 && "Int32 from Int8")
    }
    {
      auto val2 = binMapper.readFromString<oatpp::UInt32>(bob);
      OATPP_ASSERT(val1 == val2 && "UInt32 from Int8")
    }
    {
      auto val2 = binMapper.readFromString<oatpp::Int64>(bob);
      OATPP_ASSERT(val1 == val2 && "Int64 from Int8")
    }
    {
      auto val2 = binMapper.readFromString<oatpp::UInt64>(bob);
      OATPP_ASSERT(val1 == val2 && "UInt64 from Int8")
    }
  }

  {
    v_uint8 val1 = std::numeric_limits<v_uint8>::max();
    OATPP_LOGD(TAG, "Test: UInt8. Val=%d", val1)
    auto bob = binMapper.writeToString(oatpp::UInt8(val1));
    {
      auto val2 = binMapper.readFromString<oatpp::Int8>(bob);
      OATPP_ASSERT(val1 != val2 && "Int8 from Int8")
    }
    {
      auto val2 = binMapper.readFromString<oatpp::UInt8>(bob);
      OATPP_ASSERT(val1 == val2 && "UInt8 from Int8")
    }
    {
      auto val2 = binMapper.readFromString<oatpp::Int16>(bob);
      OATPP_ASSERT(val1 == val2 && "Int16 from Int8")
    }
    {
      auto val2 = binMapper.readFromString<oatpp::UInt16>(bob);
      OATPP_ASSERT(val1 == val2 && "UInt16 from Int8")
    }
    {
      auto val2 = binMapper.readFromString<oatpp::Int32>(bob);
      OATPP_ASSERT(val1 == val2 && "Int32 from Int8")
    }
    {
      auto val2 = binMapper.readFromString<oatpp::UInt32>(bob);
      OATPP_ASSERT(val1 == val2 && "UInt32 from Int8")
    }
    {
      auto val2 = binMapper.readFromString<oatpp::Int64>(bob);
      OATPP_ASSERT(val1 == val2 && "Int64 from Int8")
    }
    {
      auto val2 = binMapper.readFromString<oatpp::UInt64>(bob);
      OATPP_ASSERT(val1 == val2 && "UInt64 from Int8")
    }
  }

  {
    v_int16 val1 = std::numeric_limits<v_int16>::max();
    OATPP_LOGD(TAG, "Test: Int16. Val=%d", val1)
    auto bob = binMapper.writeToString(oatpp::Int16(val1));
    {
      auto val2 = binMapper.readFromString<oatpp::Int8>(bob);
      OATPP_ASSERT(val1 != val2 && "Int8 from Int16")
    }
    {
      auto val2 = binMapper.readFromString<oatpp::UInt8>(bob);
      OATPP_ASSERT(val1 != val2 && "UInt8 from Int16")
    }
    {
      auto val2 = binMapper.readFromString<oatpp::Int16>(bob);
      OATPP_ASSERT(val1 == val2 && "Int16 from Int16")
    }
    {
      auto val2 = binMapper.readFromString<oatpp::UInt16>(bob);
      OATPP_ASSERT(val1 == val2 && "UInt16 from Int16")
    }
    {
      auto val2 = binMapper.readFromString<oatpp::Int32>(bob);
      OATPP_ASSERT(val1 == val2 && "Int32 from Int16")
    }
    {
      auto val2 = binMapper.readFromString<oatpp::UInt32>(bob);
      OATPP_ASSERT(val1 == val2 && "UInt32 from Int16")
    }
    {
      auto val2 = binMapper.readFromString<oatpp::Int64>(bob);
      OATPP_ASSERT(val1 == val2 && "Int64 from Int16")
    }
    {
      auto val2 = binMapper.readFromString<oatpp::UInt64>(bob);
      OATPP_ASSERT(val1 == val2 && "UInt64 from Int16")
    }
  }

  {
    v_uint16 val1 = std::numeric_limits<v_uint16>::max();
    OATPP_LOGD(TAG, "Test: UInt16. Val=%d", val1)
    auto bob = binMapper.writeToString(oatpp::UInt16(val1));
    {
      auto val2 = binMapper.readFromString<oatpp::Int8>(bob);
      OATPP_ASSERT(val1 != val2 && "Int8 from UInt16")
    }
    {
      auto val2 = binMapper.readFromString<oatpp::UInt8>(bob);
      OATPP_ASSERT(val1 != val2 && "UInt8 from UInt16")
    }
    {
      auto val2 = binMapper.readFromString<oatpp::Int16>(bob);
      OATPP_ASSERT(val1 != val2 && "Int16 from UInt16")
    }
    {
      auto val2 = binMapper.readFromString<oatpp::UInt16>(bob);
      OATPP_ASSERT(val1 == val2 && "UInt16 from UInt16")
    }
    {
      auto val2 = binMapper.readFromString<oatpp::Int32>(bob);
      OATPP_LOGD(TAG, "%d, %d", val1, *val2)
      OATPP_ASSERT(val1 == val2 && "Int32 from UInt16")
    }
    {
      auto val2 = binMapper.readFromString<oatpp::UInt32>(bob);
      OATPP_ASSERT(val1 == val2 && "UInt32 from UInt16")
    }
    {
      auto val2 = binMapper.readFromString<oatpp::Int64>(bob);
      OATPP_ASSERT(val1 == val2 && "Int64 from UInt16")
    }
    {
      auto val2 = binMapper.readFromString<oatpp::UInt64>(bob);
      OATPP_ASSERT(val1 == val2 && "UInt64 from UInt16")
    }
  }

  {
    v_int32 val1 = std::numeric_limits<v_int32>::max();
    OATPP_LOGD(TAG, "Test: Int32. Val=%d", val1)
    auto bob = binMapper.writeToString(oatpp::Int32(val1));
    {
      auto val2 = binMapper.readFromString<oatpp::Int8>(bob);
      OATPP_ASSERT(val1 != val2 && "Int8 from Int32")
    }
    {
      auto val2 = binMapper.readFromString<oatpp::UInt8>(bob);
      OATPP_ASSERT(val1 != val2 && "UInt8 from Int32")
    }
    {
      auto val2 = binMapper.readFromString<oatpp::Int16>(bob);
      OATPP_ASSERT(val1 != val2 && "Int16 from Int32")
    }
    {
      auto val2 = binMapper.readFromString<oatpp::UInt16>(bob);
      OATPP_ASSERT(val1 != val2 && "UInt16 from Int32")
    }
    {
      auto val2 = binMapper.readFromString<oatpp::Int32>(bob);
      OATPP_ASSERT(val1 == val2 && "Int32 from Int32")
    }
    {
      auto val2 = binMapper.readFromString<oatpp::UInt32>(bob);
      OATPP_ASSERT(val1 == val2 && "UInt32 from Int32")
    }
    {
      auto val2 = binMapper.readFromString<oatpp::Int64>(bob);
      OATPP_ASSERT(val1 == val2 && "Int64 from Int32")
    }
    {
      auto val2 = binMapper.readFromString<oatpp::UInt64>(bob);
      OATPP_ASSERT(val1 == val2 && "UInt64 from Int32")
    }
  }

  {
    v_uint32 val1 = std::numeric_limits<v_uint32>::max();
    OATPP_LOGD(TAG, "Test: UInt32. Val=%lld", val1)
    auto bob = binMapper.writeToString(oatpp::UInt32(val1));
    {
      auto val2 = binMapper.readFromString<oatpp::Int8>(bob);
      OATPP_ASSERT((v_int64)val1 != (v_int64) val2 && "Int8 from UInt32")
    }
    {
      auto val2 = binMapper.readFromString<oatpp::UInt8>(bob);
      OATPP_ASSERT((v_int64)val1 != (v_int64)val2 && "UInt8 from UInt32")
    }
    {
      auto val2 = binMapper.readFromString<oatpp::Int16>(bob);
      OATPP_ASSERT((v_int64)val1 != (v_int64)val2 && "Int16 from UInt32")
    }
    {
      auto val2 = binMapper.readFromString<oatpp::UInt16>(bob);
      OATPP_ASSERT((v_int64)val1 != (v_int64)val2 && "UInt16 from UInt32")
    }
    {
      auto val2 = binMapper.readFromString<oatpp::Int32>(bob);
      OATPP_ASSERT((v_int64)val1 != (v_int64)val2 && "Int32 from UInt32")
    }
    {
      auto val2 = binMapper.readFromString<oatpp::UInt32>(bob);
      OATPP_ASSERT(val1 == val2 && "UInt32 from UInt32")
    }
    {
      auto val2 = binMapper.readFromString<oatpp::Int64>(bob);
      OATPP_ASSERT(val1 == val2 && "Int64 from UInt32")
    }
    {
      auto val2 = binMapper.readFromString<oatpp::UInt64>(bob);
      OATPP_ASSERT(val1 == val2 && "UInt64 from UInt32")
    }
  }

  {
    v_int64 val1 = std::numeric_limits<v_int64>::max();
    OATPP_LOGD(TAG, "Test: Int64. Val=%lld", val1)
    auto bob = binMapper.writeToString(oatpp::Int64(val1));
    {
      auto val2 = binMapper.readFromString<oatpp::Int8>(bob);
      OATPP_ASSERT((v_int64)val1 != (v_int64) val2 && "Int8 from Int64")
    }
    {
      auto val2 = binMapper.readFromString<oatpp::UInt8>(bob);
      OATPP_ASSERT((v_int64)val1 != (v_int64)val2 && "UInt8 from Int64")
    }
    {
      auto val2 = binMapper.readFromString<oatpp::Int16>(bob);
      OATPP_ASSERT((v_int64)val1 != (v_int64)val2 && "Int16 from Int64")
    }
    {
      auto val2 = binMapper.readFromString<oatpp::UInt16>(bob);
      OATPP_ASSERT((v_int64)val1 != (v_int64)val2 && "UInt16 from Int64")
    }
    {
      auto val2 = binMapper.readFromString<oatpp::Int32>(bob);
      OATPP_ASSERT((v_int64)val1 != (v_int64)val2 && "Int32 from Int64")
    }
    {
      auto val2 = binMapper.readFromString<oatpp::UInt32>(bob);
      OATPP_ASSERT((v_int64)val1 != (v_int64)val2 && "UInt32 from Int64")
    }
    {
      auto val2 = binMapper.readFromString<oatpp::Int64>(bob);
      OATPP_ASSERT(val1 == val2 && "Int64 from Int64")
    }
    {
      auto val2 = binMapper.readFromString<oatpp::UInt64>(bob);
      OATPP_ASSERT(val1 == val2 && "UInt64 from Int64")
    }
  }

  {
    v_uint64 val1 = (v_uint64) -0x1011121314151617;
    OATPP_LOGD(TAG, "Test: UInt64. Val=%llu", val1)
    auto bob = binMapper.writeToString(oatpp::UInt64(val1));
    {
      auto val2 = binMapper.readFromString<oatpp::Int8>(bob);
      OATPP_ASSERT(val1 != val2 && "Int8 from UInt64")
    }
    {
      auto val2 = binMapper.readFromString<oatpp::UInt8>(bob);
      OATPP_ASSERT(val1 != val2 && "UInt8 from UInt64")
    }
    {
      auto val2 = binMapper.readFromString<oatpp::Int16>(bob);
      OATPP_ASSERT(val1 != val2 && "Int16 from UInt64")
    }
    {
      auto val2 = binMapper.readFromString<oatpp::UInt16>(bob);
      OATPP_ASSERT(val1 != val2 && "UInt16 from UInt64")
    }
    {
      auto val2 = binMapper.readFromString<oatpp::Int32>(bob);
      OATPP_ASSERT(val1 != val2 && "Int32 from UInt64")
    }
    {
      auto val2 = binMapper.readFromString<oatpp::UInt32>(bob);
      OATPP_ASSERT(val1 != val2 && "UInt32 from UInt64")
    }
    {
      auto val2 = binMapper.readFromString<oatpp::Int64>(bob);
      OATPP_ASSERT(val1 == val2 && "Int64 from UInt64")
    }
    {
      auto val2 = binMapper.readFromString<oatpp::UInt64>(bob);
      OATPP_ASSERT(val1 == val2 && "UInt64 from UInt64")
    }
  }

}


}}}
