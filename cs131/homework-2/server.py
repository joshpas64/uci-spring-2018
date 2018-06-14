#!/usr/bin/env python

import socket, sys

## Port used for communication, Port 8080 was being used by Apache Tomcat
PORT = 8080

PAGE_404 = "<html>" + \
                    "<head>" + \
                           "<title>404 Not Found</title>" + \
                    "</head><body>" + \
                    "<h1>404 Error: Page Not Found</h1>" + \
                    "<p>The page you requested was not found on this server.</p>" + \
                    "</body></html>\r\n"



if __name__ == "__main__":
	print("Executing program: %s" % (sys.argv[0]))
	print("Preparing TCP Socket Server for PORT %d" % (PORT))

	## Create a TCP socket
	serverSocket = socket.socket(socket.AF_INET,socket.SOCK_STREAM)

	## Create a TCP listening socket on Port 5006 that would be open to communication to any outside IP's
	serverSocket.bind(('0.0.0.0',PORT))

	##Accept up to 5 connections
	serverSocket.listen(5)

	while True:
		print("Waiting for TCP requests")
		## Obtain a connection to the client program
		connection, addr = serverSocket.accept()

		print(addr)
		print("Ready to serve...")

		try:
			## Receive up to 2048 bytes from the client connection.
			message = connection.recv(2048)
			
			print(connection.getpeername())
			print(message)
			print("Message from a client connection: %s" % (message))
			
			status = "Hello client."

			## Send an Ok status to the client program, encode the message as bytes
			connection.send("HTTP/1.1 200 OK\r\n\r\n".encode())
	
			## Send actual message to client
			connection.send(status.encode())

			##Send end of message bytes to client
			connection.send("\r\n\r\n".encode())
		except IOError:
			## Send a 404 HTTP Error to the client connection
			connection.send(b'HTTP/1.1 404 Not Found\r\n\r\n')

			##Send a 404 html page to the client
			connection.send(PAGE_404.decode())
		finally:

			##Close connection to the client program
			connection.close()

	## Close TCP server socket and exit the main program
	serverSocket.close()
	sys.exit()
