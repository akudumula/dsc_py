# Generated by the gRPC Python protocol compiler plugin. DO NOT EDIT!
"""Client and server classes corresponding to protobuf-defined services."""
import grpc

import blockchain_pb2 as blockchain__pb2


class blockchainStub(object):
    """Missing associated documentation comment in .proto file."""

    def __init__(self, channel):
        """Constructor.

        Args:
            channel: A grpc.Channel.
        """
        self.balance = channel.unary_unary(
                '/adarsh.dsc.blockchain/balance',
                request_serializer=blockchain__pb2.balanceRequest.SerializeToString,
                response_deserializer=blockchain__pb2.balanceReply.FromString,
                )
        self.block_creation = channel.unary_unary(
                '/adarsh.dsc.blockchain/block_creation',
                request_serializer=blockchain__pb2.blockRequest.SerializeToString,
                response_deserializer=blockchain__pb2.response.FromString,
                )
        self.metronome_blockHash = channel.unary_unary(
                '/adarsh.dsc.blockchain/metronome_blockHash',
                request_serializer=blockchain__pb2.new_block_request.SerializeToString,
                response_deserializer=blockchain__pb2.new_block_response.FromString,
                )


class blockchainServicer(object):
    """Missing associated documentation comment in .proto file."""

    def balance(self, request, context):
        """Missing associated documentation comment in .proto file."""
        context.set_code(grpc.StatusCode.UNIMPLEMENTED)
        context.set_details('Method not implemented!')
        raise NotImplementedError('Method not implemented!')

    def block_creation(self, request, context):
        """Missing associated documentation comment in .proto file."""
        context.set_code(grpc.StatusCode.UNIMPLEMENTED)
        context.set_details('Method not implemented!')
        raise NotImplementedError('Method not implemented!')

    def metronome_blockHash(self, request, context):
        """Missing associated documentation comment in .proto file."""
        context.set_code(grpc.StatusCode.UNIMPLEMENTED)
        context.set_details('Method not implemented!')
        raise NotImplementedError('Method not implemented!')


def add_blockchainServicer_to_server(servicer, server):
    rpc_method_handlers = {
            'balance': grpc.unary_unary_rpc_method_handler(
                    servicer.balance,
                    request_deserializer=blockchain__pb2.balanceRequest.FromString,
                    response_serializer=blockchain__pb2.balanceReply.SerializeToString,
            ),
            'block_creation': grpc.unary_unary_rpc_method_handler(
                    servicer.block_creation,
                    request_deserializer=blockchain__pb2.blockRequest.FromString,
                    response_serializer=blockchain__pb2.response.SerializeToString,
            ),
            'metronome_blockHash': grpc.unary_unary_rpc_method_handler(
                    servicer.metronome_blockHash,
                    request_deserializer=blockchain__pb2.new_block_request.FromString,
                    response_serializer=blockchain__pb2.new_block_response.SerializeToString,
            ),
    }
    generic_handler = grpc.method_handlers_generic_handler(
            'adarsh.dsc.blockchain', rpc_method_handlers)
    server.add_generic_rpc_handlers((generic_handler,))


 # This class is part of an EXPERIMENTAL API.
class blockchain(object):
    """Missing associated documentation comment in .proto file."""

    @staticmethod
    def balance(request,
            target,
            options=(),
            channel_credentials=None,
            call_credentials=None,
            insecure=False,
            compression=None,
            wait_for_ready=None,
            timeout=None,
            metadata=None):
        return grpc.experimental.unary_unary(request, target, '/adarsh.dsc.blockchain/balance',
            blockchain__pb2.balanceRequest.SerializeToString,
            blockchain__pb2.balanceReply.FromString,
            options, channel_credentials,
            insecure, call_credentials, compression, wait_for_ready, timeout, metadata)

    @staticmethod
    def block_creation(request,
            target,
            options=(),
            channel_credentials=None,
            call_credentials=None,
            insecure=False,
            compression=None,
            wait_for_ready=None,
            timeout=None,
            metadata=None):
        return grpc.experimental.unary_unary(request, target, '/adarsh.dsc.blockchain/block_creation',
            blockchain__pb2.blockRequest.SerializeToString,
            blockchain__pb2.response.FromString,
            options, channel_credentials,
            insecure, call_credentials, compression, wait_for_ready, timeout, metadata)

    @staticmethod
    def metronome_blockHash(request,
            target,
            options=(),
            channel_credentials=None,
            call_credentials=None,
            insecure=False,
            compression=None,
            wait_for_ready=None,
            timeout=None,
            metadata=None):
        return grpc.experimental.unary_unary(request, target, '/adarsh.dsc.blockchain/metronome_blockHash',
            blockchain__pb2.new_block_request.SerializeToString,
            blockchain__pb2.new_block_response.FromString,
            options, channel_credentials,
            insecure, call_credentials, compression, wait_for_ready, timeout, metadata)