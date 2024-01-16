import time
import threading
import grpc
import blockchain_pb2
import blockchain_pb2_grpc
import pool_pb2
import pool_pb2_grpc
import yaml
import metronome_pb2
import metronome_pb2_grpc
from datetime import datetime
from concurrent import futures
import subprocess
import sys
import pickle
import blake3
import base64

fingerprint = ""
public_key = ""
result_hash = ""
threads = -1
blockID = 0
timer = datetime.now()
previous_block_ID = -1
difficulty = -1
nonce = -1
block_time=0
hashes_generated = -1
proof_of_memory_hashes = []
proof_of_memory_nonce = []
hashes_to_be_generated = 0

class hash_block:
    def __init__(self,fingerprint,public_key,nonce):
        self.fingerprint = fingerprint
        self.public_key = public_key
        self.nonce = nonce

    def serialize(self):
        # Serialize the block object
        serialized_block = pickle.dumps(self)
        return serialized_block

    def hash_block(self):
        # Serialize the block
        serialized_block = self.serialize()

        # Hash the serialized block using BLAKE3
        hasher = blake3.blake3()
        hasher.update(serialized_block)
        block_hash = hasher.digest()

        return block_hash


def compare_first_bits(str1,str2,diff):
    bin_str1 = bin(int.from_bytes(str1.encode(), 'big'))[2:]
    bin_str2 = bin(int.from_bytes(str2.encode(), 'big'))[2:]

    bin_str1 = bin_str1.zfill(max(len(bin_str1),len(bin_str2)))
    bin_str2 = bin_str2.zfill(max(len(bin_str1),len(bin_str2)))

    if bin_str1[:diff] == bin_str2[:diff] : 
        return 1
    else:
        return 0

def proof_of_Work(thread_id):
    global nonce
    global block_time
    global threads
    global hashes_generated
    hash_block_thread = hash_block(fingerprint,public_key,thread_id)
    start_time = time.time()

    while ((time.time() - start_time < block_time)):
        hash = base64.b64encode(hash_block_thread.hash_block()).decode('utf-8')[:24]

        if(compare_first_bits(hash,result_hash,difficulty)==1):
            nonce = hash_block_thread.nonce
            break
        hash_block_thread.nonce += threads
    
    if thread_id +1 == threads:
        hashes_generated = hash_block_thread.nonce
        
    
def proof_of_memory(thread_id):
    global hashes_to_be_generated
    global proof_of_memory_hashes
    hash_block_thread = hash_block(fingerprint,public_key,thread_id)
    while(hash_block_thread.nonce<hashes_to_be_generated):
        hash = base64.b64encode(hash_block_thread.hash_block()).decode('utf-8')[:24]

        proof_of_memory_hashes[hash_block_thread.nonce] =hash
        proof_of_memory_nonce[hash_block_thread.nonce] = nonce
        hash_block_thread.nonce+=threads

def proof_of_memory_search():
    global nonce
    global block_time
    global proof_of_memory_hashes
    global proof_of_memory_nonce
    low,high =0,len(proof_of_memory_hashes) - 1

    start_time = time.time()

    while low<=high:
        mid = (low+high)//2
        mid_value = proof_of_memory_hashes[mid]

        if (compare_first_bits(mid_value,result_hash,difficulty)==1):
            nonce = proof_of_memory_nonce[mid]
            break
        elif mid_value < result_hash:
            low = mid+1
        else:
            high = mid -1

        elapsed_time = time.time() - start_time
        if elapsed_time>= block_time:
            nonce = -1
            break
            

def log_message(message):
    current_time = datetime.now()
    formatted_time = current_time.strftime('%Y%m%d %H:%M:%S.%f')[:-3]
    log_entry = f"{formatted_time} {message}"
    print(log_entry)

