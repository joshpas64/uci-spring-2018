#!/usr/bin/env python
import socket, sys

## Default TCP port used to connect to receiver socket from sender socket
PORT = 8080

## Default Public IPv4 address of receiver socket
DEFAULT_IP = '35.171.97.54'

if __name__ == "__main__":
	print("Executing client program: %s" % (sys.argv[0]))
	host = DEFAULT_IP
	port = PORT

	## Allow command-line arguments to override the default IPv4 and TCP ports the sender socket will connect to.
	if len(sys.argv) > 1:
		host = sys.argv[1]
	if len(sys.argv) > 2:
		port = int(sys.argv[2])

	client_socket = None

	## Create a TCP socket
	client_socket = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
	print("Attempting to connect to a machine at IP: %s on port %d" % (host,port))

	try:
		## Connect to receiver pocket at the specified IPv4 address and TCP port.
		client_socket.connect((host,port))

		## Issue a generic HTTP GET request with TCP
		request = "GET / HTTP/1.1\r\nHost: %s\r\n\r\n" % (host)

		## Encode request as bytes and send it to the receiver socket
		client_socket.send(request.encode())

		## Receive and decode the response received from the server
		response = client_socket.recv(2048).decode()
		print("Response from server %s" % (response))

	except ConnectionRefusedError:
		## Error message displayed when the client socket cannot connect to the receiver socket
		print("I am sorry we are not able to locate the server with IP you have requested the server is unable to or has refused to connect to you.")
		print("The server may also be offline.")
	except OSError:
		## Issue in creating the socket due to errors from operating system
		print("Could not connect or make socket, use this format [IP address or hostname] [port number] [file path] to execute your program properly")
	except IndexError:
		## Issue with the formatting of the IP address or TCP port when creating the connection between sockets with the client and receiver (server) socket
		print("Could not create socket, use this format to run and create client socket.\n%s [IP] [Port number]" % (sys.argv[0]))
	except ValueError:
		## Error message when an integer port number specified
		print("The port you entered was invalid, it must be an integer.")
	except:
		## Error message for any other exception or error encountered in this program.
		print("Error in program: %s\nPlease try again" % (sys.argv[0]))

	## Close the client socket and its connection if the socket was successfully made.
	if client_socket is not None:
		client_socket.close()

	## Exit main program.
	sys.exit()