// Generated by the protocol buffer compiler.  DO NOT EDIT!

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "Assets.serializer.h"

#include <algorithm>

#include "once.h"
#include "coded_stream.h"
#include "wire_format_lite_inl.h"
// @@protoc_insertion_point(includes)

namespace Serializer {

void protobuf_ShutdownFile_Assets_2eproto() {
  delete RuntimeAsset::default_instance_;
}

void protobuf_AddDesc_Assets_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  RuntimeAsset::default_instance_ = new RuntimeAsset();
  RuntimeAsset::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_Assets_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_Assets_2eproto {
  StaticDescriptorInitializer_Assets_2eproto() {
    protobuf_AddDesc_Assets_2eproto();
  }
} static_descriptor_initializer_Assets_2eproto_;


// ===================================================================

#ifndef _MSC_VER
const int RuntimeAsset::kTypeFieldNumber;
const int RuntimeAsset::kGuidFieldNumber;
const int RuntimeAsset::kRawdataFieldNumber;
#endif  // !_MSC_VER

RuntimeAsset::RuntimeAsset()
  : ::google::protobuf::MessageLite() {
  SharedCtor();
}

void RuntimeAsset::InitAsDefaultInstance() {
}

RuntimeAsset::RuntimeAsset(const RuntimeAsset& from)
  : ::google::protobuf::MessageLite() {
  SharedCtor();
  MergeFrom(from);
}

void RuntimeAsset::SharedCtor() {
  _cached_size_ = 0;
  type_ = 0;
  guid_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  rawdata_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

RuntimeAsset::~RuntimeAsset() {
  SharedDtor();
}

void RuntimeAsset::SharedDtor() {
  if (guid_ != &::google::protobuf::internal::kEmptyString) {
    delete guid_;
  }
  if (rawdata_ != &::google::protobuf::internal::kEmptyString) {
    delete rawdata_;
  }
  if (this != default_instance_) {
  }
}

void RuntimeAsset::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const RuntimeAsset& RuntimeAsset::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_Assets_2eproto();  return *default_instance_;
}

RuntimeAsset* RuntimeAsset::default_instance_ = NULL;

RuntimeAsset* RuntimeAsset::New() const {
  return new RuntimeAsset;
}

void RuntimeAsset::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    type_ = 0;
    if (has_guid()) {
      if (guid_ != &::google::protobuf::internal::kEmptyString) {
        guid_->clear();
      }
    }
    if (has_rawdata()) {
      if (rawdata_ != &::google::protobuf::internal::kEmptyString) {
        rawdata_->clear();
      }
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

bool RuntimeAsset::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required int32 type = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &type_)));
          set_has_type();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(18)) goto parse_guid;
        break;
      }
      
      // required string guid = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_guid:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_guid()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(26)) goto parse_rawdata;
        break;
      }
      
      // required bytes rawdata = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_rawdata:
          DO_(::google::protobuf::internal::WireFormatLite::ReadBytes(
                input, this->mutable_rawdata()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectAtEnd()) return true;
        break;
      }
      
      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormatLite::SkipField(input, tag));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void RuntimeAsset::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required int32 type = 1;
  if (has_type()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(1, this->type(), output);
  }
  
  // required string guid = 2;
  if (has_guid()) {
    ::google::protobuf::internal::WireFormatLite::WriteString(
      2, this->guid(), output);
  }
  
  // required bytes rawdata = 3;
  if (has_rawdata()) {
    ::google::protobuf::internal::WireFormatLite::WriteBytes(
      3, this->rawdata(), output);
  }
  
}

int RuntimeAsset::ByteSize() const {
  int total_size = 0;
  
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required int32 type = 1;
    if (has_type()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->type());
    }
    
    // required string guid = 2;
    if (has_guid()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->guid());
    }
    
    // required bytes rawdata = 3;
    if (has_rawdata()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::BytesSize(
          this->rawdata());
    }
    
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void RuntimeAsset::CheckTypeAndMergeFrom(
    const ::google::protobuf::MessageLite& from) {
  MergeFrom(*::google::protobuf::down_cast<const RuntimeAsset*>(&from));
}

void RuntimeAsset::MergeFrom(const RuntimeAsset& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_type()) {
      set_type(from.type());
    }
    if (from.has_guid()) {
      set_guid(from.guid());
    }
    if (from.has_rawdata()) {
      set_rawdata(from.rawdata());
    }
  }
}

void RuntimeAsset::CopyFrom(const RuntimeAsset& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool RuntimeAsset::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000007) != 0x00000007) return false;
  
  return true;
}

void RuntimeAsset::Swap(RuntimeAsset* other) {
  if (other != this) {
    std::swap(type_, other->type_);
    std::swap(guid_, other->guid_);
    std::swap(rawdata_, other->rawdata_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::std::string RuntimeAsset::GetTypeName() const {
  return "Serializer.RuntimeAsset";
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace Serializer

// @@protoc_insertion_point(global_scope)
