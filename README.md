# FTAP - File Transfer Application Program

A basic file server implementation in C.

## Features

-   A simple user authentication feature
-   Create a File on server
-   Upload file to server
-   Download file from server
-   List all files on server

## Preqrequisite

To use this program make sure you have the following packages -

-   `gcc` - The GNU Compiler Collection
-   `gnuplot` - A simple graph plotting utility
-   `make` - The make utility

## Usage

First clone this repository and change to the directory.

```
$ git clone https://github.com/aneeshsharma/FileTransferInC
$ cd FileTransferInC
```

To use the program compile the files `client.c` and `server.c` using the included `makefile` or `gcc`.

```
$ make
```

Then execute the server (first) and client (second) applications.

```
$ ./server
```

```
$ ./client
```

## Input Format

You can give input in the client application. For any word input the server replies with the message "message".

-   Quit the application by inputting `Bye`
-   Initiate large file transfer using `GivemeyourVideo`
