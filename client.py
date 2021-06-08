import socket

def get_ips_for_host(host):
    try:
        ips = socket.gethostbyname_ex(host)
    except socket.gaierror:
        ips=[]
    return ips
 

UDP_IP_ADDRESS = ''.join([str(elem) for elem in get_ips_for_host('ESP-6BFDB0.mshome.net')[2]])
UDP_PORT_NO = 3000
message = "23,2,3,4"

clientSock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

clientSock.sendto(str.encode(message), (UDP_IP_ADDRESS, UDP_PORT_NO))