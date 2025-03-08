#!/usr/bin/env python3
import socket

def main():
    HOST = ''  # Listen on all available interfaces
    PORT = 39439
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server_socket:
        server_socket.bind((HOST, PORT))
        server_socket.listen()
        print(f"Listening on port {PORT}...")
        while True:
            client_socket, client_address = server_socket.accept()
            print(f"Connected by {client_address}")
            with client_socket:
                while True:
                    data = client_socket.recv(1024)
                    if not data:
                        break
                    # Decode the received bytes and process line-by-line
                    message = data.decode('utf-8').strip()
                    for line in message.splitlines():
                        parts = line.split(',')
                        if len(parts) == 2:
                            try:
                                x = float(parts[0])
                                y = float(parts[1])
                                print(f"Received coordinates: x={x}, y={y}")
                            except ValueError:
                                print(f"Invalid coordinate data: {line}")
                        else:
                            print(f"Unexpected message format: {line}")
            print("Client disconnected.")

if __name__ == '__main__':
    main()

