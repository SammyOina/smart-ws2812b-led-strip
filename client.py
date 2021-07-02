import socket
from spectrum import SpectrumAnalyzer
from colourGenerator import getColour
from time import sleep

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
        #spec.graphplot()
        message =  message + getColour(max(spec.spec_y[0:2]), 0)
        message =  message + getColour(max(spec.spec_y[3:8]), 1)
        message =  message + getColour(max(spec.spec_y[9:16]), 2)
        message =  message + getColour(max(spec.spec_y[17:64]), 3)
        message =  message + getColour(max(spec.spec_y[65:128]), 4)
        message =  message + getColour(max(spec.spec_y[129:192]), 5)
        message =  message + getColour(max(spec.spec_y[193:255]), 6)

        message = message.rstrip(',')

        clientSock.sendto(str.encode(message), (UDP_IP_ADDRESS, UDP_PORT_NO))
        #sleep(0.8)
except KeyboardInterrupt:
    spec.pa.close()
    print("End...")