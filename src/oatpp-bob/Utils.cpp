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


#include "Utils.hpp"

namespace oatpp { namespace bob {

Utils::BO_TYPE Utils::MY_INT_BO = detectIntBO();
Utils::BO_TYPE Utils::MY_FLOAT_BO = detectFloatBO();

Utils::BO_TYPE Utils::detectIntBO() {
  BO_TYPE result = BO_TYPE::NETWORK;
  BO_CHECK check;
  check.i64 = 255;
  if(check.bytes[0] == 255) {
    result = BO_TYPE::LITTLE;
  }
  return result;
}

Utils::BO_TYPE Utils::detectFloatBO() {
  BO_TYPE result = BO_TYPE::LITTLE;
  BO_CHECK check;
  check.f64 = 2.0;
  if(check.bytes[0] > 0) {
    result = BO_TYPE::NETWORK;
  }
  return result;
}

oatpp::String Utils::readCString(oatpp::parser::Caret& caret) {
  auto label = caret.putLabel();
  if(caret.findChar(0)) {
    label.end();
    caret.inc();
    return label.toString();
  }
  caret.setError("[oatpp::bob::readCString()]: Error. Unterminated CString.");
  return nullptr;
}

void Utils::writeCString(ConsistentOutputStream* stream, const oatpp::String& data) {
  if(data != nullptr && data->size() > 0) {
    stream->writeSimple(data->data(), data->size());
  }
  stream->writeCharSimple(0);
}

v_int8 Utils::readInt8(oatpp::parser::Caret& caret) {

  if(caret.getDataSize() - caret.getPosition() < 1) {
    caret.setError("[oatpp::bob::readInt8]: Error. Invalid value.");
    return 0;
  }

  v_int8 value = (v_int8) caret.getCurrData()[0];
  caret.inc();

  return value;

}

void Utils::writeInt8(ConsistentOutputStream* stream, v_int8 value) {
  stream->writeCharSimple(value);
}

v_int16 Utils::readInt16(oatpp::parser::Caret& caret, BO_TYPE readBO) {

  if(caret.getDataSize() - caret.getPosition() < 2) {
    caret.setError("[oatpp::bob::readInt16]: Error. Invalid value.");
    return 0;
  }

  p_char8 data = (p_char8) caret.getCurrData();
  v_int16 val;

  if(MY_INT_BO == readBO) {
    val = *((p_int16) data);
  } else {

    switch (readBO) {

      case BO_TYPE::LITTLE:
        val = ((v_int16) data[0]) | ((v_int16) data[1] << 8);
        break;

      case BO_TYPE::NETWORK:
        val = ((v_int16) data[1]) | ((v_int16) data[0] << 8);
        break;

    }

  }
  caret.inc(2);
  return val;

}

void Utils::writeInt16(ConsistentOutputStream* stream, v_int16 value, BO_TYPE writeBO) {

  if(MY_INT_BO == writeBO) {
    stream->writeSimple(&value, 2);
  } else {

    v_uint8 data[2];

    switch (writeBO) {

      case BO_TYPE::LITTLE:
        data[0] = (v_uint8) (0xFF & value);
        data[1] = (v_uint8) (0xFF & (value >> 8));
        break;

      case BO_TYPE::NETWORK:
        data[1] = (v_uint8) (0xFF & value);
        data[0] = (v_uint8) (0xFF & (value >> 8));
        break;

    }

    stream->writeSimple(data, 2);

  }

}

v_int32 Utils::readInt32(oatpp::parser::Caret& caret, BO_TYPE readBO) {

  if(caret.getDataSize() - caret.getPosition() < 4) {
    caret.setError("[oatpp::bob::readInt32]: Error. Invalid value.");
    return 0;
  }

  p_char8 data = (p_char8) caret.getCurrData();
  v_int32 val;

  if(MY_INT_BO == readBO) {
    val = *((p_int32) data);
  } else {

    switch (readBO) {

      case BO_TYPE::LITTLE:
        val = ((v_int32) data[0]) | ((v_int32) data[1] << 8) | ((v_int32) data[2] << 16) | ((v_int32) data[3] << 24);
        break;

      case BO_TYPE::NETWORK:
        val = ((v_int32) data[3]) | ((v_int32) data[2] << 8) | ((v_int32) data[1] << 16) | ((v_int32) data[0] << 24);
        break;

    }

  }

  caret.inc(4);
  return val;

}

void Utils::writeInt32(ConsistentOutputStream* stream, v_int32 value, BO_TYPE writeBO) {

  if(MY_INT_BO == writeBO) {
    stream->writeSimple(&value, 4);
  } else {

    v_uint8 data[4];

    switch (writeBO) {

      case BO_TYPE::LITTLE:
        data[0] = (v_uint8) (0xFF & value);
        data[1] = (v_uint8) (0xFF & (value >> 8));
        data[2] = (v_uint8) (0xFF & (value >> 16));
        data[3] = (v_uint8) (0xFF & (value >> 24));
        break;

      case BO_TYPE::NETWORK:
        data[3] = (v_uint8) (0xFF & value);
        data[2] = (v_uint8) (0xFF & (value >> 8));
        data[1] = (v_uint8) (0xFF & (value >> 16));
        data[0] = (v_uint8) (0xFF & (value >> 24));
        break;

    }

    stream->writeSimple(data, 4);

  }

}

v_int64 Utils::readInt64(oatpp::parser::Caret& caret, BO_TYPE readBO) {

  if(caret.getDataSize() - caret.getPosition() < 8) {
    caret.setError("[oatpp::bob::readInt64]: Error. Invalid value.");
    return 0;
  }

  p_char8 data = (p_char8) caret.getCurrData();
  v_int64 val;

  if(MY_INT_BO == readBO) {
    val = *((p_int64) data);
  } else {

    switch (readBO) {

      case BO_TYPE::LITTLE:
        val = ((v_int64) data[0]      ) | ((v_int64) data[1] <<  8) | ((v_int64) data[2] << 16) | ((v_int64) data[3] << 24) |
              ((v_int64) data[4] << 32) | ((v_int64) data[5] << 40) | ((v_int64) data[6] << 48) | ((v_int64) data[7] << 56);
        break;

      case BO_TYPE::NETWORK:
        val = ((v_int64) data[7]      ) | ((v_int64) data[6] <<  8) | ((v_int64) data[5] << 16) | ((v_int64) data[4] << 24) |
              ((v_int64) data[3] << 32) | ((v_int64) data[2] << 40) | ((v_int64) data[1] << 48) | ((v_int64) data[0] << 56);
        break;

    }

  }

  caret.inc(8);
  return val;


}

void Utils::writeInt64(ConsistentOutputStream* stream, v_int64 value, BO_TYPE writeBO) {

  if(MY_INT_BO == writeBO) {
    stream->writeSimple(&value, 8);
  } else {

    v_uint8 data[8];

    switch (writeBO) {

      case BO_TYPE::LITTLE:
        data[0] = (v_uint8) (0xFF & value);
        data[1] = (v_uint8) (0xFF & (value >> 8));
        data[2] = (v_uint8) (0xFF & (value >> 16));
        data[3] = (v_uint8) (0xFF & (value >> 24));
        data[4] = (v_uint8) (0xFF & (value >> 32));
        data[5] = (v_uint8) (0xFF & (value >> 40));
        data[6] = (v_uint8) (0xFF & (value >> 48));
        data[7] = (v_uint8) (0xFF & (value >> 56));
        break;

      case BO_TYPE::NETWORK:
        data[7] = (v_uint8) (0xFF & value);
        data[6] = (v_uint8) (0xFF & (value >> 8));
        data[5] = (v_uint8) (0xFF & (value >> 16));
        data[4] = (v_uint8) (0xFF & (value >> 24));
        data[3] = (v_uint8) (0xFF & (value >> 32));
        data[2] = (v_uint8) (0xFF & (value >> 40));
        data[1] = (v_uint8) (0xFF & (value >> 48));
        data[0] = (v_uint8) (0xFF & (value >> 56));
        break;

    }

    stream->writeSimple(data, 8);

  }

}

v_float32 Utils::readFloat32(oatpp::parser::Caret& caret, BO_TYPE readBO) {

  if(caret.getDataSize() - caret.getPosition() < 4) {
    caret.setError("[oatpp::bob::readFloat32]: Error. Invalid value.");
    return 0;
  }

  p_char8 data = (p_char8) caret.getCurrData();
  v_float32 val;

  if(MY_FLOAT_BO == readBO) {
    val = *((p_float32) data);
  } else {
    p_char8 vald = (p_char8) &val;
    vald[0] = data[3];
    vald[1] = data[2];
    vald[2] = data[1];
    vald[3] = data[0];
  }

  caret.inc(4);
  return val;

}

void Utils::writeFloat32(ConsistentOutputStream* stream, v_float32 value, BO_TYPE writeBO) {

  if(MY_FLOAT_BO == writeBO) {
    stream->writeSimple(&value, 4);
  } else {

    p_char8 vd = (p_char8) &value;
    v_char8 data[4];

    data[0] = vd[3];
    data[1] = vd[2];
    data[2] = vd[1];
    data[3] = vd[0];

    stream->writeSimple(data, 4);

  }

}

v_float64 Utils::readFloat64(oatpp::parser::Caret& caret, BO_TYPE readBO) {

  if(caret.getDataSize() - caret.getPosition() < 8) {
    caret.setError("[oatpp::bob::readFloat64]: Error. Invalid value.");
    return 0;
  }

  p_char8 data = (p_char8) caret.getCurrData();
  v_float64 val;

  if(MY_FLOAT_BO == readBO) {
    val = *((p_float64) data);
  } else {
    p_char8 vald = (p_char8) &val;

    vald[0] = data[7];
    vald[1] = data[6];
    vald[2] = data[5];
    vald[3] = data[4];
    vald[4] = data[3];
    vald[5] = data[2];
    vald[6] = data[1];
    vald[7] = data[0];
  }

  caret.inc(8);
  return val;

}

void Utils::writeFloat64(ConsistentOutputStream* stream, v_float64 value, BO_TYPE writeBO) {

  if(MY_FLOAT_BO == writeBO) {
    stream->writeSimple(&value, 8);
  } else {

    p_char8 vd = (p_char8) &value;
    v_char8 data[8];

    data[0] = vd[7];
    data[1] = vd[6];
    data[2] = vd[5];
    data[3] = vd[4];
    data[4] = vd[3];
    data[5] = vd[2];
    data[6] = vd[1];
    data[7] = vd[0];

    stream->writeSimple(data, 8);

  }

}

}}