def convert_to_bytes(size_string):
    if not size_string or len(size_string) < 2:
        return -1  # Invalid string format

    unit = size_string[-1].upper()
    value = int(size_string[:-1])

    if unit == 'G':
        bytes_value = value * (1024 ** 3)  # 1 GB = 1024^3 bytes
    elif unit == 'M':
        bytes_value = value * (1024 ** 2)  # 1 MB = 1024^2 bytes
    elif unit == 'K':
        bytes_value = value * 1024  # 1 KB = 1024 bytes
    else:
        return -1  # Invalid unit

    return bytes_value

def run(result_hash,block_time):
    if output["validator"]["proof_pom"]["enable"] == True:
        threads = output["validator"]["proof_pom"]["threaads_hash"]
        memory = output["validator"]["proof_pom"]["memory"]

        bytes_value = convert_to_bytes(memory)

        # block_time = 6000
        #only for trial, I am generating this hash, once I implement blockchain, I will import hash from there

        # result_hash = "abcdefghijkl"

        print(f"Hash Result : {result_hash}")
        print(f"Length : {len(result_hash)} bytes")

        # execute_command = f"./hash-search/blake3/Proof_of_work {fingerprint} {public_key} {threads} {block_time} {result_hash}"
        # subprocess.run(execute_command,shell=True,check=True)




def read_one_block(filename):
    with open(f'{filename}.yaml','r') as f:
        output=yaml.safe_load(f)

    return(output)

output = read_one_block("dsc-config")

