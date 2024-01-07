import time
import threading
import grpc
import blockchain_pb2
import blockchain_pb2_grpc
import yaml
import metronome_pb2
import metronome_pb2_grpc
from datetime import datetime
from concurrent import futures

def read_one_block(filename):
    with open(f'{filename}.yaml','r') as f:
        output=yaml.safe_load(f)

    return(output)

output = read_one_block("dsc-config")

def connect_to_metronome():
    metronome_server = output["metronome"]["server"]
    metronome_port = str(output["metronome"]["port"])
    channel = grpc.insecure_channel(metronome_server+":"+metronome_port)
    stub = metronome_pb2_grpc.metronomeStub(channel)
    metronome_request = metronome_pb2.validatorRequest()
    metronome_request.request = 1
    metronome_response = stub.get_details(metronome_request)

    print(metronome_response)

if __name__ == "__main__":
    connect_to_metronome()
