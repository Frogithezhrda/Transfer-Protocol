# File Transfer Protocol

This program implements a simple file transfer system over TCP. It was tested successfully with a 10 GB file, which completed in about 17 minutes.

## Usage

Make sure both computers can connect to each other over a LAN.

If you are not on the same physical network, use a LAN overlay such as ZeroTier or Hamachi.

Run the receiver first. It will:

Accept the incoming connection.

Receive data in 16,000 byte chunks.

Write the chunks into the output file.

## Run the sender:

Provide the file you want to send.

It will split the file into 16,000 byte chunks and transmit them sequentially.

When the transfer completes, the receiver saves the full reconstructed file.

## Notes

The protocol is confirmed working and capable of handling very large files.

Transfer speed depends on your network connection.

Occasionally, connection issues may happen.
