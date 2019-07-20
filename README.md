# file_transfer_program

Sending and receiving files using tcp sockets.

## about

Tested on debian linux.

### client.cpp

Make sure the server is running.
```
./client <file to send>
```

### server.cpp

```
./server
```
when *./client* is ran, server will download the file specified.

## example run
client
```
$ ./client large.pdf
Sent file: large.pdf
# of bytes sent: 6213911
$ 
```
server
```
$./server
Listening on port: 5555
Server listening
Received Filename: large.pdf
# of bytes for file: 6213911
Writing to large.pdf
Packet contains 807684 bytes
Packet contains 523864 bytes
...
Packet contains 458381 bytes
Packet contains 1215315 bytes
Done. # of bytes received: 6213911
```

