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


#include "SkipTest.hpp"

#include "oatpp-bob/ObjectMapper.hpp"

#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "oatpp/core/macro/codegen.hpp"

namespace oatpp { namespace bob { namespace test {

namespace {

#include OATPP_CODEGEN_BEGIN(DTO)

template<typename T>
class TestDto : public oatpp::DTO {

  DTO_INIT(TestDto, DTO)

  DTO_FIELD(T, unknownField1);
  DTO_FIELD(String, check1);
  DTO_FIELD(T, unknownField2);
  DTO_FIELD(String, check2);
  DTO_FIELD(T, unknownField3);
  DTO_FIELD(String, check3);

};

class CheckDto : public oatpp::DTO {

  DTO_INIT(CheckDto, DTO)

  DTO_FIELD(String, check1);
  DTO_FIELD(String, check2);
  DTO_FIELD(String, check3);

};


#include OATPP_CODEGEN_END(DTO)

}

namespace {

template<typename T, typename V>
void checkString(const char* TAG, V param,
                 const oatpp::bob::ObjectMapper& mapper,
                 const oatpp::bob::ObjectMapper& debugMapper,
                 const oatpp::parser::json::mapping::ObjectMapper& jsonMapper)
{

  auto dto1 = TestDto<T>::createShared();

  dto1->unknownField1 = T(param);
  dto1->unknownField2 = T(param);
  dto1->unknownField3 = T(param);

  dto1->check1 = "check-1";
  dto1->check2 = "check-2";
  dto1->check3 = "check-3";

  auto bob = mapper.writeToString(dto1);
  auto debugDto = debugMapper.readFromString<oatpp::Any>(bob);
  auto debugJson = jsonMapper.writeToString(debugDto);
  OATPP_LOGD(TAG, "debug='%s'", debugJson->c_str())
  auto dto2 = mapper.readFromString<oatpp::Object<CheckDto>>(bob);

  OATPP_ASSERT(dto2->check1 == "check-1")
  OATPP_ASSERT(dto2->check2 == "check-2")
  OATPP_ASSERT(dto2->check3 == "check-3")
}

}

void SkipTest::onRun() {

  oatpp::bob::ObjectMapper mapper;
  oatpp::bob::ObjectMapper mapperDebug;
  mapperDebug.getDeserializer()->useDebugStrings(true);

  oatpp::parser::json::mapping::ObjectMapper jsonMapper;

  {
    OATPP_LOGD(TAG, "Skip null")
    checkString<oatpp::String>(TAG, nullptr, mapper, mapperDebug, jsonMapper);
  }

  {
    OATPP_LOGD(TAG, "Skip True")
    checkString<oatpp::Boolean>(TAG, true, mapper, mapperDebug, jsonMapper);
  }

  {
    OATPP_LOGD(TAG, "Skip false")
    checkString<oatpp::Boolean>(TAG, false, mapper, mapperDebug, jsonMapper);
  }

  {
    OATPP_LOGD(TAG, "Skip Int8")
    checkString<oatpp::Int8>(TAG, 100, mapper, mapperDebug, jsonMapper);
  }

  {
    OATPP_LOGD(TAG, "Skip UInt8")
    checkString<oatpp::UInt8>(TAG, 100, mapper, mapperDebug, jsonMapper);
  }

  {
    OATPP_LOGD(TAG, "Skip Int16")
    checkString<oatpp::Int16>(TAG, 100, mapper, mapperDebug, jsonMapper);
  }

  {
    OATPP_LOGD(TAG, "Skip UInt16")
    checkString<oatpp::UInt16>(TAG, 100, mapper, mapperDebug, jsonMapper);
  }

  {
    OATPP_LOGD(TAG, "Skip Int32")
    checkString<oatpp::Int32>(TAG, 100, mapper, mapperDebug, jsonMapper);
  }

  {
    OATPP_LOGD(TAG, "Skip UInt32")
    checkString<oatpp::UInt32>(TAG, 100, mapper, mapperDebug, jsonMapper);
  }

  {
    OATPP_LOGD(TAG, "Skip Int64")
    checkString<oatpp::Int64>(TAG, 100, mapper, mapperDebug, jsonMapper);
  }

  {
    OATPP_LOGD(TAG, "Skip UInt64")
    checkString<oatpp::UInt64>(TAG, 100, mapper, mapperDebug, jsonMapper);
  }

  {
    OATPP_LOGD(TAG, "Skip Float32")
    checkString<oatpp::Float32>(TAG, 100, mapper, mapperDebug, jsonMapper);
  }

  {
    OATPP_LOGD(TAG, "Skip Float64")
    checkString<oatpp::Float64>(TAG, 100, mapper, mapperDebug, jsonMapper);
  }

  {
    OATPP_LOGD(TAG, "Skip String1")
    checkString<oatpp::String>(TAG, 120, mapper, mapperDebug, jsonMapper);
  }

  {
    OATPP_LOGD(TAG, "Skip String2")
    checkString<oatpp::String>(TAG, 65530, mapper, mapperDebug, jsonMapper);
  }

  {
    OATPP_LOGD(TAG, "Skip String4")
    checkString<oatpp::String>(TAG, 66000, mapper, mapperDebug, jsonMapper);
  }

  {
    OATPP_LOGD(TAG, "Skip Vector")
    checkString<oatpp::Vector<oatpp::String>>(TAG, oatpp::Vector<oatpp::String>({"A", nullptr, "C"}), mapper, mapperDebug, jsonMapper);
  }

  {
    OATPP_LOGD(TAG, "Skip Map")
    checkString<oatpp::Fields<oatpp::String>>(TAG, oatpp::Fields<oatpp::String>({{"A", "A"}, {"B", nullptr}, {"C", "C"}}), mapper, mapperDebug, jsonMapper);
  }

}

}}}