/**
 * Autogenerated by Thrift
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#pragma once

#include "thrift/compiler/test/fixtures/forward_compatibility/gen-cpp2/forward_compatibility_types.h"
#include <thrift/lib/cpp/TApplicationException.h>
#include <folly/io/IOBuf.h>
#include <folly/io/IOBufQueue.h>
#include <thrift/lib/cpp/transport/THeader.h>
#include <thrift/lib/cpp2/server/Cpp2ConnContext.h>
#include <thrift/lib/cpp2/GeneratedCodeHelper.h>
#include <thrift/lib/cpp2/GeneratedSerializationCodeHelper.h>

#include <thrift/lib/cpp2/protocol/BinaryProtocol.h>
#include <thrift/lib/cpp2/protocol/CompactProtocol.h>
#include <thrift/lib/cpp2/protocol/SimpleJSONProtocol.h>
namespace cpp2 {

template <class Protocol_>
uint32_t OldStructure::read(Protocol_* iprot) {
  uint32_t xfer = 0;
  std::string _fname;
  apache::thrift::protocol::TType _ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(_fname);

  using apache::thrift::TProtocolException;


  while (true) {
    xfer += iprot->readFieldBegin(_fname, _ftype, fid);
    if (_ftype == apache::thrift::protocol::T_STOP) {
      break;
    }
    if (fid == std::numeric_limits<int16_t>::min()) {
      this->translateFieldName(_fname, fid, _ftype);
    }
    switch (fid) {
      case 1:
      {
        if (_ftype == apache::thrift::protocol::T_MAP) {
          this->features = std::map<int16_t, double>();
          xfer += ::apache::thrift::detail::pm::protocol_methods< ::apache::thrift::type_class::map<::apache::thrift::type_class::integral, ::apache::thrift::type_class::floating_point>, std::map<int16_t, double>>::read(*iprot, this->features);
          this->__isset.features = true;
        } else {
          xfer += iprot->skip(_ftype);
        }
        break;
      }
      default:
      {
        xfer += iprot->skip(_ftype);
        break;
      }
    }
    xfer += iprot->readFieldEnd();
  }
  xfer += iprot->readStructEnd();

  return xfer;
}

template <class Protocol_>
uint32_t OldStructure::serializedSize(Protocol_ const* prot_) const {
  uint32_t xfer = 0;
  xfer += prot_->serializedStructSize("OldStructure");
  xfer += prot_->serializedFieldSize("features", apache::thrift::protocol::T_MAP, 1);
  xfer += ::apache::thrift::detail::pm::protocol_methods< ::apache::thrift::type_class::map<::apache::thrift::type_class::integral, ::apache::thrift::type_class::floating_point>, std::map<int16_t, double>>::serializedSize<false>(*prot_, this->features);
  xfer += prot_->serializedSizeStop();
  return xfer;
}

template <class Protocol_>
uint32_t OldStructure::serializedSizeZC(Protocol_ const* prot_) const {
  uint32_t xfer = 0;
  xfer += prot_->serializedStructSize("OldStructure");
  xfer += prot_->serializedFieldSize("features", apache::thrift::protocol::T_MAP, 1);
  xfer += ::apache::thrift::detail::pm::protocol_methods< ::apache::thrift::type_class::map<::apache::thrift::type_class::integral, ::apache::thrift::type_class::floating_point>, std::map<int16_t, double>>::serializedSize<false>(*prot_, this->features);
  xfer += prot_->serializedSizeStop();
  return xfer;
}

template <class Protocol_>
uint32_t OldStructure::write(Protocol_* prot_) const {
  uint32_t xfer = 0;
  xfer += prot_->writeStructBegin("OldStructure");
  xfer += prot_->writeFieldBegin("features", apache::thrift::protocol::T_MAP, 1);
  xfer += ::apache::thrift::detail::pm::protocol_methods< ::apache::thrift::type_class::map<::apache::thrift::type_class::integral, ::apache::thrift::type_class::floating_point>, std::map<int16_t, double>>::write(*prot_, this->features);
  xfer += prot_->writeFieldEnd();
  xfer += prot_->writeFieldStop();
  xfer += prot_->writeStructEnd();
  return xfer;
}

} // cpp2
namespace apache { namespace thrift {

}} // apache::thrift
namespace cpp2 {

template <class Protocol_>
uint32_t NewStructure::read(Protocol_* iprot) {
  uint32_t xfer = 0;
  std::string _fname;
  apache::thrift::protocol::TType _ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(_fname);

  using apache::thrift::TProtocolException;


  while (true) {
    xfer += iprot->readFieldBegin(_fname, _ftype, fid);
    if (_ftype == apache::thrift::protocol::T_STOP) {
      break;
    }
    if (fid == std::numeric_limits<int16_t>::min()) {
      this->translateFieldName(_fname, fid, _ftype);
    }
    switch (fid) {
      case 1:
      {
        if (_ftype == apache::thrift::protocol::T_MAP) {
          this->features = std::map<int16_t, double>();
          xfer += ::apache::thrift::detail::pm::protocol_methods< ::apache::thrift::type_class::map_forward_compatibility<::apache::thrift::type_class::integral, ::apache::thrift::type_class::floating_point>, std::map<int16_t, double>>::read(*iprot, this->features);
          this->__isset.features = true;
        } else {
          xfer += iprot->skip(_ftype);
        }
        break;
      }
      default:
      {
        xfer += iprot->skip(_ftype);
        break;
      }
    }
    xfer += iprot->readFieldEnd();
  }
  xfer += iprot->readStructEnd();

  return xfer;
}

template <class Protocol_>
uint32_t NewStructure::serializedSize(Protocol_ const* prot_) const {
  uint32_t xfer = 0;
  xfer += prot_->serializedStructSize("NewStructure");
  xfer += prot_->serializedFieldSize("features", apache::thrift::protocol::T_MAP, 1);
  xfer += ::apache::thrift::detail::pm::protocol_methods< ::apache::thrift::type_class::map_forward_compatibility<::apache::thrift::type_class::integral, ::apache::thrift::type_class::floating_point>, std::map<int16_t, double>>::serializedSize<false>(*prot_, this->features);
  xfer += prot_->serializedSizeStop();
  return xfer;
}

template <class Protocol_>
uint32_t NewStructure::serializedSizeZC(Protocol_ const* prot_) const {
  uint32_t xfer = 0;
  xfer += prot_->serializedStructSize("NewStructure");
  xfer += prot_->serializedFieldSize("features", apache::thrift::protocol::T_MAP, 1);
  xfer += ::apache::thrift::detail::pm::protocol_methods< ::apache::thrift::type_class::map_forward_compatibility<::apache::thrift::type_class::integral, ::apache::thrift::type_class::floating_point>, std::map<int16_t, double>>::serializedSize<false>(*prot_, this->features);
  xfer += prot_->serializedSizeStop();
  return xfer;
}

template <class Protocol_>
uint32_t NewStructure::write(Protocol_* prot_) const {
  uint32_t xfer = 0;
  xfer += prot_->writeStructBegin("NewStructure");
  xfer += prot_->writeFieldBegin("features", apache::thrift::protocol::T_MAP, 1);
  xfer += ::apache::thrift::detail::pm::protocol_methods< ::apache::thrift::type_class::map_forward_compatibility<::apache::thrift::type_class::integral, ::apache::thrift::type_class::floating_point>, std::map<int16_t, double>>::write(*prot_, this->features);
  xfer += prot_->writeFieldEnd();
  xfer += prot_->writeFieldStop();
  xfer += prot_->writeStructEnd();
  return xfer;
}

} // cpp2
namespace apache { namespace thrift {

}} // apache::thrift
namespace cpp2 {

template <class Protocol_>
uint32_t NewStructure2::read(Protocol_* iprot) {
  uint32_t xfer = 0;
  std::string _fname;
  apache::thrift::protocol::TType _ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(_fname);

  using apache::thrift::TProtocolException;


  while (true) {
    xfer += iprot->readFieldBegin(_fname, _ftype, fid);
    if (_ftype == apache::thrift::protocol::T_STOP) {
      break;
    }
    if (fid == std::numeric_limits<int16_t>::min()) {
      this->translateFieldName(_fname, fid, _ftype);
    }
    switch (fid) {
      case 1:
      {
        if (_ftype == apache::thrift::protocol::T_MAP) {
          this->features =  ::cpp2::FloatFeatures();
          xfer += ::apache::thrift::detail::pm::protocol_methods< ::apache::thrift::type_class::map_forward_compatibility<::apache::thrift::type_class::integral, ::apache::thrift::type_class::floating_point>,  ::cpp2::FloatFeatures>::read(*iprot, this->features);
          this->__isset.features = true;
        } else {
          xfer += iprot->skip(_ftype);
        }
        break;
      }
      default:
      {
        xfer += iprot->skip(_ftype);
        break;
      }
    }
    xfer += iprot->readFieldEnd();
  }
  xfer += iprot->readStructEnd();

  return xfer;
}

template <class Protocol_>
uint32_t NewStructure2::serializedSize(Protocol_ const* prot_) const {
  uint32_t xfer = 0;
  xfer += prot_->serializedStructSize("NewStructure2");
  xfer += prot_->serializedFieldSize("features", apache::thrift::protocol::T_MAP, 1);
  xfer += ::apache::thrift::detail::pm::protocol_methods< ::apache::thrift::type_class::map_forward_compatibility<::apache::thrift::type_class::integral, ::apache::thrift::type_class::floating_point>,  ::cpp2::FloatFeatures>::serializedSize<false>(*prot_, this->features);
  xfer += prot_->serializedSizeStop();
  return xfer;
}

template <class Protocol_>
uint32_t NewStructure2::serializedSizeZC(Protocol_ const* prot_) const {
  uint32_t xfer = 0;
  xfer += prot_->serializedStructSize("NewStructure2");
  xfer += prot_->serializedFieldSize("features", apache::thrift::protocol::T_MAP, 1);
  xfer += ::apache::thrift::detail::pm::protocol_methods< ::apache::thrift::type_class::map_forward_compatibility<::apache::thrift::type_class::integral, ::apache::thrift::type_class::floating_point>,  ::cpp2::FloatFeatures>::serializedSize<false>(*prot_, this->features);
  xfer += prot_->serializedSizeStop();
  return xfer;
}

template <class Protocol_>
uint32_t NewStructure2::write(Protocol_* prot_) const {
  uint32_t xfer = 0;
  xfer += prot_->writeStructBegin("NewStructure2");
  xfer += prot_->writeFieldBegin("features", apache::thrift::protocol::T_MAP, 1);
  xfer += ::apache::thrift::detail::pm::protocol_methods< ::apache::thrift::type_class::map_forward_compatibility<::apache::thrift::type_class::integral, ::apache::thrift::type_class::floating_point>,  ::cpp2::FloatFeatures>::write(*prot_, this->features);
  xfer += prot_->writeFieldEnd();
  xfer += prot_->writeFieldStop();
  xfer += prot_->writeStructEnd();
  return xfer;
}

} // cpp2
namespace apache { namespace thrift {

}} // apache::thrift
namespace cpp2 {

template <class Protocol_>
uint32_t NewStructureNested::read(Protocol_* iprot) {
  uint32_t xfer = 0;
  std::string _fname;
  apache::thrift::protocol::TType _ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(_fname);

  using apache::thrift::TProtocolException;


  while (true) {
    xfer += iprot->readFieldBegin(_fname, _ftype, fid);
    if (_ftype == apache::thrift::protocol::T_STOP) {
      break;
    }
    if (fid == std::numeric_limits<int16_t>::min()) {
      this->translateFieldName(_fname, fid, _ftype);
    }
    switch (fid) {
      case 1:
      {
        if (_ftype == apache::thrift::protocol::T_LIST) {
          this->lst = std::vector< ::cpp2::FloatFeatures>();
          xfer += ::apache::thrift::detail::pm::protocol_methods< ::apache::thrift::type_class::list<::apache::thrift::type_class::map_forward_compatibility<::apache::thrift::type_class::integral, ::apache::thrift::type_class::floating_point>>, std::vector< ::cpp2::FloatFeatures>>::read(*iprot, this->lst);
          this->__isset.lst = true;
        } else {
          xfer += iprot->skip(_ftype);
        }
        break;
      }
      case 2:
      {
        if (_ftype == apache::thrift::protocol::T_MAP) {
          this->mp = std::map<int16_t,  ::cpp2::FloatFeatures>();
          xfer += ::apache::thrift::detail::pm::protocol_methods< ::apache::thrift::type_class::map<::apache::thrift::type_class::integral, ::apache::thrift::type_class::map_forward_compatibility<::apache::thrift::type_class::integral, ::apache::thrift::type_class::floating_point>>, std::map<int16_t,  ::cpp2::FloatFeatures>>::read(*iprot, this->mp);
          this->__isset.mp = true;
        } else {
          xfer += iprot->skip(_ftype);
        }
        break;
      }
      case 3:
      {
        if (_ftype == apache::thrift::protocol::T_SET) {
          this->s = std::set< ::cpp2::FloatFeatures>();
          xfer += ::apache::thrift::detail::pm::protocol_methods< ::apache::thrift::type_class::set<::apache::thrift::type_class::map_forward_compatibility<::apache::thrift::type_class::integral, ::apache::thrift::type_class::floating_point>>, std::set< ::cpp2::FloatFeatures>>::read(*iprot, this->s);
          this->__isset.s = true;
        } else {
          xfer += iprot->skip(_ftype);
        }
        break;
      }
      default:
      {
        xfer += iprot->skip(_ftype);
        break;
      }
    }
    xfer += iprot->readFieldEnd();
  }
  xfer += iprot->readStructEnd();

  return xfer;
}

template <class Protocol_>
uint32_t NewStructureNested::serializedSize(Protocol_ const* prot_) const {
  uint32_t xfer = 0;
  xfer += prot_->serializedStructSize("NewStructureNested");
  xfer += prot_->serializedFieldSize("lst", apache::thrift::protocol::T_LIST, 1);
  xfer += ::apache::thrift::detail::pm::protocol_methods< ::apache::thrift::type_class::list<::apache::thrift::type_class::map_forward_compatibility<::apache::thrift::type_class::integral, ::apache::thrift::type_class::floating_point>>, std::vector< ::cpp2::FloatFeatures>>::serializedSize<false>(*prot_, this->lst);
  xfer += prot_->serializedFieldSize("mp", apache::thrift::protocol::T_MAP, 2);
  xfer += ::apache::thrift::detail::pm::protocol_methods< ::apache::thrift::type_class::map<::apache::thrift::type_class::integral, ::apache::thrift::type_class::map_forward_compatibility<::apache::thrift::type_class::integral, ::apache::thrift::type_class::floating_point>>, std::map<int16_t,  ::cpp2::FloatFeatures>>::serializedSize<false>(*prot_, this->mp);
  xfer += prot_->serializedFieldSize("s", apache::thrift::protocol::T_SET, 3);
  xfer += ::apache::thrift::detail::pm::protocol_methods< ::apache::thrift::type_class::set<::apache::thrift::type_class::map_forward_compatibility<::apache::thrift::type_class::integral, ::apache::thrift::type_class::floating_point>>, std::set< ::cpp2::FloatFeatures>>::serializedSize<false>(*prot_, this->s);
  xfer += prot_->serializedSizeStop();
  return xfer;
}

template <class Protocol_>
uint32_t NewStructureNested::serializedSizeZC(Protocol_ const* prot_) const {
  uint32_t xfer = 0;
  xfer += prot_->serializedStructSize("NewStructureNested");
  xfer += prot_->serializedFieldSize("lst", apache::thrift::protocol::T_LIST, 1);
  xfer += ::apache::thrift::detail::pm::protocol_methods< ::apache::thrift::type_class::list<::apache::thrift::type_class::map_forward_compatibility<::apache::thrift::type_class::integral, ::apache::thrift::type_class::floating_point>>, std::vector< ::cpp2::FloatFeatures>>::serializedSize<false>(*prot_, this->lst);
  xfer += prot_->serializedFieldSize("mp", apache::thrift::protocol::T_MAP, 2);
  xfer += ::apache::thrift::detail::pm::protocol_methods< ::apache::thrift::type_class::map<::apache::thrift::type_class::integral, ::apache::thrift::type_class::map_forward_compatibility<::apache::thrift::type_class::integral, ::apache::thrift::type_class::floating_point>>, std::map<int16_t,  ::cpp2::FloatFeatures>>::serializedSize<false>(*prot_, this->mp);
  xfer += prot_->serializedFieldSize("s", apache::thrift::protocol::T_SET, 3);
  xfer += ::apache::thrift::detail::pm::protocol_methods< ::apache::thrift::type_class::set<::apache::thrift::type_class::map_forward_compatibility<::apache::thrift::type_class::integral, ::apache::thrift::type_class::floating_point>>, std::set< ::cpp2::FloatFeatures>>::serializedSize<false>(*prot_, this->s);
  xfer += prot_->serializedSizeStop();
  return xfer;
}

template <class Protocol_>
uint32_t NewStructureNested::write(Protocol_* prot_) const {
  uint32_t xfer = 0;
  xfer += prot_->writeStructBegin("NewStructureNested");
  xfer += prot_->writeFieldBegin("lst", apache::thrift::protocol::T_LIST, 1);
  xfer += ::apache::thrift::detail::pm::protocol_methods< ::apache::thrift::type_class::list<::apache::thrift::type_class::map_forward_compatibility<::apache::thrift::type_class::integral, ::apache::thrift::type_class::floating_point>>, std::vector< ::cpp2::FloatFeatures>>::write(*prot_, this->lst);
  xfer += prot_->writeFieldEnd();
  xfer += prot_->writeFieldBegin("mp", apache::thrift::protocol::T_MAP, 2);
  xfer += ::apache::thrift::detail::pm::protocol_methods< ::apache::thrift::type_class::map<::apache::thrift::type_class::integral, ::apache::thrift::type_class::map_forward_compatibility<::apache::thrift::type_class::integral, ::apache::thrift::type_class::floating_point>>, std::map<int16_t,  ::cpp2::FloatFeatures>>::write(*prot_, this->mp);
  xfer += prot_->writeFieldEnd();
  xfer += prot_->writeFieldBegin("s", apache::thrift::protocol::T_SET, 3);
  xfer += ::apache::thrift::detail::pm::protocol_methods< ::apache::thrift::type_class::set<::apache::thrift::type_class::map_forward_compatibility<::apache::thrift::type_class::integral, ::apache::thrift::type_class::floating_point>>, std::set< ::cpp2::FloatFeatures>>::write(*prot_, this->s);
  xfer += prot_->writeFieldEnd();
  xfer += prot_->writeFieldStop();
  xfer += prot_->writeStructEnd();
  return xfer;
}

} // cpp2
namespace apache { namespace thrift {

}} // apache::thrift
namespace cpp2 {

template <class Protocol_>
uint32_t NewStructureNestedField::read(Protocol_* iprot) {
  uint32_t xfer = 0;
  std::string _fname;
  apache::thrift::protocol::TType _ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(_fname);

  using apache::thrift::TProtocolException;


  while (true) {
    xfer += iprot->readFieldBegin(_fname, _ftype, fid);
    if (_ftype == apache::thrift::protocol::T_STOP) {
      break;
    }
    if (fid == std::numeric_limits<int16_t>::min()) {
      this->translateFieldName(_fname, fid, _ftype);
    }
    switch (fid) {
      case 1:
      {
        if (_ftype == apache::thrift::protocol::T_STRUCT) {
          xfer += ::apache::thrift::Cpp2Ops<  ::cpp2::NewStructureNested>::read(iprot, &this->f);
          this->__isset.f = true;
        } else {
          xfer += iprot->skip(_ftype);
        }
        break;
      }
      default:
      {
        xfer += iprot->skip(_ftype);
        break;
      }
    }
    xfer += iprot->readFieldEnd();
  }
  xfer += iprot->readStructEnd();

  return xfer;
}

template <class Protocol_>
uint32_t NewStructureNestedField::serializedSize(Protocol_ const* prot_) const {
  uint32_t xfer = 0;
  xfer += prot_->serializedStructSize("NewStructureNestedField");
  xfer += prot_->serializedFieldSize("f", apache::thrift::protocol::T_STRUCT, 1);
  xfer += ::apache::thrift::Cpp2Ops<  ::cpp2::NewStructureNested>::serializedSize(prot_, &this->f);
  xfer += prot_->serializedSizeStop();
  return xfer;
}

template <class Protocol_>
uint32_t NewStructureNestedField::serializedSizeZC(Protocol_ const* prot_) const {
  uint32_t xfer = 0;
  xfer += prot_->serializedStructSize("NewStructureNestedField");
  xfer += prot_->serializedFieldSize("f", apache::thrift::protocol::T_STRUCT, 1);
  xfer += ::apache::thrift::Cpp2Ops<  ::cpp2::NewStructureNested>::serializedSizeZC(prot_, &this->f);
  xfer += prot_->serializedSizeStop();
  return xfer;
}

template <class Protocol_>
uint32_t NewStructureNestedField::write(Protocol_* prot_) const {
  uint32_t xfer = 0;
  xfer += prot_->writeStructBegin("NewStructureNestedField");
  xfer += prot_->writeFieldBegin("f", apache::thrift::protocol::T_STRUCT, 1);
  xfer += ::apache::thrift::Cpp2Ops<  ::cpp2::NewStructureNested>::write(prot_, &this->f);
  xfer += prot_->writeFieldEnd();
  xfer += prot_->writeFieldStop();
  xfer += prot_->writeStructEnd();
  return xfer;
}

} // cpp2
namespace apache { namespace thrift {

}} // apache::thrift
namespace cpp2 {

} // cpp2
