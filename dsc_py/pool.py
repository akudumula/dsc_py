from concurrent import futures
import time
import yaml
import grpc
import pool_pb2
import pool_pb2_grpc
from datetime import datetime

class transaction:
    def __init__(self,sender,recepient,value,timestamp,transaction_id,signature):
        self.sender = sender
        self.recepient = recepient
        self.value = value
        self.timestamp = timestamp
        self.transaction_id = transaction_id
        self.signature = signature

class poolServicer (pool_pb2_grpc.poolServicer):
    
    def send(self, request, context):
        print("Send request made:")
        print(request)
        print(type(request))
        send_reply = pool_pb2.sendReply()
        
        send_transaction = transaction(request.sender_public_address,request.recepeint_pubic_address,request.value,request.timestamp,request.transaction_id,request.signature)
        submit_transaction(send_transaction)
        while(send_reply.reply != "Confirmed"):
            submit=0
            for t in submitted:
                if t.transaction_id == send_transaction.transaction_id:
                    submit = 1
            if(submit == 1):
                send_reply.reply = f"Submitted"
            elif request.transaction_id in unconfirmed:
                send_reply.reply = "Unconfirmed"
            else:
                send_reply.reply = "Confirmed"
            yield send_reply
            time.sleep(1)

    def transaction(self, request, context):

        transaction_reply = pool_pb2.transactionReply()
        for t in submitted:
            if t.transaction_id == request.transaction_id:
                submit = 1
        if(submit == 1):
            transaction_reply.status = f"Submitted"
        elif request.transaction_id in unconfirmed:
            transaction_reply.status = "Unconfirmed"
        else:
            transaction_reply.status = "cannot verify ask blockchain"

        return transaction_reply
    
    def transactions(self, request, context):
        print("Transaction Request recieved:")
        print(request)

        transaction_reply = pool_pb2.transactionReply()
        transaction_reply.status = f"Status unchanged for wallet {request.wallet_public_key}"

        return transaction_reply
    
    def confirm_transactions(self, request, context):
        transactions_list = []
        for i in range (0,200):
            latest_transaction = move_to_unconfirmed()
            if latest_transaction != None:
                transactions_list.append(pool_pb2.sendRequest(sender_public_address = latest_transaction.sender,recepeint_pubic_address =latest_transaction.recepient,value=latest_transaction.value,timestamp = latest_transaction.timestamp,transaction_id = latest_transaction.transaction_id,signature = latest_transaction.signature ))
            break
        response = pool_pb2.transferTransaction(transactions = transactions_list)
        return response
    
    def move_to_confirm(self, request, context):
        for r in request.transaction_ids:
            confirm_transaction(r)

        return pool_pb2.validator_confirm_response(response = "Good")

submitted = []
unconfirmed = {}

def read_one_block(filename):
    with open(f'{filename}.yaml','r') as f:
        output=yaml.safe_load(f)

    return(output)

output = read_one_block("dsc-config")

def submit_transaction(transaction):
    submitted.append(transaction)

def move_to_unconfirmed():
    if submitted:
        latest_transaction = submitted.pop(0)
        unconfirmed[latest_transaction.transaction_id] = latest_transaction
        return latest_transaction

def confirm_transaction(transaction_id):
    if transaction_id in unconfirmed:
        confirmed_transaction = unconfirmed.pop(transaction_id)
        return confirmed_transaction



def log_message(message):
    current_time = datetime.now()
    formatted_time = current_time.strftime('%Y%m%d %H:%M:%S.%f')[:-3]
    log_entry = f"{formatted_time} {message}"
    print(log_entry)

def serve():
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=10))
    pool_pb2_grpc.add_poolServicer_to_server(poolServicer(),server)
    pool_server = output["pool"]["server"]
    pool_port = str(output["pool"]["port"])
    pool_threads = str(output["pool"]["threads"])
    server.add_insecure_port(pool_server+":"+pool_port)
    server.start()
    log_message("DSC v1.0")
    log_message(f"Pool server started at server:{pool_server} and port:{pool_port} with {pool_threads} threads.")
    server.wait_for_termination()

if __name__ == "__main__":
    serve()