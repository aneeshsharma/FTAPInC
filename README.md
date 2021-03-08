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
$ git clone https://github.com/aneeshsharma/FTAPInC
$ cd FTAPInC
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

## The Password File

The file `passwd_list.txt` stores all users' credentials in the format of comma seperated values of username and password as given below.

```
user1,password1
user2,password2
.
.
.
user3,password3
```

> NOTE: Whitespaces are not allowed in username or password

## Input Format

The client application first asks you to enter the username and password for authentication. If the entered password is correct, you can start using the following commands.

-   `CreateFile filename` - Create an empty file with the name `filename`
-   `StoreFile filename` - Upload the file with the name `filename` (relative to the current directory) to the server
-   `GetFile filename` - Download file with the name `filename` from the server
-   `ListDir` - List all files currently on the server
-   `QUIT` - Quit the application and disconnect from the server

## File Storeage on the Server

The server stores all the files in the directory `files/` inside the current working directory of the server.

For example, a file uploaded with the name as `filename.txt` will be stored on the server at the location `files/filename.txt`

Also, if you use `GetFile filename.txt` on the client application, the file at the location `files/filename.txt` would be downloaded.

This is done to make sure that all files uploaded to the server are organised into one folder instead of cluttering the working directory.
But this is abstracted from the client and the user doesn't need to know about it.
