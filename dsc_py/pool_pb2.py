# -*- coding: utf-8 -*-
# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: pool.proto
# Protobuf Python Version: 4.25.0
"""Generated protocol buffer code."""
from google.protobuf import descriptor as _descriptor
from google.protobuf import descriptor_pool as _descriptor_pool
from google.protobuf import symbol_database as _symbol_database
from google.protobuf.internal import builder as _builder
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()




DESCRIPTOR = _descriptor_pool.Default().AddSerializedFile(b'\n\npool.proto\x12\nadarsh.dsc\"\x9a\x01\n\x0bsendRequest\x12\x1d\n\x15sender_public_address\x18\x01 \x01(\t\x12\x1f\n\x17recepeint_pubic_address\x18\x02 \x01(\t\x12\r\n\x05value\x18\x03 \x01(\x01\x12\x11\n\ttimestamp\x18\x04 \x01(\x05\x12\x16\n\x0etransaction_id\x18\x05 \x01(\t\x12\x11\n\tsignature\x18\x06 \x01(\t\"\x1a\n\tsendReply\x12\r\n\x05reply\x18\x01 \x01(\t\",\n\x12transactionRequest\x12\x16\n\x0etransaction_id\x18\x01 \x01(\t\"0\n\x13transactionsRequest\x12\x19\n\x11wallet_public_key\x18\x01 \x01(\t\"\"\n\x10transactionReply\x12\x0e\n\x06status\x18\x01 \x01(\t2\xdc\x01\n\x04pool\x12\x38\n\x04send\x12\x17.adarsh.dsc.sendRequest\x1a\x15.adarsh.dsc.sendReply0\x01\x12K\n\x0btransaction\x12\x1e.adarsh.dsc.transactionRequest\x1a\x1c.adarsh.dsc.transactionReply\x12M\n\x0ctransactions\x12\x1f.adarsh.dsc.transactionsRequest\x1a\x1c.adarsh.dsc.transactionReplyb\x06proto3')

_globals = globals()
_builder.BuildMessageAndEnumDescriptors(DESCRIPTOR, _globals)
_builder.BuildTopDescriptorsAndMessages(DESCRIPTOR, 'pool_pb2', _globals)
if _descriptor._USE_C_DESCRIPTORS == False:
  DESCRIPTOR._options = None
  _globals['_SENDREQUEST']._serialized_start=27
  _globals['_SENDREQUEST']._serialized_end=181
  _globals['_SENDREPLY']._serialized_start=183
  _globals['_SENDREPLY']._serialized_end=209
  _globals['_TRANSACTIONREQUEST']._serialized_start=211
  _globals['_TRANSACTIONREQUEST']._serialized_end=255
  _globals['_TRANSACTIONSREQUEST']._serialized_start=257
  _globals['_TRANSACTIONSREQUEST']._serialized_end=305
  _globals['_TRANSACTIONREPLY']._serialized_start=307
  _globals['_TRANSACTIONREPLY']._serialized_end=341
  _globals['_POOL']._serialized_start=344
  _globals['_POOL']._serialized_end=564
# @@protoc_insertion_point(module_scope)