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


#include "ObjectMapperTest.hpp"

#include "oatpp-bob/ObjectMapper.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"

#include "oatpp/core/macro/codegen.hpp"

namespace oatpp { namespace bob { namespace test {

namespace {

#include OATPP_CODEGEN_BEGIN(DTO)

class TestDto1 : public oatpp::DTO {

  DTO_INIT(TestDto1, DTO)

  DTO_FIELD(String, valueStr);

};

class TestDto2 : public oatpp::DTO {

  DTO_INIT(TestDto2, DTO)

  DTO_FIELD(Int32, valueInt32);

};

class TestDto3 : public oatpp::DTO {

  DTO_INIT(TestDto3, DTO)

  DTO_FIELD(Vector<Int32>, values);

};

class PolymorphicDto : public oatpp::DTO {

  DTO_INIT(PolymorphicDto, DTO)

  DTO_FIELD(Int32, type);
  DTO_FIELD(Any, obj);

  DTO_FIELD_TYPE_SELECTOR(obj) {
    if(type == nullptr) return Void::Class::getType();
    switch (type) {
      case 1: return Object<TestDto1>::Class::getType();
      case 2: return Object<TestDto2>::Class::getType();
      case 3: return Object<TestDto3>::Class::getType();
    }
    return Void::Class::getType();
  }


};

#include OATPP_CODEGEN_END(DTO)

}

void ObjectMapperTest::onRun() {

  oatpp::bob::ObjectMapper bobMapper;
  oatpp::parser::json::mapping::ObjectMapper jsonMapper;

  auto dto1 = TestDto1::createShared();
  dto1->valueStr = "this is a string value";

  auto dto2 = TestDto2::createShared();
  dto2->valueInt32 = 32;

  auto dto3 = TestDto3::createShared();
  dto3->values = {1, 2, 3, 4, 5};

  {
    auto poly = PolymorphicDto::createShared();
    poly->type = 1;
    poly->obj = dto1;
    auto bob = bobMapper.writeToString(poly);
    auto clone = bobMapper.readFromString<oatpp::Object<PolymorphicDto>>(bob);
    auto json = jsonMapper.writeToString(clone);
    OATPP_LOGD(TAG, "json='%s'", json->c_str())
    auto obj = poly->obj.retrieve<oatpp::Object<TestDto1>>();
    OATPP_ASSERT(obj->valueStr == dto1->valueStr);
  }

  {
    auto poly = PolymorphicDto::createShared();
    poly->type = 2;
    poly->obj = dto2;
    auto bob = bobMapper.writeToString(poly);
    auto clone = bobMapper.readFromString<oatpp::Object<PolymorphicDto>>(bob);
    auto json = jsonMapper.writeToString(clone);
    OATPP_LOGD(TAG, "json='%s'", json->c_str())
    auto obj = poly->obj.retrieve<oatpp::Object<TestDto2>>();
    OATPP_ASSERT(obj->valueInt32 == dto2->valueInt32);
  }

  {
    auto poly = PolymorphicDto::createShared();
    poly->type = 3;
    poly->obj = dto3;
    auto bob = bobMapper.writeToString(poly);
    auto clone = bobMapper.readFromString<oatpp::Object<PolymorphicDto>>(bob);
    auto json = jsonMapper.writeToString(clone);
    OATPP_LOGD(TAG, "json='%s'", json->c_str())
    auto obj = poly->obj.retrieve<oatpp::Object<TestDto3>>();
    OATPP_ASSERT(obj->values->size() == dto3->values->size())
    for(v_int32 i = 0; i < obj->values->size(); i++) {
      OATPP_ASSERT(obj->values[i] == dto3->values[i]);
    }
  }

  {
    oatpp::String bob("{key\0s\5value)", 13);
    auto obj = bobMapper.readFromString<oatpp::Any>(bob);
    auto json = jsonMapper.writeToString(obj);
    OATPP_LOGD(TAG, "json='%s'", json->c_str())
  }

  {
    oatpp::String bob("{key1\0s\6value1key2\0b\5)", 22);
    auto obj = bobMapper.readFromString<oatpp::Any>(bob);
    auto json = jsonMapper.writeToString(obj);
    OATPP_LOGD(TAG, "json='%s'", json->c_str())
  }

}

}}}
