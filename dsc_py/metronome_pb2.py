# -*- coding: utf-8 -*-
# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: metronome.proto
# Protobuf Python Version: 4.25.0
"""Generated protocol buffer code."""
from google.protobuf import descriptor as _descriptor
from google.protobuf import descriptor_pool as _descriptor_pool
from google.protobuf import symbol_database as _symbol_database
from google.protobuf.internal import builder as _builder
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()




DESCRIPTOR = _descriptor_pool.Default().AddSerializedFile(b'\n\x0fmetronome.proto\x12\nadarsh.dsc\"#\n\x10validatorRequest\x12\x0f\n\x07request\x18\x01 \x01(\x05\"_\n\x11validatorResponse\x12\x13\n\x0bresult_hash\x18\x01 \x01(\t\x12\x12\n\ndifficulty\x18\x02 \x01(\x05\x12\x12\n\nblock_time\x18\x03 \x01(\x05\x12\r\n\x05timer\x18\x04 \x01(\x01\x32W\n\tmetronome\x12J\n\x0bget_details\x12\x1c.adarsh.dsc.validatorRequest\x1a\x1d.adarsh.dsc.validatorResponseb\x06proto3')

_globals = globals()
_builder.BuildMessageAndEnumDescriptors(DESCRIPTOR, _globals)
_builder.BuildTopDescriptorsAndMessages(DESCRIPTOR, 'metronome_pb2', _globals)
if _descriptor._USE_C_DESCRIPTORS == False:
  DESCRIPTOR._options = None
  _globals['_VALIDATORREQUEST']._serialized_start=31
  _globals['_VALIDATORREQUEST']._serialized_end=66
  _globals['_VALIDATORRESPONSE']._serialized_start=68
  _globals['_VALIDATORRESPONSE']._serialized_end=163
  _globals['_METRONOME']._serialized_start=165
  _globals['_METRONOME']._serialized_end=252
# @@protoc_insertion_point(module_scope)
