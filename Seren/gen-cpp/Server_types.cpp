/**
 * Autogenerated by Thrift Compiler (0.13.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#include "Server_types.h"

#include <algorithm>
#include <ostream>

#include <thrift/TToString.h>

namespace server {


SerialisedNode::~SerialisedNode() noexcept {
}


void SerialisedNode::__set_index(const int64_t val) {
  this->index = val;
}

void SerialisedNode::__set_level(const int32_t val) {
  this->level = val;
}

void SerialisedNode::__set_key(const std::string& val) {
  this->key = val;
}
std::ostream& operator<<(std::ostream& out, const SerialisedNode& obj)
{
  obj.printTo(out);
  return out;
}


uint32_t SerialisedNode::read(::apache::thrift::protocol::TProtocol* iprot) {

  ::apache::thrift::protocol::TInputRecursionTracker tracker(*iprot);
  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_I64) {
          xfer += iprot->readI64(this->index);
          this->__isset.index = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->level);
          this->__isset.level = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readBinary(this->key);
          this->__isset.key = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t SerialisedNode::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  ::apache::thrift::protocol::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("SerialisedNode");

  xfer += oprot->writeFieldBegin("index", ::apache::thrift::protocol::T_I64, 1);
  xfer += oprot->writeI64(this->index);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("level", ::apache::thrift::protocol::T_I32, 2);
  xfer += oprot->writeI32(this->level);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("key", ::apache::thrift::protocol::T_STRING, 3);
  xfer += oprot->writeBinary(this->key);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(SerialisedNode &a, SerialisedNode &b) {
  using ::std::swap;
  swap(a.index, b.index);
  swap(a.level, b.level);
  swap(a.key, b.key);
  swap(a.__isset, b.__isset);
}

SerialisedNode::SerialisedNode(const SerialisedNode& other0) {
  index = other0.index;
  level = other0.level;
  key = other0.key;
  __isset = other0.__isset;
}
SerialisedNode& SerialisedNode::operator=(const SerialisedNode& other1) {
  index = other1.index;
  level = other1.level;
  key = other1.key;
  __isset = other1.__isset;
  return *this;
}
void SerialisedNode::printTo(std::ostream& out) const {
  using ::apache::thrift::to_string;
  out << "SerialisedNode(";
  out << "index=" << to_string(index);
  out << ", " << "level=" << to_string(level);
  out << ", " << "key=" << to_string(key);
  out << ")";
}

} // namespace
