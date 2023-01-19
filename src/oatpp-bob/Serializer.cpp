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

#include "Serializer.hpp"

#include "./Utils.hpp"

namespace oatpp { namespace bob {

Serializer::Serializer(const std::shared_ptr<Config>& config)
  : m_config(config)
{

  m_methods.resize(oatpp::data::mapping::type::ClassId::getClassCount(), nullptr);

  setSerializerMethod(oatpp::data::mapping::type::__class::String::CLASS_ID, &Serializer::serializeString);
  setSerializerMethod(oatpp::data::mapping::type::__class::Any::CLASS_ID, &Serializer::serializeAny);

  setSerializerMethod(oatpp::data::mapping::type::__class::Int8::CLASS_ID, &Serializer::serializeInt1);
  setSerializerMethod(oatpp::data::mapping::type::__class::UInt8::CLASS_ID, &Serializer::serializeUInt1);

  setSerializerMethod(oatpp::data::mapping::type::__class::Int16::CLASS_ID, &Serializer::serializeInt2);
  setSerializerMethod(oatpp::data::mapping::type::__class::UInt16::CLASS_ID, &Serializer::serializeUInt2);

  setSerializerMethod(oatpp::data::mapping::type::__class::Int32::CLASS_ID, &Serializer::serializeInt4);
  setSerializerMethod(oatpp::data::mapping::type::__class::UInt32::CLASS_ID, &Serializer::serializeUInt4);

  setSerializerMethod(oatpp::data::mapping::type::__class::Int64::CLASS_ID, &Serializer::serializeInt8);
  setSerializerMethod(oatpp::data::mapping::type::__class::UInt64::CLASS_ID, &Serializer::serializeUInt8);

  setSerializerMethod(oatpp::data::mapping::type::__class::Float32::CLASS_ID, &Serializer::serializeFloat4);
  setSerializerMethod(oatpp::data::mapping::type::__class::Float64::CLASS_ID, &Serializer::serializeFloat8);
  setSerializerMethod(oatpp::data::mapping::type::__class::Boolean::CLASS_ID, &Serializer::serializeBool);

  setSerializerMethod(oatpp::data::mapping::type::__class::AbstractObject::CLASS_ID, &Serializer::serializeObject);
  setSerializerMethod(oatpp::data::mapping::type::__class::AbstractEnum::CLASS_ID, &Serializer::serializeEnum);

  setSerializerMethod(oatpp::data::mapping::type::__class::AbstractVector::CLASS_ID, &Serializer::serializeCollection);
  setSerializerMethod(oatpp::data::mapping::type::__class::AbstractList::CLASS_ID, &Serializer::serializeCollection);
  setSerializerMethod(oatpp::data::mapping::type::__class::AbstractUnorderedSet::CLASS_ID, &Serializer::serializeCollection);

  setSerializerMethod(oatpp::data::mapping::type::__class::AbstractPairList::CLASS_ID, &Serializer::serializeMap);
  setSerializerMethod(oatpp::data::mapping::type::__class::AbstractUnorderedMap::CLASS_ID, &Serializer::serializeMap);

}

void Serializer::setSerializerMethod(const oatpp::data::mapping::type::ClassId& classId, SerializerMethod method) {
  const v_uint32 id = classId.id;
  if(id >= m_methods.size()) {
    m_methods.resize(id + 1, nullptr);
  }
  m_methods[id] = method;
}

void Serializer::serializeKey(ConsistentOutputStream* stream, const char* data, v_buff_size size) {
  stream->writeSimple(data, size);
  stream->writeCharSimple(0);
}

void Serializer::serializeString(Serializer* serializer,
                                 ConsistentOutputStream* stream,
                                 const oatpp::Void& polymorph)
{

  if(!polymorph) {
    stream->writeCharSimple(Utils::TYPE_NULL);
    return;
  }

  auto str = static_cast<std::string*>(polymorph.get());

  if(str->size() < ((v_int64)1 << 8)) {
    stream->writeCharSimple(Utils::TYPE_STRING_1);
    Utils::writeInt8(stream, str->size());
  } else if(str->length() < ((v_int64)1 << 16)) {
    stream->writeCharSimple(Utils::TYPE_STRING_2);
    Utils::writeInt16(stream, str->size(), Utils::BO_TYPE::NETWORK);
  } else if(str->length() < ((v_int64)1 << 32)) {
    stream->writeCharSimple(Utils::TYPE_STRING_4);
    Utils::writeInt32(stream, str->size(), Utils::BO_TYPE::NETWORK);
  } else {
    throw std::runtime_error("[oatpp::bob::Serializer::serializeString()]: Error. Invalid string size.");
  }

  stream->writeSimple(str->data(), str->size());

}

void Serializer::serializeBool(Serializer* serializer, ConsistentOutputStream* stream, const oatpp::Void& polymorph) {
  if(!polymorph) {
    stream->writeCharSimple(Utils::TYPE_NULL);
    return;
  }

  auto v = *(static_cast<bool*>(polymorph.get()));

  if(v) {
    stream->writeCharSimple(Utils::TYPE_BOOL_TRUE);
  } else {
    stream->writeCharSimple(Utils::TYPE_BOOL_FALSE);
  }
}

void Serializer::serializeInt1(Serializer* serializer, ConsistentOutputStream* stream, const oatpp::Void& polymorph) {
  if(!polymorph) {
    stream->writeCharSimple(Utils::TYPE_NULL);
    return;
  }
  stream->writeCharSimple(Utils::TYPE_INT_1);
  auto v = *(static_cast<v_int8*>(polymorph.get()));
  Utils::writeInt8(stream, v);
}

void Serializer::serializeUInt1(Serializer* serializer, ConsistentOutputStream* stream, const oatpp::Void& polymorph) {
  if(!polymorph) {
    stream->writeCharSimple(Utils::TYPE_NULL);
    return;
  }
  stream->writeCharSimple(Utils::TYPE_UINT_1);
  auto v = *(static_cast<v_int8*>(polymorph.get()));
  Utils::writeInt8(stream, v);
}

void Serializer::serializeInt2(Serializer* serializer, ConsistentOutputStream* stream, const oatpp::Void& polymorph) {
  if(!polymorph) {
    stream->writeCharSimple(Utils::TYPE_NULL);
    return;
  }
  stream->writeCharSimple(Utils::TYPE_INT_2);
  auto v = *(static_cast<v_int16*>(polymorph.get()));
  Utils::writeInt16(stream, v, Utils::BO_TYPE::NETWORK);
}

void Serializer::serializeUInt2(Serializer* serializer, ConsistentOutputStream* stream, const oatpp::Void& polymorph) {
  if(!polymorph) {
    stream->writeCharSimple(Utils::TYPE_NULL);
    return;
  }
  stream->writeCharSimple(Utils::TYPE_UINT_2);
  auto v = *(static_cast<v_int16*>(polymorph.get()));
  Utils::writeInt16(stream, v, Utils::BO_TYPE::NETWORK);
}

void Serializer::serializeInt4(Serializer* serializer, ConsistentOutputStream* stream, const oatpp::Void& polymorph) {
  if(!polymorph) {
    stream->writeCharSimple(Utils::TYPE_NULL);
    return;
  }
  stream->writeCharSimple(Utils::TYPE_INT_4);
  auto v = *(static_cast<v_int32*>(polymorph.get()));
  Utils::writeInt32(stream, v, Utils::BO_TYPE::NETWORK);
}

void Serializer::serializeUInt4(Serializer* serializer, ConsistentOutputStream* stream, const oatpp::Void& polymorph) {
  if(!polymorph) {
    stream->writeCharSimple(Utils::TYPE_NULL);
    return;
  }
  stream->writeCharSimple(Utils::TYPE_UINT_4);
  auto v = *(static_cast<v_int32*>(polymorph.get()));
  Utils::writeInt32(stream, v, Utils::BO_TYPE::NETWORK);
}

void Serializer::serializeInt8(Serializer* serializer, ConsistentOutputStream* stream, const oatpp::Void& polymorph) {
  if(!polymorph) {
    stream->writeCharSimple(Utils::TYPE_NULL);
    return;
  }
  stream->writeCharSimple(Utils::TYPE_INT_8);
  auto v = *(static_cast<v_int64*>(polymorph.get()));
  Utils::writeInt64(stream, v, Utils::BO_TYPE::NETWORK);
}

void Serializer::serializeUInt8(Serializer* serializer, ConsistentOutputStream* stream, const oatpp::Void& polymorph) {
  if(!polymorph) {
    stream->writeCharSimple(Utils::TYPE_NULL);
    return;
  }
  stream->writeCharSimple(Utils::TYPE_UINT_8);
  auto v = *(static_cast<v_int64*>(polymorph.get()));
  Utils::writeInt64(stream, v, Utils::BO_TYPE::NETWORK);
}

void Serializer::serializeFloat4(Serializer* serializer, ConsistentOutputStream* stream, const oatpp::Void& polymorph) {
  if(!polymorph) {
    stream->writeCharSimple(Utils::TYPE_NULL);
    return;
  }
  stream->writeCharSimple(Utils::TYPE_FLOAT_4);
  auto v = *(static_cast<v_float32 *>(polymorph.get()));
  Utils::writeFloat32(stream, v, Utils::BO_TYPE::NETWORK);
}

void Serializer::serializeFloat8(Serializer* serializer, ConsistentOutputStream* stream, const oatpp::Void& polymorph) {
  if(!polymorph) {
    stream->writeCharSimple(Utils::TYPE_NULL);
    return;
  }
  stream->writeCharSimple(Utils::TYPE_FLOAT_8);
  auto v = *(static_cast<v_float64 *>(polymorph.get()));
  Utils::writeFloat64(stream, v, Utils::BO_TYPE::NETWORK);
}

void Serializer::serializeAny(Serializer* serializer,
                              ConsistentOutputStream* stream,
                              const oatpp::Void& polymorph)
{

  if(!polymorph) {
    stream->writeCharSimple(Utils::TYPE_NULL);
    return;
  }

  auto anyHandle = static_cast<oatpp::data::mapping::type::AnyHandle*>(polymorph.get());
  serializer->serialize(stream, oatpp::Void(anyHandle->ptr, anyHandle->type));

}

void Serializer::serializeEnum(Serializer* serializer,
                               ConsistentOutputStream* stream,
                               const oatpp::Void& polymorph)
{
  auto polymorphicDispatcher = static_cast<const oatpp::data::mapping::type::__class::AbstractEnum::PolymorphicDispatcher*>(
    polymorph.getValueType()->polymorphicDispatcher
  );

  oatpp::data::mapping::type::EnumInterpreterError e = oatpp::data::mapping::type::EnumInterpreterError::OK;
  serializer->serialize(stream, polymorphicDispatcher->toInterpretation(polymorph, e));

  if(e == oatpp::data::mapping::type::EnumInterpreterError::OK) {
    return;
  }

  switch(e) {
    case oatpp::data::mapping::type::EnumInterpreterError::CONSTRAINT_NOT_NULL:
      throw std::runtime_error("[oatpp::bob::Serializer::serializeEnum()]: Error. Enum constraint violated - 'NotNull'.");
    default:
      throw std::runtime_error("[oatpp::bob::Serializer::serializeEnum()]: Error. Can't serialize Enum.");
  }

}

void Serializer::serializeCollection(Serializer* serializer,
                                     ConsistentOutputStream* stream,
                                     const oatpp::Void& polymorph)
{

  if(!polymorph) {
    stream->writeCharSimple(Utils::TYPE_NULL);
    return;
  }

  auto dispatcher = static_cast<const oatpp::data::mapping::type::__class::Collection::PolymorphicDispatcher*>(
    polymorph.getValueType()->polymorphicDispatcher
  );

  stream->writeCharSimple(Utils::CONTROL_ARRAY_BEGIN);
  bool first = true;

  auto iterator = dispatcher->beginIteration(polymorph);

  while (!iterator->finished()) {
    const auto& value = iterator->get();
    if(value || serializer->getConfig()->includeNullFields || serializer->getConfig()->alwaysIncludeNullCollectionElements) {
      serializer->serialize(stream, value);
    }
    iterator->next();
  }

  stream->writeCharSimple(Utils::CONTROL_SECTION_END);

}

void Serializer::serializeMap(Serializer* serializer,
                              ConsistentOutputStream* stream,
                              const oatpp::Void& polymorph)
{

  if(!polymorph) {
    stream->writeCharSimple(Utils::TYPE_NULL);
    return;
  }

  auto dispatcher = static_cast<const oatpp::data::mapping::type::__class::Map::PolymorphicDispatcher*>(
    polymorph.getValueType()->polymorphicDispatcher
  );

  auto keyType = dispatcher->getKeyType();
  if(keyType->classId != oatpp::String::Class::CLASS_ID){
    throw std::runtime_error("[oatpp::bob::Serializer::serializeMap()]: Invalid json map key. Key should be String");
  }

  stream->writeCharSimple(Utils::CONTROL_MAP_BEGIN);
  bool first = true;

  auto iterator = dispatcher->beginIteration(polymorph);

  while (!iterator->finished()) {
    const auto& value = iterator->getValue();
    if(value || serializer->m_config->includeNullFields || serializer->m_config->alwaysIncludeNullCollectionElements) {
      const auto& untypedKey = iterator->getKey();
      const auto& key = oatpp::String(std::static_pointer_cast<std::string>(untypedKey.getPtr()));
      serializeKey(stream, key->data(), key->size());
      serializer->serialize(stream, value);
    }
    iterator->next();
  }

  stream->writeCharSimple(Utils::CONTROL_SECTION_END);

}

void Serializer::serializeObject(Serializer* serializer,
                                 ConsistentOutputStream* stream,
                                 const oatpp::Void& polymorph)
{

  if(!polymorph) {
    stream->writeCharSimple(Utils::TYPE_NULL);
    return;
  }

  stream->writeCharSimple(Utils::CONTROL_MAP_BEGIN);

  auto dispatcher = static_cast<const oatpp::data::mapping::type::__class::AbstractObject::PolymorphicDispatcher*>(
    polymorph.getValueType()->polymorphicDispatcher
  );
  auto fields = dispatcher->getProperties()->getList();
  auto object = static_cast<oatpp::BaseObject*>(polymorph.get());
  auto config = serializer->m_config;

  for (auto const& field : fields) {

    oatpp::Void value;
    if(field->info.typeSelector && field->type == oatpp::Any::Class::getType()) {
      const auto& any = field->get(object).cast<oatpp::Any>();
      value = any.retrieve(field->info.typeSelector->selectType(object));
    } else {
      value = field->get(object);
    }

    if (value || config->includeNullFields || (field->info.required && config->alwaysIncludeRequired)) {
      serializeKey(stream, field->name, std::strlen(field->name));
      serializer->serialize(stream, value);
    }

  }

  stream->writeCharSimple(Utils::CONTROL_SECTION_END);

}

void Serializer::serialize(ConsistentOutputStream* stream,
                           const oatpp::Void& polymorph)
{
  auto id = polymorph.getValueType()->classId.id;
  auto& method = m_methods[id];
  if(method) {
    (*method)(this, stream, polymorph);
  } else {

    auto* interpretation = polymorph.getValueType()->findInterpretation(m_config->enabledInterpretations);
    if(interpretation) {
      serialize(stream, interpretation->toInterpretation(polymorph));
    } else {
      throw std::runtime_error("[oatpp::bob::Serializer::serialize()]: "
                               "Error. No serialize method for type '" +
                               std::string(polymorph.getValueType()->classId.name) + "'");
    }

  }
}

void Serializer::serializeToStream(ConsistentOutputStream* stream,
                                   const oatpp::Void& polymorph)
{
  serialize(stream, polymorph);
}

const std::shared_ptr<Serializer::Config>& Serializer::getConfig() {
  return m_config;
}

}}
