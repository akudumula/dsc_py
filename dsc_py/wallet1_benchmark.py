import grpc
import pool_pb2_grpc
import pool_pb2
import blockchain_pb2
import blockchain_pb2_grpc
import time
import hashlib
import base58
import yaml
import os
from cryptography.hazmat.backends import default_backend
from cryptography.hazmat.primitives.asymmetric import ec
from cryptography.hazmat.primitives import serialization
from collections import OrderedDict
import random
import secrets
import statistics

def read_one_block(filename):
    with open(filename,'r') as f:
        output=yaml.safe_load(f)

    return(output)

output = read_one_block("dsc_py/dsc-config-testing.yaml")
private_key = read_one_block("dsc_py/dsc-key-wallet1.yaml")["private_key"]
results=[0] * 128

def run():
    pool_server = output["pool"]["server"]
    pool_port = str(output["pool"]["port"])
    channel = grpc.insecure_channel(pool_server+":"+pool_port)
    stub = pool_pb2_grpc.poolStub(channel)
    
    sender = output["wallet1"]["public_key"]
    signature = private_key

    
    for i in range(0,128):
        recepient_address = output[f"wallet{random.randint(2, 8)}"]["public_key"]
        transaction_id = secrets.token_hex(8)
        start_time = time.time()
        send_request = pool_pb2.sendRequest(sender_public_address = sender,recepeint_pubic_address=recepient_address,value = 100,timestamp = int(time.time()),transaction_id = transaction_id, signature = signature)
        send_replies = stub.send(send_request)
        for send_reply in send_replies:
            print("Response recieved: ")
            print(send_reply)

        results[i] = time.time() - start_time

        

if __name__ == "__main__":
    run()
    print(f"average = {statistics.mean(results)}")
    print(f"Minimum = {min(results)}")
    print(f"Maximum = {max}")

