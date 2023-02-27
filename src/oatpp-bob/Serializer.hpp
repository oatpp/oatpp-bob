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


#ifndef OATPP_BOB_SERIALIZER_HPP
#define OATPP_BOB_SERIALIZER_HPP


#include "oatpp/core/data/stream/Stream.hpp"
#include "oatpp/core/Types.hpp"


namespace oatpp { namespace bob {

class Serializer {
public:
  typedef oatpp::data::mapping::type::Type Type;
  typedef oatpp::data::mapping::type::BaseObject::Property Property;
  typedef oatpp::data::mapping::type::BaseObject::Properties Properties;
  typedef oatpp::data::stream::ConsistentOutputStream ConsistentOutputStream;

  typedef oatpp::String String;
public:
  /**
   * Serializer config.
   */
  class Config : public oatpp::base::Countable {
  public:
    /**
     * Constructor.
     */
    Config()
    {}
  public:

    /**
     * Create shared config.
     * @return - `std::shared_ptr` to Config.
     */
    static std::shared_ptr<Config> createShared(){
      return std::make_shared<Config>();
    }

    /**
     * Include fields with value == nullptr into serialized json.
     * Field will still be included when field-info `required` is set to true and &id:alwaysIncludeRequired is set to true.
     */
    bool includeNullFields = true;

    /**
     * Always include required fields (set in in DTO_FIELD_INFO) even if they are `value == nullptr`
     */
    bool alwaysIncludeRequired = false;

    /**
     * Always include array or map elements, even if their value is `nullptr`.
     */
    bool alwaysIncludeNullCollectionElements = false;

    /**
     * If `true` - insert string `"<unknown-type>"` in json field value in case unknown field found.
     * Fail if `false`.
     * Known types for this serializer are:<br>
     * (String, Int8, Int16, Int32, Int64, Float32, Float64, Boolean, DTOs, List, Fields).
     */
    bool throwOnUnknownTypes = true;

    /**
     * Enable type interpretations.
     */
    std::vector<std::string> enabledInterpretations = {};

    /**
     * Pointer to anything extra.
     */
    void* extra = nullptr;

  };
public:
  typedef void (*SerializerMethod)(Serializer*,
                                   ConsistentOutputStream*,
                                   const oatpp::Void&);

private:
  static void serializeKey(ConsistentOutputStream* stream, const char* data, v_buff_size size);
public:

  static void serializeString(Serializer* serializer, ConsistentOutputStream* stream, const oatpp::Void& polymorph);

  static void serializeBool(Serializer* serializer, ConsistentOutputStream* stream, const oatpp::Void& polymorph);
  static void serializeInt1(Serializer* serializer, ConsistentOutputStream* stream, const oatpp::Void& polymorph);
  static void serializeUInt1(Serializer* serializer, ConsistentOutputStream* stream, const oatpp::Void& polymorph);
  static void serializeInt2(Serializer* serializer, ConsistentOutputStream* stream, const oatpp::Void& polymorph);
  static void serializeUInt2(Serializer* serializer, ConsistentOutputStream* stream, const oatpp::Void& polymorph);
  static void serializeInt4(Serializer* serializer, ConsistentOutputStream* stream, const oatpp::Void& polymorph);
  static void serializeUInt4(Serializer* serializer, ConsistentOutputStream* stream, const oatpp::Void& polymorph);
  static void serializeInt8(Serializer* serializer, ConsistentOutputStream* stream, const oatpp::Void& polymorph);
  static void serializeUInt8(Serializer* serializer, ConsistentOutputStream* stream, const oatpp::Void& polymorph);

  static void serializeFloat4(Serializer* serializer, ConsistentOutputStream* stream, const oatpp::Void& polymorph);
  static void serializeFloat8(Serializer* serializer, ConsistentOutputStream* stream, const oatpp::Void& polymorph);

  static void serializeAny(Serializer* serializer, ConsistentOutputStream* stream, const oatpp::Void& polymorph);
  static void serializeEnum(Serializer* serializer, ConsistentOutputStream* stream, const oatpp::Void& polymorph);

  static void serializeCollection(Serializer* serializer, ConsistentOutputStream* stream, const oatpp::Void& polymorph);
  static void serializeMap(Serializer* serializer, ConsistentOutputStream* stream, const oatpp::Void& polymorph);
  static void serializeObject(Serializer* serializer, ConsistentOutputStream* stream, const oatpp::Void& polymorph);

private:
  void serialize(ConsistentOutputStream* stream, const oatpp::Void& polymorph);
private:
  std::shared_ptr<Config> m_config;
  std::vector<SerializerMethod> m_methods;
public:

  Serializer(const std::shared_ptr<Config>& config = std::make_shared<Config>());

  void setSerializerMethod(const oatpp::data::mapping::type::ClassId& classId, SerializerMethod method);

  void serializeToStream(ConsistentOutputStream* stream, const oatpp::Void& polymorph);

  const std::shared_ptr<Config>& getConfig();

};

}}

#endif /* OATPP_BOB_SERIALIZER_HPP */
