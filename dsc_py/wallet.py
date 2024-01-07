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

def generate_key_pair():
    private_key = ec.generate_private_key(ec.SECP256R1, default_backend())
    public_key = private_key.public_key()

    private_key_bytes = private_key.private_bytes(encoding=serialization.Encoding.PEM, format=serialization.PrivateFormat.TraditionalOpenSSL,encryption_algorithm=serialization.NoEncryption())

    public_key_bytes = public_key.public_bytes(encoding=serialization.Encoding.PEM, format=serialization.PublicFormat.SubjectPublicKeyInfo)

    return private_key_bytes,public_key_bytes

def sha256(data):
    return hashlib.sha256(data).digest()

def base58_encode(data):
    return base58.b58encode(data).decode('utf-8')

def create():
    with open("dsc-config.yaml") as file:
        existing_data = yaml.safe_load(file)
    if(existing_data["wallet"] == None):
        private_key, public_key = generate_key_pair()
        private_key_sha256 = sha256(private_key)
        public_key_sha256 = sha256(public_key)

        encoded_private_key = base58_encode(private_key_sha256)
        encoded_public_key = base58_encode(public_key_sha256)

        key_data = {'private_key': encoded_private_key}
        with open("dsc-key.yaml","w") as file:
            yaml.dump(key_data,file)

        os.chmod("dsc-key.yaml",0o400)

        with open("dsc-config.yaml",'r') as file:
            existing_data = yaml.safe_load(file)

        existing_data["wallet"]={"public_key": encoded_public_key}

        with open("dsc-config.yaml",'w') as file:
            yaml.dump(existing_data,file,default_flow_style=False)

        print(f"Private Address: {encoded_private_key}")
        print(f"Public Address: {encoded_public_key}")
    else:
        print("Wallet already exists at dsc-config.yaml. wallet create aborted")

def key():
    with open("dsc-config.yaml") as file:
        existing_data = yaml.safe_load(file)
    if(existing_data["wallet"] == None):
        print('''Error in finding key information. ensure that dsc-config.yaml and dsc-key.yaml exist and that they contain the correct nformation. You may need to run "./dsc wallet create" .''')
    else:
        print("Wallet public Address: "+existing_data["wallet"]["public_key"])
        with open("dsc-key.yaml") as file:
            key = yaml.safe_load(file)
        print("Wallet private Address: "+key["private_key"])

def read_one_block(filename):
    with open(f'{filename}.yaml','r') as f:
        output=yaml.safe_load(f)

    return(output)

output = read_one_block("dsc-config")

def run(): 
    pool_server = output["pool"]["server"]
    pool_port = str(output["pool"]["port"])
    channel = grpc.insecure_channel(pool_server+":"+pool_port)
    stub = pool_pb2_grpc.poolStub(channel)
    blockchain_server = output["blockchain"]["server"]
    blockchain_port = str(output["blockchain"]["port"])
    channel2 = grpc.insecure_channel(blockchain_server+":"+blockchain_port)
    stub2 = blockchain_pb2_grpc.blockchainStub(channel2)
    print("1. send")
    print("2. transaction")
    print("3. transactions")
    print("4. balance")
    rpc_call = int(input("Enter your choice: "))
    #rpc_call=1
    if(rpc_call == 1):
        print("send operation")
        send_request = pool_pb2.sendRequest(sender_public_address = "Sender",recepeint_pubic_address="Recepeint",value = 100,timestamp = 10,transaction_id = "101", signature = "ABCD")
        send_replies = stub.send(send_request)
        for send_reply in send_replies:

            print("Response recieved: ")
            print(send_reply)

    elif(rpc_call == 2):
        print("Transaction operation")
        transaction_request = pool_pb2.transactionRequest(transaction_id = "101")
        transaction_reply = stub.transaction(transaction_request)
        print("Response recieved")
        print(transaction_reply)

    elif(rpc_call == 3):
        print("Transactions operation")
        transactions_request = pool_pb2.transactionsRequest(wallet_public_key = "Sender")
        transaction_reply = stub.transactions(transactions_request)
        print("Response recieved")
        print(transaction_reply)
    elif (rpc_call ==4):
        print("Balance request:")
        balance_request = blockchain_pb2.balanceRequest(wallet_public_key = "Sender")
        balance_reply = stub2.balance(balance_request)
        print("Response recieved")
        print(balance_reply)

if __name__ == "__main__":
    run()