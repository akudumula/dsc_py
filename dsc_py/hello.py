# import uuid
# print("Fingerprint : ",uuid.uuid4())

import yaml
import subprocess
import random

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

def read_one_block(filename):
    with open(f'{filename}.yaml','r') as f:
        output=yaml.safe_load(f)

    print(output)
    return(output)

output = read_one_block("dsc-config")


# compile_command = "gcc -o hash-search/blake3/Proof_of_work hash-search/blake3/Proof_of_work.c"
# subprocess.run(compile_command,shell=True,check=True)
fingerprint = output["validator"]["fingerprint"]
public_key = output["validator"]["public_key"]
if output["validator"]["proof_pow"]["enable"] == True:

    threads = output["validator"]["proof_pow"]["threaads_hash"]
    block_time = 6000
    #only for trial, I am generating this hash, once I implement blockchain, I will import hash from there

    result_hash = "abcdefghijkl"

    print(f"Hash Result : {result_hash}")
    print(f"Length : {len(result_hash)} bytes")


    execute_command = f"./hash-search/blake3/Proof_of_work {fingerprint} {public_key} {threads} {block_time} {result_hash}"
    result = subprocess.run(execute_command,capture_output=True,text=True,shell=True,check=True)
    print(result.stdout.strip())

elif output["validator"]["proof_pom"]["enable"] == True:
    threads = output["validator"]["proof_pom"]["threaads_hash"]
    memory = output["validator"]["proof_pom"]["memory"]

    bytes_value = convert_to_bytes(memory)

    block_time = 6000
    #only for trial, I am generating this hash, once I implement blockchain, I will import hash from there

    result_hash = "abcdefghijkl"

    print(f"Hash Result : {result_hash}")
    print(f"Length : {len(result_hash)} bytes")

    execute_command = f"./hash-search/blake3/Proof_of_work {fingerprint} {public_key} {threads} {block_time} {result_hash}"
    subprocess.run(execute_command,shell=True,check=True)