def connect_to_metronome():
    global result_hash
    global blockID
    global timer
    global previous_block_ID
    global difficulty
    global fingerprint
    global public_key
    global block_time
    global threads
    global nonce
    global hashes_generated
    if output["validator2"]["proof_pow"]["enable"] == True:
        log_message("DSC v1.0")
        fingerprint = output["validator2"]["fingerprint"]
        public_key = output["validator2"]["public_key"]
        threads = output["validator2"]["proof_pow"]["threads_hash"]
        log_message(f"Proof of Work ({threads}-threads)")
        log_message(f"Fingerprint: {fingerprint}")
        metronome_server = output["metronome"]["server"]
        metronome_port = str(output["metronome"]["port"])
        channel = grpc.insecure_channel(metronome_server+":"+metronome_port)
        stub = metronome_pb2_grpc.metronomeStub(channel)
        metronome_request = metronome_pb2.validatorRequest()
        metronome_request.request = 1
        
        while True:
            metronome_response = stub.get_details(metronome_request)
            
            
            while previous_block_ID == metronome_response.blockID:
                metronome_response = stub.get_details(metronome_request)
            result_hash = metronome_response.result_hash
            block_time = 6.0 - (datetime.now() -datetime.strptime(metronome_response.timer, "%Y-%m-%d %H:%M:%S")).total_seconds()
            timer = datetime.strptime(metronome_response.timer, "%Y-%m-%d %H:%M:%S")
            difficulty = metronome_response.difficulty
            blockID = metronome_response.blockID
            nonce = -1
            log_message(f"Block {blockID}, diff {difficulty}, hash {result_hash}")
            # print(f"Hash Result : {result_hash}")
            # print(f"Length : {len(result_hash)} bytes")        
            # log_message(f"./hash-search/blake3/Proof_of_work {fingerprint} {public_key} {threads} {block_time} {result_hash} {difficulty}")
            # execute_command = f"./hash-search/blake3/Proof_of_work {fingerprint} {public_key} {threads} {block_time} {result_hash} {difficulty}"
            # result = subprocess.run(execute_command,capture_output=True,text=True,shell=True,check=True)
            # pow_output = result.stdout.strip().split(",")
            running_threads = []
            for i in range(0,threads):
                one_thread = threading.Thread(target=proof_of_Work,args=(i,))
                running_threads.append(one_thread)
                one_thread.start()

            for one_thread in running_threads:
                one_thread.join()

            
            validator_win_request = metronome_pb2.validatorWinRequest()
            validator_win_request.fingerprint = fingerprint
            validator_win_request.public_address = public_key
            validator_win_request.nonce = nonce
            validator_win_request.hashes_generated = hashes_generated
            log_message(f"Block {blockID} NONCE {nonce} ({hashes_generated})")

            validator_win_response = stub.validator_win(validator_win_request)
            previous_block_ID = blockID
            if (validator_win_response.validator_win == 1):
                pool_server = output["pool"]["server"]
                pool_port = output["pool"]["port"]
                channel_pool = grpc.insecure_channel(f"{pool_server}:{pool_port}")
                stub_pool = pool_pb2_grpc.poolStub(channel_pool)
                pool_transaction_request = pool_pb2.transferTransactionsRequest()
                pool_transaction_request.request = 1
                confirm_transaction_response = stub_pool.confirm_transactions(pool_transaction_request)
                
                transaction_list = confirm_transaction_response.transactions
                blockchain_server = output["blockchain"]["server"]
                # blockchain_port = str(output["blockchain"]["port"])
                # channel2 = grpc.insecure_channel(blockchain_server+":"+blockchain_port)
                # stub2 = blockchain_pb2_grpc.blockchainStub(channel2)
                # block_request = blockchain_pb2.blockRequest()
                block_request = metronome_pb2.blockRequestMetronome()
                
                block_request.header.version = 1
                block_request.header.previous_block_hash = result_hash
                block_request.header.blockID = blockID + 1
                block_request.header.timestamp = 1
                block_request.header.difficulty = difficulty
                block_request.header.nonce = str(nonce)
                block_request.transaction_counter = len(transaction_list)
                block_request.reserved = "reserved"
                # transaction = transaction_list
                # transaction = blockchain_pb2.transaction(sender_public_address = "sender",recepeint_pubic_address = "recepient", value = 1
                #                                             ,timestamp = 1,transaction_id = "transaction",signature = "signature")
                transactions_list_validator = []
                for transactions in transaction_list:
                    transactions_list_validator.append(metronome_pb2.transactionMetronome(sender_public_address = transactions.sender,recepeint_pubic_address =transactions.recepient,value=transactions.value,timestamp = transactions.timestamp,transaction_id = transactions.transaction_id,signature = transactions.signature ))

                block_request.transactions.extend(transactions_list_validator)
                block_request.fingerprint = fingerprint
                block_request.block_size = sys.getsizeof(block_request)
                
                # time_left = (datetime.now() -datetime.strptime(validator_win_response.timer, "%Y-%m-%d %H:%M:%S")).total_seconds()
                # time.sleep(6 - time_left)
                a = stub.block_creation_validator(block_request)
        # print(metronome_response)
                    
    elif output["validator"]["proof_pom"]["enable"] == True:
        log_message("DSC v1.0")
        fingerprint = output["validator"]["fingerprint"]
        public_key = output["validator"]["public_key"]
        threads = output["validator"]["proof_pom"]["threads_hash"]
        log_message(f"Proof of Memory ({threads}-threads)")
        log_message(f"Fingerprint: {fingerprint}")
        global proof_of_memory_hashes
        global proof_of_memory_nonce
        size = convert_to_bytes(output["validator"]["proof_pom"]["memory"])
        hashes_to_be_generated = size // 48
        proof_of_memory_hashes = [""] * (hashes_to_be_generated+100)
        proof_of_memory_nonce = [-1] * (hashes_to_be_generated+100)
        running_threads = []
        for i in range(0,threads):
            one_thread = threading.Thread(target=proof_of_memory,args=(i,))
            running_threads.append(one_thread)
            one_thread.start()

        for one_thread in running_threads:
            one_thread.join()        

        combined_values = list(zip(proof_of_memory_hashes,proof_of_memory_nonce))

        sorted_combined_values = sorted(combined_values,key = lambda x: x[0])

        proof_of_memory_hashes,proof_of_memory_nonce = zip(*sorted_combined_values)
        log_message("hashes generated and stored in memory")
        metronome_server = output["metronome"]["server"]
        metronome_port = str(output["metronome"]["port"])
        channel = grpc.insecure_channel(metronome_server+":"+metronome_port)
        stub = metronome_pb2_grpc.metronomeStub(channel)
        metronome_request = metronome_pb2.validatorRequest()
        metronome_request.request = 1
        
        while True:
            metronome_response = stub.get_details(metronome_request)

            while previous_block_ID == metronome_response.blockID:
                metronome_response = stub.get_details(metronome_request)
            result_hash = metronome_response.result_hash
            block_time = 6.0 - (datetime.now() -datetime.strptime(metronome_response.timer, "%Y-%m-%d %H:%M:%S")).total_seconds()
            timer = datetime.strptime(metronome_response.timer, "%Y-%m-%d %H:%M:%S")
            difficulty = metronome_response.difficulty
            blockID = metronome_response.blockID
            nonce = -1
            log_message(f"Block {blockID}, diff {difficulty}, hash {result_hash}")
            proof_of_memory_search()
            validator_win_request = metronome_pb2.validatorWinRequest()
            validator_win_request.fingerprint = fingerprint
            validator_win_request.public_address = public_key
            validator_win_request.nonce = nonce
            validator_win_request.hashes_generated = hashes_to_be_generated
            log_message(f"Block {blockID} NONCE {nonce} ({hashes_generated})")

            validator_win_response = stub.validator_win(validator_win_request)
            previous_block_ID = blockID
            if (validator_win_response.validator_win == 1):
                pool_server = output["pool"]["server"]
                pool_port = output["pool"]["port"]
                channel_pool = grpc.insecure_channel(f"{pool_server}:{pool_port}")
                stub_pool = pool_pb2_grpc.poolStub(channel_pool)
                pool_transaction_request = pool_pb2.transferTransactionsRequest()
                pool_transaction_request.request = 1
                confirm_transaction_response = stub_pool.confirm_transactions(pool_transaction_request)
                
                transaction_list = confirm_transaction_response.transactions
                blockchain_server = output["blockchain"]["server"]
                # blockchain_port = str(output["blockchain"]["port"])
                # channel2 = grpc.insecure_channel(blockchain_server+":"+blockchain_port)
                # stub2 = blockchain_pb2_grpc.blockchainStub(channel2)
                # block_request = blockchain_pb2.blockRequest()
                block_request = metronome_pb2.blockRequestMetronome()
                
                block_request.header.version = 1
                block_request.header.previous_block_hash = result_hash
                block_request.header.blockID = blockID + 1
                block_request.header.timestamp = 1
                block_request.header.difficulty = difficulty
                block_request.header.nonce = str(nonce)
                block_request.transaction_counter = len(transaction_list)
                block_request.reserved = "reserved"
                # transaction = transaction_list
                # transaction = blockchain_pb2.transaction(sender_public_address = "sender",recepeint_pubic_address = "recepient", value = 1
                #                                             ,timestamp = 1,transaction_id = "transaction",signature = "signature")
                transactions_list_validator = []
                for transactions in transaction_list:
                    transactions_list_validator.append(metronome_pb2.transactionMetronome(sender_public_address = transactions.sender,recepeint_pubic_address =transactions.recepient,value=transactions.value,timestamp = transactions.timestamp,transaction_id = transactions.transaction_id,signature = transactions.signature ))

                block_request.transactions.extend(transactions_list_validator)
                block_request.fingerprint = fingerprint
                block_request.block_size = sys.getsizeof(block_request)
                
                # time_left = (datetime.now() -datetime.strptime(validator_win_response.timer, "%Y-%m-%d %H:%M:%S")).total_seconds()
                # time.sleep(6 - time_left)
                a = stub.block_creation_validator(block_request)
            





if __name__ == "__main__":
    connect_to_metronome()
