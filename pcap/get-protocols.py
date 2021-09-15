#!/usr/local/bin/python3

from sys import argv

def get_protocols_hash():
    with open("/etc/protocols","r") as f:
        temp = f.read()
        protocols = [protocol for protocol in temp.split("\n")]
    protocols_hash = {}
    for i in range(11,len(protocols)-1):
        key = protocols[i].split("\t")[1]
        value = protocols[i].split("\t")[2]
        protocols_hash[key] = value
    return protocols_hash

def get_protocol_by_id(key):
	protocols = get_protocols_hash()
	return protocols[key]

def main():
	print(get_protocol_by_id(argv[1]))
	
if __name__ == "__main__": main()

