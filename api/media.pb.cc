// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: media.proto

#include "media.pb.h"

#include <algorithm>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>

PROTOBUF_PRAGMA_INIT_SEG

namespace _pb = ::PROTOBUF_NAMESPACE_ID;
namespace _pbi = _pb::internal;

namespace pbmedia {
PROTOBUF_CONSTEXPR ComReturn::ComReturn(
    ::_pbi::ConstantInitialized): _impl_{
    /*decltype(_impl_.message_)*/{&::_pbi::fixed_address_empty_string, ::_pbi::ConstantInitialized{}}
  , /*decltype(_impl_.code_)*/0
  , /*decltype(_impl_._cached_size_)*/{}} {}
struct ComReturnDefaultTypeInternal {
  PROTOBUF_CONSTEXPR ComReturnDefaultTypeInternal()
      : _instance(::_pbi::ConstantInitialized{}) {}
  ~ComReturnDefaultTypeInternal() {}
  union {
    ComReturn _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 ComReturnDefaultTypeInternal _ComReturn_default_instance_;
}  // namespace pbmedia
static ::_pb::Metadata file_level_metadata_media_2eproto[1];
static constexpr ::_pb::EnumDescriptor const** file_level_enum_descriptors_media_2eproto = nullptr;
static constexpr ::_pb::ServiceDescriptor const** file_level_service_descriptors_media_2eproto = nullptr;

const uint32_t TableStruct_media_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::pbmedia::ComReturn, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
  PROTOBUF_FIELD_OFFSET(::pbmedia::ComReturn, _impl_.code_),
  PROTOBUF_FIELD_OFFSET(::pbmedia::ComReturn, _impl_.message_),
};
static const ::_pbi::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, -1, sizeof(::pbmedia::ComReturn)},
};

static const ::_pb::Message* const file_default_instances[] = {
  &::pbmedia::_ComReturn_default_instance_._instance,
};

const char descriptor_table_protodef_media_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\013media.proto\022\007pbmedia\"*\n\tComReturn\022\014\n\004C"
  "ode\030\001 \001(\005\022\017\n\007Message\030\002 \001(\tB\023Z\021blueplane/"
  "pbmediab\006proto3"
  ;
static ::_pbi::once_flag descriptor_table_media_2eproto_once;
const ::_pbi::DescriptorTable descriptor_table_media_2eproto = {
    false, false, 95, descriptor_table_protodef_media_2eproto,
    "media.proto",
    &descriptor_table_media_2eproto_once, nullptr, 0, 1,
    schemas, file_default_instances, TableStruct_media_2eproto::offsets,
    file_level_metadata_media_2eproto, file_level_enum_descriptors_media_2eproto,
    file_level_service_descriptors_media_2eproto,
};
PROTOBUF_ATTRIBUTE_WEAK const ::_pbi::DescriptorTable* descriptor_table_media_2eproto_getter() {
  return &descriptor_table_media_2eproto;
}

// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY2 static ::_pbi::AddDescriptorsRunner dynamic_init_dummy_media_2eproto(&descriptor_table_media_2eproto);
namespace pbmedia {

// ===================================================================

class ComReturn::_Internal {
 public:
};

ComReturn::ComReturn(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned) {
  SharedCtor(arena, is_message_owned);
  // @@protoc_insertion_point(arena_constructor:pbmedia.ComReturn)
}
ComReturn::ComReturn(const ComReturn& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  ComReturn* const _this = this; (void)_this;
  new (&_impl_) Impl_{
      decltype(_impl_.message_){}
    , decltype(_impl_.code_){}
    , /*decltype(_impl_._cached_size_)*/{}};

  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  _impl_.message_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.message_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (!from._internal_message().empty()) {
    _this->_impl_.message_.Set(from._internal_message(), 
      _this->GetArenaForAllocation());
  }
  _this->_impl_.code_ = from._impl_.code_;
  // @@protoc_insertion_point(copy_constructor:pbmedia.ComReturn)
}

inline void ComReturn::SharedCtor(
    ::_pb::Arena* arena, bool is_message_owned) {
  (void)arena;
  (void)is_message_owned;
  new (&_impl_) Impl_{
      decltype(_impl_.message_){}
    , decltype(_impl_.code_){0}
    , /*decltype(_impl_._cached_size_)*/{}
  };
  _impl_.message_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.message_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
}

ComReturn::~ComReturn() {
  // @@protoc_insertion_point(destructor:pbmedia.ComReturn)
  if (auto *arena = _internal_metadata_.DeleteReturnArena<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>()) {
  (void)arena;
    return;
  }
  SharedDtor();
}

inline void ComReturn::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
  _impl_.message_.Destroy();
}

