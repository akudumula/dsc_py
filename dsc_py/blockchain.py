from concurrent import futures
import time

import grpc
import blockchain_pb2
import blockchain_pb2_grpc
import struct
import yaml
from datetime import datetime

# class transaction:
#     def __init__(self,sender,recepient,value,timestamp,transaction_id,signature):
#         self.transaction = {
#             "sender":sender,
#             "recepient":recepient,
#             "value":value,
#             "timestamp":timestamp,
#             "transaction_id":transaction_id,
#             "signature":signature
#         }

# class block:
#     def __init__(self,block_size,version,previous_block_hash,blockID,timestamp,difficulty,nonce,transaction_count,reserved,transactions):
#         self.block = {
#             "block_size":block_size,
#             "version":version,
#             "previous_block_hash":previous_block_hash,
#             "blockID":blockID,
#             "timestamp":timestamp,
#             "difficulty":difficulty,
#             "nonce":nonce,
#             "transaction_count":transaction_count,
#             "reserved":reserved,
#             "transactions":transactions
#         }   

#     def add_transaction(self,transaction):
#         self.block["transactions"].append(transaction)
#         self.block["block_size"] += len(transaction)
#         self.block["transaction_count"] = len(self.block["transactions"])



class transaction:
    def __init__(self,sender,recepient,value,timestamp,transaction_id,signature):
        self.sender = sender
        self.recepient = recepient
        self.value = value
        self.timestamp = timestamp
        self.transaction_id = transaction_id
        self.signature = signature
        
class block_header:
    def __init__(self,version,previous_block_hash,blockID,timestamp,difficulty,nonce):
        self.version = version
        self.previous_block_hash = previous_block_hash
        self.blockID = blockID
        self.timestamp = timestamp
        self.difficulty = difficulty
        self.nonce = nonce

class block:
    def __init__(self,block_size,block_header,transaction_count,reserved,transactions):
        self.block_size = block_size
        self.block_header = block_header
        self.transaction_count = transaction_count
        self.reserved = reserved
        self.transactions = transactions

    def add_transaction(self,transaction):
        self.transactions.append(transaction)
        self.block_size += len(transaction)
        self.transaction_count = len(self.transactions)

    def return_block_header(self):
        return self.block_header
        

class blockchainServicer (blockchain_pb2_grpc.blockchainServicer):
    def balance(self, request, context):
        print("Balance request made")
        print(request)

        balance_reply = blockchain_pb2.balanceReply()
        balance_reply.value = 0.0

        return balance_reply
    def block_creation(self, request, context):
        print("metronome Request recieved")
        print(request)

        block_response = blockchain_pb2.response()
        return block_response
    
    def metronome_blockHash(self, request, context):
        print("metronome Request recieved")
        print(request)

        new_block_response = blockchain_pb2.new_block_response()
        new_block_response.block_hash = "ABCDEFGHIJ"
        new_block_response.blockID = 2

        return new_block_response

block_chain = []
balance_cache = {}

def transaction_verify(transaction_id):
    for block in block_chain[::-1]:
        for transaction in block.transactions:
            if transaction.transaction_id == transaction_id:
                return "Success"
    
    return "Unknown"

def return_balance(public_address):

    if public_address in balance_cache.keys():
        return balance_cache[public_address]["balance"]
    balance = 0
    last_transaction = ""
    last_block =0
    for block in block_chain[::-1]:
        for transaction in block.transactions:
            if transaction.sender == public_address:
                balance -=transaction.value
                last_transaction = transaction.transaction_id
                last_block = block.blockID
            elif transaction.recepient == public_address:
                balance +=transaction.value
                last_transaction = transaction.transaction_id
                last_block = block.blockID
    
    balance_cache[public_address] = {"balance" : balance, "blockID":last_block,"public_address":public_address,"last_transaction":last_transaction}
    return balance

def retrieve_all_transactions(public_address):
    transactions = []
    for block in block_chain[::-1]:
        for transaction in block.transactions:
            if transaction.sender == public_address or transaction.recepient == public_address:
                transactions.append(transaction.transaction_id)
    
    return transactions



def create_genesis_block():
    genesis_block = block(block_size=0,block_header=block_header(version=1,previous_block_hash="",blockID=0,timestamp=0,difficulty=30,nonce=0),transaction_count=0,reserved="",transactions=[])
    return genesis_block

def read_one_block(filename):
    with open(f'{filename}.yaml','r') as f:
        output=yaml.safe_load(f)

    return(output)

output = read_one_block("dsc-config")

def log_message(message):
    current_time = datetime.now()
    formatted_time = current_time.strftime('%Y%m%d %H:%M:%S.%f')[:-3]
    log_entry = f"{formatted_time} {message}"
    print(log_entry)

def serve():
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=2))
    blockchain_pb2_grpc.add_blockchainServicer_to_server(blockchainServicer(),server)
    blockchain_server = output["blockchain"]["server"]
    blockchain_port = str(output["blockchain"]["port"])
    blockchain_threads = str(output["blockchain"]["threads"])
    server.add_insecure_port(blockchain_server+":"+blockchain_port)
    server.start()
    log_message("DSC v1.0")
    log_message(f"Blockchain server started at server:{blockchain_server} and port:{blockchain_port} with {blockchain_threads} threads")
    server.wait_for_termination()

if __name__ == "__main__":
    serve()