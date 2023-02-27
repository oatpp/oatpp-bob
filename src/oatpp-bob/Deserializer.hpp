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

#ifndef OATPP_BOB_DESERIALIZER_HPP
#define OATPP_BOB_DESERIALIZER_HPP

#include "./Utils.hpp"
#include "oatpp/core/parser/Caret.hpp"
#include "oatpp/core/Types.hpp"

#include <vector>

namespace oatpp { namespace bob {

/**
 * Json Deserializer.
 * Deserialize oatpp DTO object from json. See [Data Transfer Object(DTO) component](https://oatpp.io/docs/components/dto/).
 */
class Deserializer {
public:
  typedef oatpp::data::mapping::type::Type Type;
  typedef oatpp::data::mapping::type::BaseObject::Property Property;
  typedef oatpp::data::mapping::type::BaseObject::Properties Properties;

  typedef oatpp::String String;

public:

  /**
 * "'{' - expected"
 */
  static constexpr v_int32 ERROR_CODE_OBJECT_SCOPE_OPEN = 1;

  /**
   * "'}' - expected"
   */
  static constexpr v_int32 ERROR_CODE_OBJECT_SCOPE_CLOSE = 2;

  /**
   * "Unknown field"
   */
  static constexpr v_int32 ERROR_CODE_OBJECT_SCOPE_UNKNOWN_FIELD = 3;

  /**
   * "':' - expected"
   */
  static constexpr v_int32 ERROR_CODE_OBJECT_SCOPE_COLON_MISSING = 4;

  /**
   * "'[' - expected"
   */
  static constexpr v_int32 ERROR_CODE_ARRAY_SCOPE_OPEN = 5;

  /**
   * "']' - expected"
   */
  static constexpr v_int32 ERROR_CODE_ARRAY_SCOPE_CLOSE = 6;

  /**
   * "'true' or 'false' - expected"
   */
  static constexpr v_int32 ERROR_CODE_VALUE_BOOLEAN = 7;

public:

  /**
   * Deserializer config.
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
     * Create shared Config.
     * @return - `std::shared_ptr` to Config.
     */
    static std::shared_ptr<Config> createShared(){
      return std::make_shared<Config>();
    }

    /**
     * Do not fail if unknown field is found in json.
     * "unknown field" is the one which is not present in DTO object class.
     */
    bool allowUnknownFields = true;

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
  typedef oatpp::Void (*DeserializerMethod)(Deserializer*, oatpp::parser::Caret&, const Type* const);
private:
  static void skipKey(oatpp::parser::Caret& caret);
  static void skipString(oatpp::parser::Caret& caret);
  static void skipMap(oatpp::parser::Caret& caret);
  static void skipArray(oatpp::parser::Caret& caret);
  static void skipValue(oatpp::parser::Caret& caret);
private:
  static const Type* guessType(oatpp::parser::Caret& caret);
public:

  static oatpp::Void deserializeInt8(Deserializer* deserializer, oatpp::parser::Caret& caret, const Type* const type);
  static oatpp::Void deserializeUInt8(Deserializer* deserializer, oatpp::parser::Caret& caret, const Type* const type);
  static oatpp::Void deserializeInt16(Deserializer* deserializer, oatpp::parser::Caret& caret, const Type* const type);
  static oatpp::Void deserializeUInt16(Deserializer* deserializer, oatpp::parser::Caret& caret, const Type* const type);
  static oatpp::Void deserializeInt32(Deserializer* deserializer, oatpp::parser::Caret& caret, const Type* const type);
  static oatpp::Void deserializeUInt32(Deserializer* deserializer, oatpp::parser::Caret& caret, const Type* const type);
  static oatpp::Void deserializeInt64(Deserializer* deserializer, oatpp::parser::Caret& caret, const Type* const type);
  static oatpp::Void deserializeUInt64(Deserializer* deserializer, oatpp::parser::Caret& caret, const Type* const type);

  static oatpp::Void deserializeFloat32(Deserializer* deserializer, oatpp::parser::Caret& caret, const Type* const type);
  static oatpp::Void deserializeFloat64(Deserializer* deserializer, oatpp::parser::Caret& caret, const Type* const type);

  static oatpp::Void deserializeBoolean(Deserializer* deserializer, oatpp::parser::Caret& caret, const Type* const type);
  static oatpp::Void deserializeString(Deserializer* deserializer, oatpp::parser::Caret& caret, const Type* const type);
  static oatpp::Void deserializeStringDebug(Deserializer* deserializer, oatpp::parser::Caret& caret, const Type* const type);
  static oatpp::Void deserializeAny(Deserializer* deserializer, oatpp::parser::Caret& caret, const Type* const type);
  static oatpp::Void deserializeEnum(Deserializer* deserializer, oatpp::parser::Caret& caret, const Type* const type);

  static oatpp::Void deserializeCollection(Deserializer* deserializer, oatpp::parser::Caret& caret, const Type* type);
  static oatpp::Void deserializeMap(Deserializer* deserializer, oatpp::parser::Caret& caret, const Type* const type);

  static oatpp::Void deserializeObject(Deserializer* deserializer, oatpp::parser::Caret& caret, const Type* const type);

private:
  std::shared_ptr<Config> m_config;
  std::vector<DeserializerMethod> m_methods;
public:

  /**
   * Constructor.
   * @param config
   */
  Deserializer(const std::shared_ptr<Config>& config = std::make_shared<Config>());

  /**
   * Set deserializer method for type.
   * @param classId - &id:oatpp::data::mapping::type::ClassId;.
   * @param method - `typedef oatpp::Void (*DeserializerMethod)(Deserializer*, parser::Caret&, const Type* const)`.
   */
  void setDeserializerMethod(const oatpp::data::mapping::type::ClassId& classId, DeserializerMethod method);

  /**
   * Deserialize text.
   * @param caret - &id:oatpp::parser::Caret;.
   * @param type - &id:oatpp::data::mapping::type::Type;
   * @return - `oatpp::Void` over deserialized object.
   */
  oatpp::Void deserialize(oatpp::parser::Caret& caret, const Type* const type);

  /**
   * Get deserializer config.
   * @return
   */
  const std::shared_ptr<Config>& getConfig();

  void useDebugStrings(bool debugStrings);

};

}}

#endif /* OATPP_BOB_DESERIALIZER_HPP */