void ComReturn::SetCachedSize(int size) const {
  _impl_._cached_size_.Set(size);
}

void ComReturn::Clear() {
// @@protoc_insertion_point(message_clear_start:pbmedia.ComReturn)
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  _impl_.message_.ClearToEmpty();
  _impl_.code_ = 0;
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* ComReturn::_InternalParse(const char* ptr, ::_pbi::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    uint32_t tag;
    ptr = ::_pbi::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // int32 Code = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 8)) {
          _impl_.code_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint32(&ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // string Message = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 18)) {
          auto str = _internal_mutable_message();
          ptr = ::_pbi::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(ptr);
          CHK_(::_pbi::VerifyUTF8(str, "pbmedia.ComReturn.Message"));
        } else
          goto handle_unusual;
        continue;
      default:
        goto handle_unusual;
    }  // switch
  handle_unusual:
    if ((tag == 0) || ((tag & 7) == 4)) {
      CHK_(ptr);
      ctx->SetLastTag(tag);
      goto message_done;
    }
    ptr = UnknownFieldParse(
        tag,
        _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
        ptr, ctx);
    CHK_(ptr != nullptr);
  }  // while
message_done:
  return ptr;
failure:
  ptr = nullptr;
  goto message_done;
#undef CHK_
}

uint8_t* ComReturn::_InternalSerialize(
    uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:pbmedia.ComReturn)
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  // int32 Code = 1;
  if (this->_internal_code() != 0) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteInt32ToArray(1, this->_internal_code(), target);
  }

  // string Message = 2;
  if (!this->_internal_message().empty()) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_message().data(), static_cast<int>(this->_internal_message().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "pbmedia.ComReturn.Message");
    target = stream->WriteStringMaybeAliased(
        2, this->_internal_message(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:pbmedia.ComReturn)
  return target;
}

size_t ComReturn::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:pbmedia.ComReturn)
  size_t total_size = 0;

  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // string Message = 2;
  if (!this->_internal_message().empty()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_message());
  }

  // int32 Code = 1;
  if (this->_internal_code() != 0) {
    total_size += ::_pbi::WireFormatLite::Int32SizePlusOne(this->_internal_code());
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_impl_._cached_size_);
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData ComReturn::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSourceCheck,
    ComReturn::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*ComReturn::GetClassData() const { return &_class_data_; }


void ComReturn::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg) {
  auto* const _this = static_cast<ComReturn*>(&to_msg);
  auto& from = static_cast<const ComReturn&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:pbmedia.ComReturn)
  GOOGLE_DCHECK_NE(&from, _this);
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  if (!from._internal_message().empty()) {
    _this->_internal_set_message(from._internal_message());
  }
  if (from._internal_code() != 0) {
    _this->_internal_set_code(from._internal_code());
  }
  _this->_internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void ComReturn::CopyFrom(const ComReturn& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:pbmedia.ComReturn)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool ComReturn::IsInitialized() const {
  return true;
}

void ComReturn::InternalSwap(ComReturn* other) {
  using std::swap;
  auto* lhs_arena = GetArenaForAllocation();
  auto* rhs_arena = other->GetArenaForAllocation();
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::InternalSwap(
      &_impl_.message_, lhs_arena,
      &other->_impl_.message_, rhs_arena
  );
  swap(_impl_.code_, other->_impl_.code_);
}

::PROTOBUF_NAMESPACE_ID::Metadata ComReturn::GetMetadata() const {
  return ::_pbi::AssignDescriptors(
      &descriptor_table_media_2eproto_getter, &descriptor_table_media_2eproto_once,
      file_level_metadata_media_2eproto[0]);
}

// @@protoc_insertion_point(namespace_scope)
}  // namespace pbmedia
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::pbmedia::ComReturn*
Arena::CreateMaybeMessage< ::pbmedia::ComReturn >(Arena* arena) {
  return Arena::CreateMessageInternal< ::pbmedia::ComReturn >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>