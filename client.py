import socket
from time import sleep
from spectrum import SpectrumAnalyzer
from colourGenerator import getColour

#create spectrum analyzer
spec = SpectrumAnalyzer()

def get_ips_for_host(host):
    try:
        ips = socket.gethostbyname_ex(host)
    except socket.gaierror:
        ips=[]
    return ips
 

UDP_IP_ADDRESS = ''.join([str(elem) for elem in get_ips_for_host('ESP-6BFDB0.mshome.net')[2]])
UDP_PORT_NO = 3000

clientSock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

try:
    while True:
        message = ""
        spec.data = spec.audioinput()
        spec.fft()
        spec.graphplot()
        for x in range(150):
            message = str(x)
            if x >= 129:
                message =  message + getColour(max(spec.spec_y[193:255]), 6)
            elif x >= 108:
                message =  message + getColour(max(spec.spec_y[129:192]), 5)
            elif  x >= 87:
                message =  message + getColour(max(spec.spec_y[65:128]), 4)
            elif x >= 65:
                message =  message + getColour(max(spec.spec_y[17:64]), 3)
            elif x >= 43:
                message =  message + getColour(max(spec.spec_y[9:16]), 2)
            elif x >= 21:
                message =  message + getColour(max(spec.spec_y[3:8]), 1)
            else:
                message =  message + getColour(max(spec.spec_y[0:2]), 0)
            message = message.rstrip(',')
            print(len(message))
            clientSock.sendto(str.encode(message), (UDP_IP_ADDRESS, UDP_PORT_NO))
            sleep(0.005)
except KeyboardInterrupt:
    spec.pa.close()
    print("End...")