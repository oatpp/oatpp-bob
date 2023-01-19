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


#ifndef OATPP_BOB_OBJECTMAPPER_HPP
#define OATPP_BOB_OBJECTMAPPER_HPP

#include "./Serializer.hpp"
#include "./Deserializer.hpp"

#include "oatpp/core/data/mapping/ObjectMapper.hpp"

namespace oatpp { namespace bob {


class ObjectMapper : public oatpp::base::Countable, public oatpp::data::mapping::ObjectMapper {
private:
  static Info& getMapperInfo() {
    static Info info("application/oatpp-bob");
    return info;
  }
private:
  std::shared_ptr<Serializer> m_serializer;
  std::shared_ptr<Deserializer> m_deserializer;
public:

  ObjectMapper(const std::shared_ptr<Serializer::Config>& serializerConfig,
               const std::shared_ptr<Deserializer::Config>& deserializerConfig);


  ObjectMapper(const std::shared_ptr<Serializer>& serializer = std::make_shared<Serializer>(),
               const std::shared_ptr<Deserializer>& deserializer = std::make_shared<Deserializer>());
public:


  static std::shared_ptr<ObjectMapper>
  createShared(const std::shared_ptr<Serializer::Config>& serializerConfig,
               const std::shared_ptr<Deserializer::Config>& deserializerConfig);


  static std::shared_ptr<ObjectMapper>
  createShared(const std::shared_ptr<Serializer>& serializer = std::make_shared<Serializer>(),
               const std::shared_ptr<Deserializer>& deserializer = std::make_shared<Deserializer>());


  void write(oatpp::data::stream::ConsistentOutputStream* stream, const oatpp::Void& variant) const override;

  oatpp::Void read(oatpp::parser::Caret& caret, const oatpp::data::mapping::type::Type* const type) const override;


  std::shared_ptr<Serializer> getSerializer();

  std::shared_ptr<Deserializer> getDeserializer();
  
};
  
}}

#endif /* OATPP_BOB_OBJECTMAPPER_HPP */
