/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: sdmessage.proto */

/* Do not generate deprecated warnings for self */
#ifndef PROTOBUF_C__NO_DEPRECATED
#define PROTOBUF_C__NO_DEPRECATED
#endif

#include "sdmessage.pb-c.h"
void   message_t__entry__init
                     (MessageT__Entry         *message)
{
  static const MessageT__Entry init_value = MESSAGE_T__ENTRY__INIT;
  *message = init_value;
}
void   message_t__init
                     (MessageT         *message)
{
  static const MessageT init_value = MESSAGE_T__INIT;
  *message = init_value;
}
size_t message_t__get_packed_size
                     (const MessageT *message)
{
  assert(message->base.descriptor == &message_t__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t message_t__pack
                     (const MessageT *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &message_t__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t message_t__pack_to_buffer
                     (const MessageT *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &message_t__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
MessageT *
       message_t__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (MessageT *)
     protobuf_c_message_unpack (&message_t__descriptor,
                                allocator, len, data);
}
void   message_t__free_unpacked
                     (MessageT *message,
                      ProtobufCAllocator *allocator)
{
  if(!message)
    return;
  assert(message->base.descriptor == &message_t__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
static const ProtobufCFieldDescriptor message_t__entry__field_descriptors[2] =
{
  {
    "key",
    1,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(MessageT__Entry, key),
    NULL,
    &protobuf_c_empty_string,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "data",
    2,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_BYTES,
    0,   /* quantifier_offset */
    offsetof(MessageT__Entry, data),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned message_t__entry__field_indices_by_name[] = {
  1,   /* field[1] = data */
  0,   /* field[0] = key */
};
static const ProtobufCIntRange message_t__entry__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 2 }
};
const ProtobufCMessageDescriptor message_t__entry__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "message_t.Entry",
  "Entry",
  "MessageT__Entry",
  "",
  sizeof(MessageT__Entry),
  2,
  message_t__entry__field_descriptors,
  message_t__entry__field_indices_by_name,
  1,  message_t__entry__number_ranges,
  (ProtobufCMessageInit) message_t__entry__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCEnumValue message_t__opcode__enum_values_by_number[9] =
{
  { "OP_BAD", "MESSAGE_T__OPCODE__OP_BAD", 0 },
  { "OP_SIZE", "MESSAGE_T__OPCODE__OP_SIZE", 10 },
  { "OP_DEL", "MESSAGE_T__OPCODE__OP_DEL", 20 },
  { "OP_GET", "MESSAGE_T__OPCODE__OP_GET", 30 },
  { "OP_PUT", "MESSAGE_T__OPCODE__OP_PUT", 40 },
  { "OP_GETKEYS", "MESSAGE_T__OPCODE__OP_GETKEYS", 50 },
  { "OP_PRINT", "MESSAGE_T__OPCODE__OP_PRINT", 60 },
  { "OP_STATS", "MESSAGE_T__OPCODE__OP_STATS", 70 },
  { "OP_ERROR", "MESSAGE_T__OPCODE__OP_ERROR", 99 },
};
static const ProtobufCIntRange message_t__opcode__value_ranges[] = {
{0, 0},{10, 1},{20, 2},{30, 3},{40, 4},{50, 5},{60, 6},{70, 7},{99, 8},{0, 9}
};
static const ProtobufCEnumValueIndex message_t__opcode__enum_values_by_name[9] =
{
  { "OP_BAD", 0 },
  { "OP_DEL", 2 },
  { "OP_ERROR", 8 },
  { "OP_GET", 3 },
  { "OP_GETKEYS", 5 },
  { "OP_PRINT", 6 },
  { "OP_PUT", 4 },
  { "OP_SIZE", 1 },
  { "OP_STATS", 7 },
};
const ProtobufCEnumDescriptor message_t__opcode__descriptor =
{
  PROTOBUF_C__ENUM_DESCRIPTOR_MAGIC,
  "message_t.Opcode",
  "Opcode",
  "MessageT__Opcode",
  "",
  9,
  message_t__opcode__enum_values_by_number,
  9,
  message_t__opcode__enum_values_by_name,
  9,
  message_t__opcode__value_ranges,
  NULL,NULL,NULL,NULL   /* reserved[1234] */
};
static const ProtobufCEnumValue message_t__c_type__enum_values_by_number[9] =
{
  { "CT_BAD", "MESSAGE_T__C_TYPE__CT_BAD", 0 },
  { "CT_KEY", "MESSAGE_T__C_TYPE__CT_KEY", 10 },
  { "CT_VALUE", "MESSAGE_T__C_TYPE__CT_VALUE", 20 },
  { "CT_ENTRY", "MESSAGE_T__C_TYPE__CT_ENTRY", 30 },
  { "CT_KEYS", "MESSAGE_T__C_TYPE__CT_KEYS", 40 },
  { "CT_RESULT", "MESSAGE_T__C_TYPE__CT_RESULT", 50 },
  { "CT_TABLE", "MESSAGE_T__C_TYPE__CT_TABLE", 60 },
  { "CT_NONE", "MESSAGE_T__C_TYPE__CT_NONE", 70 },
  { "CT_STATS", "MESSAGE_T__C_TYPE__CT_STATS", 80 },
};
static const ProtobufCIntRange message_t__c_type__value_ranges[] = {
{0, 0},{10, 1},{20, 2},{30, 3},{40, 4},{50, 5},{60, 6},{70, 7},{80, 8},{0, 9}
};
static const ProtobufCEnumValueIndex message_t__c_type__enum_values_by_name[9] =
{
  { "CT_BAD", 0 },
  { "CT_ENTRY", 3 },
  { "CT_KEY", 1 },
  { "CT_KEYS", 4 },
  { "CT_NONE", 7 },
  { "CT_RESULT", 5 },
  { "CT_STATS", 8 },
  { "CT_TABLE", 6 },
  { "CT_VALUE", 2 },
};
const ProtobufCEnumDescriptor message_t__c_type__descriptor =
{
  PROTOBUF_C__ENUM_DESCRIPTOR_MAGIC,
  "message_t.C_type",
  "C_type",
  "MessageT__CType",
  "",
  9,
  message_t__c_type__enum_values_by_number,
  9,
  message_t__c_type__enum_values_by_name,
  9,
  message_t__c_type__value_ranges,
  NULL,NULL,NULL,NULL   /* reserved[1234] */
};
static const ProtobufCFieldDescriptor message_t__field_descriptors[8] =
{
  {
    "opcode",
    1,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_ENUM,
    0,   /* quantifier_offset */
    offsetof(MessageT, opcode),
    &message_t__opcode__descriptor,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "c_type",
    2,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_ENUM,
    0,   /* quantifier_offset */
    offsetof(MessageT, c_type),
    &message_t__c_type__descriptor,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "keys",
    3,
    PROTOBUF_C_LABEL_REPEATED,
    PROTOBUF_C_TYPE_STRING,
    offsetof(MessageT, n_keys),
    offsetof(MessageT, keys),
    NULL,
    &protobuf_c_empty_string,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "data",
    4,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_BYTES,
    0,   /* quantifier_offset */
    offsetof(MessageT, data),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "result",
    5,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_INT32,
    0,   /* quantifier_offset */
    offsetof(MessageT, result),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "entries",
    6,
    PROTOBUF_C_LABEL_REPEATED,
    PROTOBUF_C_TYPE_MESSAGE,
    offsetof(MessageT, n_entries),
    offsetof(MessageT, entries),
    &message_t__entry__descriptor,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "stats",
    7,
    PROTOBUF_C_LABEL_REPEATED,
    PROTOBUF_C_TYPE_INT32,
    offsetof(MessageT, n_stats),
    offsetof(MessageT, stats),
    NULL,
    NULL,
    0 | PROTOBUF_C_FIELD_FLAG_PACKED,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "avg_time",
    8,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_FLOAT,
    0,   /* quantifier_offset */
    offsetof(MessageT, avg_time),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned message_t__field_indices_by_name[] = {
  7,   /* field[7] = avg_time */
  1,   /* field[1] = c_type */
  3,   /* field[3] = data */
  5,   /* field[5] = entries */
  2,   /* field[2] = keys */
  0,   /* field[0] = opcode */
  4,   /* field[4] = result */
  6,   /* field[6] = stats */
};
static const ProtobufCIntRange message_t__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 8 }
};
const ProtobufCMessageDescriptor message_t__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "message_t",
  "MessageT",
  "MessageT",
  "",
  sizeof(MessageT),
  8,
  message_t__field_descriptors,
  message_t__field_indices_by_name,
  1,  message_t__number_ranges,
  (ProtobufCMessageInit) message_t__init,
  NULL,NULL,NULL    /* reserved[123] */
};
