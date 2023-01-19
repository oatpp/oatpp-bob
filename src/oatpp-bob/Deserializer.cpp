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

#include "Deserializer.hpp"

#include "oatpp/core/data/stream/BufferStream.hpp"
#include "oatpp/core/utils/ConversionUtils.hpp"

namespace oatpp { namespace bob {

Deserializer::Deserializer(const std::shared_ptr<Config>& config)
  : m_config(config)
{

  m_methods.resize(oatpp::data::mapping::type::ClassId::getClassCount(), nullptr);

  setDeserializerMethod(oatpp::data::mapping::type::__class::String::CLASS_ID, &Deserializer::deserializeString);
  setDeserializerMethod(oatpp::data::mapping::type::__class::Any::CLASS_ID, &Deserializer::deserializeAny);

  setDeserializerMethod(oatpp::data::mapping::type::__class::Int8::CLASS_ID, &Deserializer::deserializeInt8);
  setDeserializerMethod(oatpp::data::mapping::type::__class::UInt8::CLASS_ID, &Deserializer::deserializeUInt8);

  setDeserializerMethod(oatpp::data::mapping::type::__class::Int16::CLASS_ID, &Deserializer::deserializeInt16);
  setDeserializerMethod(oatpp::data::mapping::type::__class::UInt16::CLASS_ID, &Deserializer::deserializeUInt16);

  setDeserializerMethod(oatpp::data::mapping::type::__class::Int32::CLASS_ID, &Deserializer::deserializeInt32);
  setDeserializerMethod(oatpp::data::mapping::type::__class::UInt32::CLASS_ID, &Deserializer::deserializeUInt32);

  setDeserializerMethod(oatpp::data::mapping::type::__class::Int64::CLASS_ID, &Deserializer::deserializeInt64);
  setDeserializerMethod(oatpp::data::mapping::type::__class::UInt64::CLASS_ID, &Deserializer::deserializeUInt64);

  setDeserializerMethod(oatpp::data::mapping::type::__class::Float32::CLASS_ID, &Deserializer::deserializeFloat32);
  setDeserializerMethod(oatpp::data::mapping::type::__class::Float64::CLASS_ID, &Deserializer::deserializeFloat64);
  setDeserializerMethod(oatpp::data::mapping::type::__class::Boolean::CLASS_ID, &Deserializer::deserializeBoolean);

  setDeserializerMethod(oatpp::data::mapping::type::__class::AbstractObject::CLASS_ID, &Deserializer::deserializeObject);
  setDeserializerMethod(oatpp::data::mapping::type::__class::AbstractEnum::CLASS_ID, &Deserializer::deserializeEnum);

  setDeserializerMethod(oatpp::data::mapping::type::__class::AbstractVector::CLASS_ID, &Deserializer::deserializeCollection);
  setDeserializerMethod(oatpp::data::mapping::type::__class::AbstractList::CLASS_ID, &Deserializer::deserializeCollection);
  setDeserializerMethod(oatpp::data::mapping::type::__class::AbstractUnorderedSet::CLASS_ID, &Deserializer::deserializeCollection);

  setDeserializerMethod(oatpp::data::mapping::type::__class::AbstractPairList::CLASS_ID, &Deserializer::deserializeMap);
  setDeserializerMethod(oatpp::data::mapping::type::__class::AbstractUnorderedMap::CLASS_ID, &Deserializer::deserializeMap);

}

void Deserializer::setDeserializerMethod(const oatpp::data::mapping::type::ClassId& classId, DeserializerMethod method) {
  const v_uint32 id = classId.id;
  if(id >= m_methods.size()) {
    m_methods.resize(id + 1, nullptr);
  }
  m_methods[id] = method;
}

void Deserializer::skipKey(oatpp::parser::Caret& caret) {

    while(!caret.isAtChar(0)) {
      caret.inc();
    }

    if(!caret.canContinueAtChar(0, 1)) {
      caret.setError("[oatpp::bob::Deserializer::skipKey()]: Error. Invalid key.");
    }

}

void Deserializer::skipString(oatpp::parser::Caret& caret) {
  v_int64 size;
  if(caret.canContinueAtChar(Utils::TYPE_STRING_1, 1)) {
    size = (v_uint8) Utils::readInt8(caret);
  } else if(caret.canContinueAtChar(Utils::TYPE_STRING_2, 1)) {
    size = (v_uint16) Utils::readInt16(caret, Utils::BO_TYPE::NETWORK);
  } else if(caret.canContinueAtChar(Utils::TYPE_STRING_4, 1)) {
    size = (v_uint32) Utils::readInt32(caret, Utils::BO_TYPE::NETWORK);
  } else {
    caret.setError("[oatpp::bob::Deserializer::skipString()]: Error. Invalid state.");
  }
  caret.inc(size);
}

void Deserializer::skipMap(oatpp::parser::Caret& caret) {

  if(caret.canContinueAtChar(Utils::CONTROL_MAP_BEGIN, 1)) {

    while (!caret.isAtChar(Utils::CONTROL_SECTION_END) && caret.canContinue()) {

      skipKey(caret);
      if(caret.hasError()) return;

      skipValue(caret);
      if(caret.hasError()) return;

    }

    if(!caret.canContinueAtChar(Utils::CONTROL_SECTION_END, 1)){
      caret.setError("[oatpp::bob::Deserializer::skipMap()]: Error. '}' - expected", ERROR_CODE_OBJECT_SCOPE_CLOSE);
      return;
    }

  } else {
    caret.setError("[oatpp::bob::Deserializer::skipMap()]: Error. '{' - expected", ERROR_CODE_OBJECT_SCOPE_OPEN);
  }

}

void Deserializer::skipArray(oatpp::parser::Caret& caret) {

  if(caret.canContinueAtChar(Utils::CONTROL_ARRAY_BEGIN, 1)) {

    while(!caret.isAtChar(Utils::CONTROL_SECTION_END) && caret.canContinue()) {

      skipValue(caret);
      if(caret.hasError()) return;

    }

    if(!caret.canContinueAtChar(Utils::CONTROL_SECTION_END, 1)){
      caret.setError("[oatpp::bob::Deserializer::skipArray()]: Error. ')' - expected", ERROR_CODE_ARRAY_SCOPE_CLOSE);
      return;
    };

  } else {
    caret.setError("[oatpp::bob::Deserializer::skipArray()]: Error. '[' - expected", ERROR_CODE_ARRAY_SCOPE_OPEN);
    return;
  }

}

void Deserializer::skipValue(oatpp::parser::Caret& caret) {

  v_char8 c = *caret.getCurrData();

  switch (c) {
    case Utils::TYPE_NULL: caret.inc();
      break;
    case Utils::TYPE_STRING_1:
    case Utils::TYPE_STRING_2:
    case Utils::TYPE_STRING_4: skipString(caret);
      break;

    case Utils::CONTROL_MAP_BEGIN: skipMap(caret);
      break;
    case Utils::CONTROL_ARRAY_BEGIN: skipArray(caret);
      break;

    case Utils::TYPE_BOOL_TRUE: caret.inc();
      break;
    case Utils::TYPE_BOOL_FALSE: caret.inc();
      break;

    case Utils::TYPE_INT_1:
    case Utils::TYPE_UINT_1: caret.inc(1 + 1);
      break;
    case Utils::TYPE_INT_2:
    case Utils::TYPE_UINT_2: caret.inc(1 + 2);
      break;
    case Utils::TYPE_INT_4:
    case Utils::TYPE_UINT_4: caret.inc(1 + 4);
      break;
    case Utils::TYPE_INT_8:
    case Utils::TYPE_UINT_8: caret.inc(1 + 8);
      break;

    case Utils::TYPE_FLOAT_4: caret.inc(1 + 4);
      break;
    case Utils::TYPE_FLOAT_8: caret.inc(1 + 8);
      break;

    default:
      caret.setError("[oatpp::bob::Deserializer::skipValue()]: Error. Invalid state.");

  }

}

oatpp::Void Deserializer::deserializeInt8(Deserializer* deserializer, oatpp::parser::Caret& caret, const Type* const type) {

  (void) deserializer;

  v_char8 c = *((p_char8) caret.getCurrData());
  v_int8 value;
  caret.inc();
  switch (c) {

    case Utils::TYPE_NULL:
      return oatpp::Void(type);

    case Utils::TYPE_INT_1:
    case Utils::TYPE_UINT_1: value = (v_int8) Utils::readInt8(caret); break;

    case Utils::TYPE_INT_2:
    case Utils::TYPE_UINT_2: value = (v_int8) Utils::readInt16(caret, Utils::BO_TYPE::NETWORK); break;

    case Utils::TYPE_INT_4:
    case Utils::TYPE_UINT_4: value = (v_int8) Utils::readInt32(caret, Utils::BO_TYPE::NETWORK); break;

    case Utils::TYPE_INT_8:
    case Utils::TYPE_UINT_8: value = (v_int8) Utils::readInt64(caret, Utils::BO_TYPE::NETWORK); break;

    default:
      caret.setError("[oatpp::bob::Deserializer::deserializeString()]: Error. Int8 expected.");
      return nullptr;
  }

  return oatpp::Int8 (value);

}

oatpp::Void Deserializer::deserializeUInt8(Deserializer* deserializer, oatpp::parser::Caret& caret, const Type* const type) {

  (void) deserializer;

  v_char8 c = *((p_char8) caret.getCurrData());
  v_uint8 value;
  caret.inc();
  switch (c) {

    case Utils::TYPE_NULL:
      return oatpp::Void(type);

    case Utils::TYPE_INT_1:
    case Utils::TYPE_UINT_1: value = (v_uint8) Utils::readInt8(caret); break;

    case Utils::TYPE_INT_2:
    case Utils::TYPE_UINT_2: value = (v_uint8) Utils::readInt16(caret, Utils::BO_TYPE::NETWORK); break;

    case Utils::TYPE_INT_4:
    case Utils::TYPE_UINT_4: value = (v_uint8) Utils::readInt32(caret, Utils::BO_TYPE::NETWORK); break;

    case Utils::TYPE_INT_8:
    case Utils::TYPE_UINT_8: value = (v_uint8) Utils::readInt64(caret, Utils::BO_TYPE::NETWORK); break;

    default:
      caret.setError("[oatpp::bob::Deserializer::deserializeString()]: Error. UInt8 expected.");
      return nullptr;
  }

  return oatpp::UInt8 (value);

}

oatpp::Void Deserializer::deserializeInt16(Deserializer* deserializer, oatpp::parser::Caret& caret, const Type* const type) {

  (void) deserializer;

  v_char8 c = *((p_char8) caret.getCurrData());
  v_int16 value;
  caret.inc();
  switch (c) {

    case Utils::TYPE_NULL:
      return oatpp::Void(type);

    case Utils::TYPE_INT_1: value = (v_int8) Utils::readInt8(caret); break;
    case Utils::TYPE_UINT_1: value = (v_uint8) Utils::readInt8(caret); break;

    case Utils::TYPE_INT_2:
    case Utils::TYPE_UINT_2: value = (v_int16) Utils::readInt16(caret, Utils::BO_TYPE::NETWORK); break;

    case Utils::TYPE_INT_4:
    case Utils::TYPE_UINT_4: value = (v_int16) Utils::readInt32(caret, Utils::BO_TYPE::NETWORK); break;

    case Utils::TYPE_INT_8:
    case Utils::TYPE_UINT_8: value = (v_int16) Utils::readInt64(caret, Utils::BO_TYPE::NETWORK); break;

    default:
      caret.setError("[oatpp::bob::Deserializer::deserializeString()]: Error. Int16 expected.");
      return nullptr;
  }

  return oatpp::Int16 (value);

}

oatpp::Void Deserializer::deserializeUInt16(Deserializer* deserializer, oatpp::parser::Caret& caret, const Type* const type) {

  (void) deserializer;

  v_char8 c = *((p_char8) caret.getCurrData());
  v_uint16 value;
  caret.inc();
  switch (c) {

    case Utils::TYPE_NULL:
      return oatpp::Void(type);

    case Utils::TYPE_INT_1:
    case Utils::TYPE_UINT_1: value = (v_uint8) Utils::readInt8(caret); break;

    case Utils::TYPE_INT_2:
    case Utils::TYPE_UINT_2: value = (v_uint16) Utils::readInt16(caret, Utils::BO_TYPE::NETWORK); break;

    case Utils::TYPE_INT_4:
    case Utils::TYPE_UINT_4: value = (v_uint16) Utils::readInt32(caret, Utils::BO_TYPE::NETWORK); break;

    case Utils::TYPE_INT_8:
    case Utils::TYPE_UINT_8: value = (v_uint16) Utils::readInt64(caret, Utils::BO_TYPE::NETWORK); break;

    default:
      caret.setError("[oatpp::bob::Deserializer::deserializeString()]: Error. UInt16 expected.");
      return nullptr;
  }

  return oatpp::UInt16 (value);

}

oatpp::Void Deserializer::deserializeInt32(Deserializer* deserializer, oatpp::parser::Caret& caret, const Type* const type) {

  (void) deserializer;

  v_char8 c = *((p_char8) caret.getCurrData());
  v_int32 value;
  caret.inc();
  switch (c) {

    case Utils::TYPE_NULL:
      return oatpp::Void(type);

    case Utils::TYPE_INT_1: value = (v_int8) Utils::readInt8(caret); break;
    case Utils::TYPE_UINT_1: value = (v_uint8) Utils::readInt8(caret); break;

    case Utils::TYPE_INT_2: value = (v_int16) Utils::readInt16(caret, Utils::BO_TYPE::NETWORK); break;
    case Utils::TYPE_UINT_2: value = (v_uint16) Utils::readInt16(caret, Utils::BO_TYPE::NETWORK); break;

    case Utils::TYPE_INT_4:
    case Utils::TYPE_UINT_4: value = (v_int32) Utils::readInt32(caret, Utils::BO_TYPE::NETWORK); break;

    case Utils::TYPE_INT_8:
    case Utils::TYPE_UINT_8: value = (v_int32) Utils::readInt64(caret, Utils::BO_TYPE::NETWORK); break;

    default:
      caret.setError("[oatpp::bob::Deserializer::deserializeString()]: Error. Int32 expected.");
      return nullptr;
  }

  return oatpp::Int32 (value);

}

oatpp::Void Deserializer::deserializeUInt32(Deserializer* deserializer, oatpp::parser::Caret& caret, const Type* const type) {

  (void) deserializer;

  v_char8 c = *((p_char8) caret.getCurrData());
  v_uint32 value;
  caret.inc();
  switch (c) {

    case Utils::TYPE_NULL:
      return oatpp::Void(type);

    case Utils::TYPE_INT_1:
    case Utils::TYPE_UINT_1: value = (v_uint8) Utils::readInt8(caret); break;

    case Utils::TYPE_INT_2:
    case Utils::TYPE_UINT_2: value = (v_uint16) Utils::readInt16(caret, Utils::BO_TYPE::NETWORK); break;

    case Utils::TYPE_INT_4:
    case Utils::TYPE_UINT_4: value = (v_uint32) Utils::readInt32(caret, Utils::BO_TYPE::NETWORK); break;

    case Utils::TYPE_INT_8:
    case Utils::TYPE_UINT_8: value = (v_uint64) Utils::readInt64(caret, Utils::BO_TYPE::NETWORK); break;

    default:
      caret.setError("[oatpp::bob::Deserializer::deserializeString()]: Error. UInt32 expected.");
      return nullptr;
  }

  return oatpp::UInt32 (value);

}

oatpp::Void Deserializer::deserializeInt64(Deserializer* deserializer, oatpp::parser::Caret& caret, const Type* const type) {

  (void) deserializer;

  v_char8 c = *((p_char8) caret.getCurrData());
  v_int64 value;
  caret.inc();
  switch (c) {

    case Utils::TYPE_NULL:
      return oatpp::Void(type);

    case Utils::TYPE_INT_1: value = (v_int8) Utils::readInt8(caret); break;
    case Utils::TYPE_UINT_1: value = (v_uint8) Utils::readInt8(caret); break;

    case Utils::TYPE_INT_2: value = (v_int16) Utils::readInt16(caret, Utils::BO_TYPE::NETWORK); break;
    case Utils::TYPE_UINT_2: value = (v_uint16) Utils::readInt16(caret, Utils::BO_TYPE::NETWORK); break;

    case Utils::TYPE_INT_4: value = (v_int32) Utils::readInt32(caret, Utils::BO_TYPE::NETWORK); break;
    case Utils::TYPE_UINT_4: value = (v_uint32) Utils::readInt32(caret, Utils::BO_TYPE::NETWORK); break;

    case Utils::TYPE_INT_8:
    case Utils::TYPE_UINT_8: value = (v_int64) Utils::readInt64(caret, Utils::BO_TYPE::NETWORK); break;

    default:
      caret.setError("[oatpp::bob::Deserializer::deserializeString()]: Error. Int64 expected.");
      return nullptr;
  }

  return oatpp::Int64 (value);

}

oatpp::Void Deserializer::deserializeUInt64(Deserializer* deserializer, oatpp::parser::Caret& caret, const Type* const type) {

  (void) deserializer;

  v_char8 c = *((p_char8) caret.getCurrData());
  v_uint64 value;
  caret.inc();
  switch (c) {

    case Utils::TYPE_NULL:
      return oatpp::Void(type);

    case Utils::TYPE_INT_1:
    case Utils::TYPE_UINT_1: value = (v_uint8) Utils::readInt8(caret); break;

    case Utils::TYPE_INT_2:
    case Utils::TYPE_UINT_2: value = (v_uint16) Utils::readInt16(caret, Utils::BO_TYPE::NETWORK); break;

    case Utils::TYPE_INT_4:
    case Utils::TYPE_UINT_4: value = (v_uint32) Utils::readInt32(caret, Utils::BO_TYPE::NETWORK); break;

    case Utils::TYPE_INT_8:
    case Utils::TYPE_UINT_8: value = (v_uint64) Utils::readInt64(caret, Utils::BO_TYPE::NETWORK); break;

    default:
      caret.setError("[oatpp::bob::Deserializer::deserializeString()]: Error. UInt64 expected.");
      return nullptr;
  }

  return oatpp::UInt64 (value);

}


oatpp::Void Deserializer::deserializeFloat32(Deserializer* deserializer, oatpp::parser::Caret& caret, const Type* const type) {

  v_char8 c = *((p_char8) caret.getCurrData());
  v_float32 value;
  caret.inc();
  switch (c) {

    case Utils::TYPE_NULL:
      return oatpp::Void(type);

    case Utils::TYPE_FLOAT_4:
      value = Utils::readFloat32(caret, Utils::BO_TYPE::NETWORK);
      break;

    case Utils::TYPE_FLOAT_8:
      value = (v_float32) Utils::readFloat64(caret, Utils::BO_TYPE::NETWORK);
      break;

    default:
      caret.setError("[oatpp::bob::Deserializer::deserializeString()]: Error. Float32 expected.");
      return nullptr;
  }

  return oatpp::Float32(value);

}

oatpp::Void Deserializer::deserializeFloat64(Deserializer* deserializer, oatpp::parser::Caret& caret, const Type* const type) {

  (void) deserializer;

  v_char8 c = *((p_char8) caret.getCurrData());
  v_float64 value;
  caret.inc();
  switch (c) {

    case Utils::TYPE_NULL:
      return oatpp::Void(type);

    case Utils::TYPE_FLOAT_4:
      value = Utils::readFloat32(caret, Utils::BO_TYPE::NETWORK);
      break;

    case Utils::TYPE_FLOAT_8:
      value = Utils::readFloat64(caret, Utils::BO_TYPE::NETWORK);
      break;

    default:
      caret.setError("[oatpp::bob::Deserializer::deserializeString()]: Error. Float64 expected.");
      return nullptr;
  }

  return oatpp::Float64(value);

}

oatpp::Void Deserializer::deserializeBoolean(Deserializer* deserializer, oatpp::parser::Caret& caret, const Type* const type) {

  (void) deserializer;

  if(caret.isAtChar(Utils::TYPE_NULL)) {
    caret.inc();
    return oatpp::Void(type);
  }

  if(caret.isAtChar(Utils::TYPE_BOOL_FALSE)) {
    caret.inc();
    return oatpp::Boolean(false);
  } else if(caret.isAtChar(Utils::TYPE_BOOL_TRUE)) {
    caret.inc();
    return oatpp::Boolean(true);
  } else {
    caret.setError("[oatpp::bob::Deserializer::deserializeString()]: Error. Boolean expected.");
    return nullptr;
  }

  caret.setError("[oatpp::bob::Deserializer::readBooleanValue()]: Error. Boolean type - expected.", ERROR_CODE_VALUE_BOOLEAN);
  return oatpp::Void(oatpp::Boolean::Class::getType());

}

oatpp::Void Deserializer::deserializeString(Deserializer* deserializer, oatpp::parser::Caret& caret, const Type* const type) {

  (void) deserializer;

  if(caret.isAtChar(Utils::TYPE_NULL)) {
    caret.inc();
    return oatpp::Void(type);
  }

  v_int64 size;
  if(caret.isAtChar(Utils::TYPE_STRING_1)) {
    caret.inc();
    size = (v_uint8) Utils::readInt8(caret);
  } else if(caret.isAtChar(Utils::TYPE_STRING_2)) {
    caret.inc();
    size = (v_uint16) Utils::readInt16(caret, Utils::BO_TYPE::NETWORK);
  } else if(caret.isAtChar(Utils::TYPE_STRING_4)) {
    caret.inc();
    size = (v_uint32) Utils::readInt32(caret, Utils::BO_TYPE::NETWORK);
  } else {
    caret.setError("[oatpp::bob::Deserializer::deserializeString()]: Error. String expected.");
    return nullptr;
  }

  if(caret.getDataSize() - caret.getPosition() < size) {
    caret.setError("[oatpp::bob::Deserializer::deserializeString()]: Error. Invalid string size.");
    return oatpp::Void(String::Class::getType());
  }

  oatpp::String result(caret.getCurrData(), size);
  caret.inc(size);
  return result;

}

oatpp::Void Deserializer::deserializeStringDebug(Deserializer* deserializer, oatpp::parser::Caret& caret, const Type* const type) {

  (void) deserializer;

  if(caret.isAtChar(Utils::TYPE_NULL)) {
    caret.inc();
    return oatpp::Void(type);
  }

  oatpp::String typeName = "";

  v_int64 size;
  if(caret.isAtChar(Utils::TYPE_STRING_1)) {
    caret.inc();
    size = (v_uint8) Utils::readInt8(caret);
    typeName = "str1";
  } else if(caret.isAtChar(Utils::TYPE_STRING_2)) {
    caret.inc();
    size = (v_uint16) Utils::readInt16(caret, Utils::BO_TYPE::NETWORK);
    typeName = "str2";
  } else if(caret.isAtChar(Utils::TYPE_STRING_4)) {
    caret.inc();
    size = (v_uint32) Utils::readInt32(caret, Utils::BO_TYPE::NETWORK);
    typeName = "str4";
  } else {
    caret.setError("[oatpp::bob::Deserializer::deserializeString()]: Error. String expected.");
    return nullptr;
  }

  if(caret.getDataSize() - caret.getPosition() < size) {
    caret.setError("[oatpp::bob::Deserializer::deserializeString()]: Error. Invalid string size.");
    return oatpp::Void(String::Class::getType());
  }

  caret.inc(size);

  oatpp::data::stream::BufferOutputStream ss(64);
  ss << "[" << typeName << ":" << size << "]";

  return ss.toString();

}

const oatpp::Type* Deserializer::guessType(oatpp::parser::Caret& caret) {
  {
    v_char8 c = *((p_char8)caret.getCurrData());
    switch (c) {
      case Utils::TYPE_STRING_1:
      case Utils::TYPE_STRING_2:
      case Utils::TYPE_STRING_4: return String::Class::getType();

      case Utils::CONTROL_MAP_BEGIN: return oatpp::Fields<oatpp::Any>::Class::getType();
      case Utils::CONTROL_ARRAY_BEGIN: return oatpp::Vector<oatpp::Any>::Class::getType();

      case Utils::TYPE_BOOL_TRUE: return oatpp::Boolean::Class::getType();
      case Utils::TYPE_BOOL_FALSE: return oatpp::Boolean::Class::getType();

      case Utils::TYPE_INT_1: return oatpp::Int8::Class::getType();
      case Utils::TYPE_UINT_1: return oatpp::UInt8::Class::getType();
      case Utils::TYPE_INT_2: return oatpp::Int16::Class::getType();
      case Utils::TYPE_UINT_2: return oatpp::UInt16::Class::getType();
      case Utils::TYPE_INT_4: return oatpp::Int32::Class::getType();
      case Utils::TYPE_UINT_4: return oatpp::UInt32::Class::getType();
      case Utils::TYPE_INT_8: return oatpp::Int64::Class::getType();
      case Utils::TYPE_UINT_8: return oatpp::UInt64::Class::getType();

      case Utils::TYPE_FLOAT_4: return oatpp::Float32::Class::getType();
      case Utils::TYPE_FLOAT_8: return oatpp::Float64::Class::getType();
    }
  }
  caret.setError("[oatpp::bob::Deserializer::guessType()]: Error. Can't guess type for oatpp::Any.");
  return nullptr;
}

oatpp::Void Deserializer::deserializeAny(Deserializer* deserializer, oatpp::parser::Caret& caret, const Type* const type) {

  if(caret.isAtChar(Utils::TYPE_NULL)) {
    caret.inc();
    return oatpp::Void(type);
  } else {

    const Type* const fieldType = guessType(caret);
    if(fieldType != nullptr) {
      auto fieldValue = deserializer->deserialize(caret, fieldType);
      auto anyHandle = std::make_shared<oatpp::data::mapping::type::AnyHandle>(fieldValue.getPtr(), fieldValue.getValueType());
      return oatpp::Void(anyHandle, type);
    }

  }

  return oatpp::Void(type);
}

oatpp::Void Deserializer::deserializeEnum(Deserializer* deserializer, oatpp::parser::Caret& caret, const Type* const type) {

  auto polymorphicDispatcher = static_cast<const oatpp::data::mapping::type::__class::AbstractEnum::PolymorphicDispatcher*>(
    type->polymorphicDispatcher
  );

  oatpp::data::mapping::type::EnumInterpreterError e = oatpp::data::mapping::type::EnumInterpreterError::OK;
  const auto& value = deserializer->deserialize(caret, polymorphicDispatcher->getInterpretationType());
  if(caret.hasError()) {
    return nullptr;
  }
  const auto& result = polymorphicDispatcher->fromInterpretation(value, e);

  if(e == oatpp::data::mapping::type::EnumInterpreterError::OK) {
    return result;
  }

  switch(e) {
    case oatpp::data::mapping::type::EnumInterpreterError::CONSTRAINT_NOT_NULL:
      caret.setError("[oatpp::bob::Deserializer::deserializeEnum()]: Error. Enum constraint violated - 'NotNull'.");
      break;
    default:
      caret.setError("[oatpp::bob::Deserializer::deserializeEnum()]: Error. Can't deserialize Enum.");
  }

  return nullptr;

}

oatpp::Void Deserializer::deserializeCollection(Deserializer* deserializer, oatpp::parser::Caret& caret, const Type* type) {

  if(caret.isAtChar(Utils::TYPE_NULL)){
    caret.inc();
    return oatpp::Void(type);
  }

  if(caret.canContinueAtChar(Utils::CONTROL_ARRAY_BEGIN, 1)) {

    auto dispatcher = static_cast<const oatpp::data::mapping::type::__class::Collection::PolymorphicDispatcher*>(type->polymorphicDispatcher);
    auto collection = dispatcher->createObject();

    auto itemType = dispatcher->getItemType();

    while(!caret.isAtChar(Utils::CONTROL_SECTION_END) && caret.canContinue()) {

      auto item = deserializer->deserialize(caret, itemType);
      if(caret.hasError()){
        return nullptr;
      }

      dispatcher->addItem(collection, item);

    }

    if(!caret.canContinueAtChar(Utils::CONTROL_SECTION_END, 1)){
      if(!caret.hasError()){
        caret.setError("[oatpp::bob::Deserializer::deserializeCollection()]: Error. ')' - expected", ERROR_CODE_ARRAY_SCOPE_CLOSE);
      }
      return nullptr;
    };

    return collection;

  } else {
    caret.setError("[oatpp::bob::Deserializer::deserializeCollection()]: Error. '[' - expected", ERROR_CODE_ARRAY_SCOPE_OPEN);
    return nullptr;
  }

}

oatpp::Void Deserializer::deserializeMap(Deserializer* deserializer, oatpp::parser::Caret& caret, const Type* const type) {

  if(caret.isAtChar(Utils::TYPE_NULL)){
    caret.inc();
    return oatpp::Void(type);
  }

  if(caret.canContinueAtChar(Utils::CONTROL_MAP_BEGIN, 1)) {

    auto dispatcher = static_cast<const oatpp::data::mapping::type::__class::Map::PolymorphicDispatcher*>(type->polymorphicDispatcher);
    auto map = dispatcher->createObject();

    auto keyType = dispatcher->getKeyType();
    if(keyType->classId != oatpp::String::Class::CLASS_ID){
      throw std::runtime_error("[oatpp::bob::Deserializer::deserializeMap()]: Invalid json map key. Key should be String");
    }
    auto valueType = dispatcher->getValueType();

    while (!caret.isAtChar(Utils::CONTROL_SECTION_END) && caret.canContinue()) {

      auto key = Utils::readCString(caret);
      if(caret.hasError()){
        return nullptr;
      }

      auto item = deserializer->deserialize(caret, valueType);
      if(caret.hasError()){
        return nullptr;
      }
      dispatcher->addItem(map, key, item);

    }

    if(!caret.canContinueAtChar(Utils::CONTROL_SECTION_END, 1)){
      if(!caret.hasError()){
        caret.setError("[oatpp::bob::Deserializer::deserializeMap()]: Error. ')' - expected", ERROR_CODE_OBJECT_SCOPE_CLOSE);
      }
      return nullptr;
    }

    return map;

  } else {
    caret.setError("[oatpp::bob::Deserializer::deserializeMap()]: Error. '{' - expected", ERROR_CODE_OBJECT_SCOPE_OPEN);
  }

  return nullptr;

}

oatpp::Void Deserializer::deserializeObject(Deserializer* deserializer, oatpp::parser::Caret& caret, const Type* const type) {

  if(caret.isAtChar(Utils::TYPE_NULL)){
    caret.inc();
    return oatpp::Void(type);
  }

  if(caret.canContinueAtChar(Utils::CONTROL_MAP_BEGIN, 1)) {

    auto dispatcher = static_cast<const oatpp::data::mapping::type::__class::AbstractObject::PolymorphicDispatcher*>(type->polymorphicDispatcher);
    auto object = dispatcher->createObject();
    const auto& fieldsMap = dispatcher->getProperties()->getMap();

    std::vector<std::pair<oatpp::BaseObject::Property*, oatpp::String>> polymorphs;
    while (!caret.isAtChar(Utils::CONTROL_SECTION_END) && caret.canContinue()) {

      auto key = Utils::readCString(caret);
      if(caret.hasError()){
        return nullptr;
      }

      auto fieldIterator = fieldsMap.find(key);
      if(fieldIterator != fieldsMap.end()) {

        auto field = fieldIterator->second;

        if(field->info.typeSelector && field->type == oatpp::Any::Class::getType()) {
          auto label = caret.putLabel();
          skipValue(caret);
          polymorphs.emplace_back(field, label.toString()); // store polymorphs for later processing.
        } else {
          field->set(static_cast<oatpp::BaseObject *>(object.get()), deserializer->deserialize(caret, field->type));
        }

      } else if (deserializer->getConfig()->allowUnknownFields) {
        skipValue(caret);
      } else {
        caret.setError("[oatpp::bob::Deserializer::readObject()]: Error. Unknown field", ERROR_CODE_OBJECT_SCOPE_UNKNOWN_FIELD);
        return nullptr;
      }

    }

    if(!caret.canContinueAtChar(Utils::CONTROL_SECTION_END, 1)){
      if(!caret.hasError()){
        caret.setError("[oatpp::bob::Deserializer::readObject()]: Error. ')' - expected", ERROR_CODE_OBJECT_SCOPE_CLOSE);
      }
      return nullptr;
    }

    for(auto& p : polymorphs) {
      oatpp::parser::Caret polyCaret(p.second);
      auto selectedType = p.first->info.typeSelector->selectType(static_cast<oatpp::BaseObject *>(object.get()));
      auto value = deserializer->deserialize(polyCaret, selectedType);
      oatpp::Any any(value);
      p.first->set(static_cast<oatpp::BaseObject *>(object.get()), oatpp::Void(any.getPtr(), p.first->type));
    }

    return object;

  } else {
    caret.setError("[oatpp::bob::Deserializer::deserializeObject()]: Error. '{' - expected", ERROR_CODE_OBJECT_SCOPE_OPEN);
  }

  return nullptr;

}

oatpp::Void Deserializer::deserialize(oatpp::parser::Caret& caret, const Type* const type) {
  auto id = type->classId.id;
  auto& method = m_methods[id];
  if(method) {
    return (*method)(this, caret, type);
  } else {

    auto* interpretation = type->findInterpretation(m_config->enabledInterpretations);
    if(interpretation) {
      return interpretation->fromInterpretation(deserialize(caret, interpretation->getInterpretationType()));
    }

    throw std::runtime_error("[oatpp::bob::Deserializer::deserialize()]: "
                             "Error. No deserialize method for type '" + std::string(type->classId.name) + "'");
  }
}

const std::shared_ptr<Deserializer::Config>& Deserializer::getConfig() {
  return m_config;
}

void Deserializer::useDebugStrings(bool debugStrings) {
  if(debugStrings) {
    setDeserializerMethod(oatpp::String::Class::CLASS_ID, Deserializer::deserializeStringDebug);
  } else {
    setDeserializerMethod(oatpp::String::Class::CLASS_ID, Deserializer::deserializeString);
  }
}

}}
