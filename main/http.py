import socket

HOST = '192.168.50.207'  # 服务器的主机地址
PORT = 8080         # 服务器的端口号
BUFFER_SIZE = 1024

def main():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server_socket:
        server_socket.bind((HOST, PORT))
        server_socket.listen(1)
        print(f"Server listening on {HOST}:{PORT}")

        while True:
            conn, addr = server_socket.accept()
            with conn:
                print(f"Connected by {addr}")

                request = conn.recv(BUFFER_SIZE).decode("utf-8")
                print("Request received:")
                print(request)

                # 构造 HTTP 响应
                response = "holystone\n\r"
                conn.sendall(response.encode("utf-8")[:len(response)])
                print("Response sent")

if __name__ == "__main__":
    main()
