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


#include "UtilsTest.hpp"

#include "oatpp-bob/Utils.hpp"

#include "oatpp/core/data/stream/BufferStream.hpp"

#include <arpa/inet.h>

namespace oatpp { namespace bob { namespace test {

namespace {

typedef oatpp::bob::Utils::BO_TYPE BO_TYPE;

}

void UtilsTest::onRun() {

  auto ibo = oatpp::bob::Utils::detectIntBO();
  auto fbo = oatpp::bob::Utils::detectFloatBO();

  OATPP_LOGD("TEST", "ibo=%d, fbo=%d", ibo, fbo)

  {
    v_uint8 data[4];
    data[0] = 255;
    data[1] = 0;
    data[2] = 0;
    data[3] = 0;
    oatpp::parser::Caret cLil((const char*) data, 4);
    auto v1 = oatpp::bob::Utils::readInt32(cLil, BO_TYPE::NETWORK);
    auto v2 = ntohl(*((p_int32) data));
    OATPP_ASSERT(v1 == v2)
  }

  {
    oatpp::String ddata = "01";
    oatpp::String idata = "10";
    oatpp::parser::Caret cLil(ddata);
    auto v = oatpp::bob::Utils::readInt16(cLil, BO_TYPE::LITTLE);
    oatpp::String check((const char*)&v, 2);
    if(ibo == BO_TYPE::LITTLE) {
      OATPP_ASSERT(check == ddata)
    } else {
      OATPP_ASSERT(check == idata)
    }
  }

  {
    oatpp::String ddata = "01";
    oatpp::String idata = "10";
    oatpp::parser::Caret cLil(ddata);
    auto v = oatpp::bob::Utils::readInt16(cLil, BO_TYPE::NETWORK);
    oatpp::String check((const char*)&v, 2);
    if(ibo == BO_TYPE::LITTLE) {
      OATPP_ASSERT(check == idata)
    } else {
      OATPP_ASSERT(check == ddata)
    }
  }

  {
    oatpp::String ddata = "0123";
    oatpp::String idata = "3210";
    oatpp::parser::Caret cLil(ddata);
    auto v = oatpp::bob::Utils::readInt32(cLil, BO_TYPE::LITTLE);
    oatpp::String check((const char*)&v, 4);
    if(ibo == BO_TYPE::LITTLE) {
      OATPP_ASSERT(check == ddata)
    } else {
      OATPP_ASSERT(check == idata)
    }
  }

  {
    oatpp::String ddata = "0123";
    oatpp::String idata = "3210";
    oatpp::parser::Caret cLil(ddata);
    auto v = oatpp::bob::Utils::readInt32(cLil, BO_TYPE::NETWORK);
    oatpp::String check((const char*)&v, 4);
    if(ibo == BO_TYPE::LITTLE) {
      OATPP_ASSERT(check == idata)
    } else {
      OATPP_ASSERT(check == ddata)
    }
  }

  {
    oatpp::String ddata = "01234567";
    oatpp::String idata = "76543210";
    oatpp::parser::Caret cLil(ddata);
    auto v = oatpp::bob::Utils::readInt64(cLil, BO_TYPE::LITTLE);
    oatpp::String check((const char*)&v, 8);
    if(ibo == BO_TYPE::LITTLE) {
      OATPP_ASSERT(check == ddata)
    } else {
      OATPP_ASSERT(check == idata)
    }
  }

  {
    oatpp::String ddata = "01234567";
    oatpp::String idata = "76543210";
    oatpp::parser::Caret cLil(ddata);
    auto v = oatpp::bob::Utils::readInt64(cLil, BO_TYPE::NETWORK);
    oatpp::String check((const char*)&v, 8);
    if(ibo == BO_TYPE::LITTLE) {
      OATPP_ASSERT(check == idata)
    } else {
      OATPP_ASSERT(check == ddata)
    }
  }

  {
    v_int64 v = ( (v_int64) '0'       ) | (((v_int64) '1') <<  8) | (((v_int64) '2') << 16) | (((v_int64) '3') << 24)
                | (((v_int64) '4') << 32) | (((v_int64) '5') << 40) | (((v_int64) '6') << 48) | (((v_int64) '7') << 56);

    oatpp::data::stream::BufferOutputStream ss;
    oatpp::bob::Utils::writeInt64(&ss, v, BO_TYPE::NETWORK);
    auto vs2 = ss.toString();

    v_int64 vcheck = htonll(v);
    oatpp::String checks((const char*) &vcheck, 8);

    OATPP_ASSERT(vs2 == checks)

  }

}

}}}
