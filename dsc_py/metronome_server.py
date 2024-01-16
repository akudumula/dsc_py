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
import sys

timer = datetime.now()
previous_hash_value =""
previous_blockID = -1
difficulty = 30
validator_win = 0
previous_blockIDs = []
class metronomeServicer(metronome_pb2_grpc.metronomeServicer):
    def get_details(self, request, context):
        # print("Request recieved")
        # print(request)
        validator_response =metronome_pb2.validatorResponse()
        validator_response.result_hash = previous_hash_value
        validator_response.block_time =6
        validator_response.timer = timer.strftime("%Y-%m-%d %H:%M:%S")
        validator_response.difficulty = difficulty
        validator_response.blockID = previous_blockID
        # print(validator_response)
        return validator_response
    
    def validator_win(self, request, context):
        global validator_win
        global difficulty
        # print("Request recieved")
        # print(request)
        validator_win_response = metronome_pb2.validatorWinResponse()
        if request.nonce != -1 and validator_win==0:
            validator_win = 1
            validator_win_response.validator_win = 1
            validator_win_response.timer = timer.strftime("%Y-%m-%d %H:%M:%S")
            
            log_message(f"Validator {request.fingerprint} claims to win block with difficulty {difficulty} hash {previous_hash_value} with nonce {request.nonce}")
            difficulty+=1
        else:
            validator_win_response.validator_win = 0
            difficulty-=1
        
        return validator_win_response
    
    def block_creation_validator(self, request, context):
        global validator_win
        blockchain_server = output["blockchain"]["server"]
        blockchain_port = str(output["blockchain"]["port"])
        channel = grpc.insecure_channel(blockchain_server+":"+blockchain_port)
        stub = blockchain_pb2_grpc.blockchainStub(channel)
        
        block_request = blockchain_pb2.blockRequest()
                    
        block_request.header.version = request.header.version
        block_request.header.previous_block_hash = request.header.previous_block_hash
        if request.header.blockID in previous_blockIDs:

            block_request.header.blockID = request.header.blockID + 1
        else:
            block_request.header.blockID = request.header.blockID 
        block_request.header.timestamp = request.header.timestamp
        block_request.header.difficulty = request.header.difficulty
        block_request.header.nonce = request.header.nonce
        block_request.transaction_counter = request.transaction_counter
        block_request.reserved = request.reserved
        # transaction = transaction_list
        # transaction = blockchain_pb2.transaction(sender_public_address = "sender",recepeint_pubic_address = "recepient", value = 1
        #                                             ,timestamp = 1,transaction_id = "transaction",signature = "signature")
        transaction_list = request.transactions
        transactions_list_validator = []
        for transactions in transaction_list:
            transactions_list_validator.append(blockchain_pb2.transaction(sender_public_address = transactions.sender_public_address,recepeint_pubic_address =transactions.recepeint_pubic_address,value=transactions.value,timestamp = transactions.timestamp,transaction_id = transactions.transaction_id,signature = transactions.signature ))

        block_request.transactions.extend(transactions_list_validator)
        block_request.fingerprint = request.fingerprint
        block_request.block_size = request.block_size

        # block_request = request
        time_left = (datetime.now() - timer).total_seconds()
        if 6-time_left>0:
            time.sleep(6 - time_left)
        stub.block_creation(block_request)

        return metronome_pb2.responseMetronome(response = "Success")



        


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




def handle_timer_expiry():
    print("Timer Expired")

def request_block_chain():
    global previous_hash_value
    global previous_blockID
    global validator_win
    validator_win = 0
    blockchain_server = output["blockchain"]["server"]
    blockchain_port = str(output["blockchain"]["port"])
    channel = grpc.insecure_channel(blockchain_server+":"+blockchain_port)
    stub = blockchain_pb2_grpc.blockchainStub(channel)
    block_hashRequest = blockchain_pb2.new_block_request()
    block_hashRequest.request = 1
    block_hashreply = stub.metronome_blockHash(block_hashRequest)
    while block_hashreply.blockID in previous_blockIDs:
        block_hashreply = stub.metronome_blockHash(block_hashRequest)
    previous_blockID = block_hashreply.blockID
    previous_blockIDs.append(previous_blockID)
    previous_hash_value = block_hashreply.block_hash
    
    # print(block_hashreply)

def send_request_to_block():
    if validator_win == 0:
        blockchain_server = output["blockchain"]["server"]
        blockchain_port = str(output["blockchain"]["port"])
        channel = grpc.insecure_channel(blockchain_server+":"+blockchain_port)
        stub = blockchain_pb2_grpc.blockchainStub(channel)
        
        block_request = blockchain_pb2.blockRequest()

        
        block_request.header.version = 1
        block_request.header.previous_block_hash = previous_hash_value
        block_request.header.blockID = previous_blockID + 1
        block_request.header.timestamp = 1
        block_request.header.difficulty = 1
        block_request.header.nonce = "nonce"
        block_request.transaction_counter = 1
        block_request.reserved = "reserved"
        transaction = blockchain_pb2.transaction(sender_public_address = "sender",recepeint_pubic_address = "recepient", value = 1
                                                    ,timestamp = 1,transaction_id = "transaction",signature = "signature")

        block_request.transactions.extend([transaction])
        block_request.fingerprint = "metronome"
        block_request.block_size = sys.getsizeof(block_request)
        log_message(f"New block created hash {previous_hash_value} sent to blockchain")
        stub.block_creation(block_request)
    

def serve():
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=2))
    metronome_pb2_grpc.add_metronomeServicer_to_server(metronomeServicer(),server)
    metronome_server = output["metronome"]["server"]
    metronome_port = str(output["metronome"]["port"])
    metronome_threads = str(output["metronome"]["threads"])
    server.add_insecure_port(metronome_server+":"+metronome_port)
    server.start()
    log_message("DSC v1.0")
    log_message(f"Metronome server started at server:{metronome_server} and port:{metronome_port} with {metronome_threads} threads")
    client()
    server.wait_for_termination()    

def client():
    global timer
    timer = datetime.now()
    
    request_block_chain()
    while True:
        time_taken = (datetime.now() - timer).total_seconds()
        
        if (6 - time_taken < 0):        
            send_request_to_block()
            timer = datetime.now()
            request_block_chain()

if __name__ == "__main__":
    # client_thread = threading.Thread(target=client())
    # # server_thread = threading.Thread(target=serve())
    

    # client_thread.start()
    # # server_thread.start()
    serve()

    # # server_thread.join()
    # client_thread.join()
    
    

    
    