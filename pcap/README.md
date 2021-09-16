# Simple packet capture POC

Simple packet capture program. Configure the interface and BPF, along with packet count number in config.json and start the capture. 

Requirements:

* libjansson
* libpcap

TODO:
---
* Tune the program even further
* Have the program spawn a different process for the capture, running in background
* Have the program send the capture to a client over the network or to a DB (mongo)


