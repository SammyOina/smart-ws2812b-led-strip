import socket
from spectrum import SpectrumAnalyzer

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
message = "23,2,3,4"

clientSock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

try:
    while True:
        spec.data = spec.audioinput()
        spec.fft()
        spec.graphplot()
        print("sub bass: ", max(spec.spec_y[0:2]))
        print("bass: ", max(spec.spec_y[3:8]))
        print("low mid: ", max(spec.spec_y[9:16]))
        print("mid: ", max(spec.spec_y[17:64]))
        print("upper mid: ", max(spec.spec_y[65:128]))
        print("presence: ", max(spec.spec_y[129:192]))
        print("brilliance:", max(spec.spec_y[193:255]))

        clientSock.sendto(str.encode(message), (UDP_IP_ADDRESS, UDP_PORT_NO))
except KeyboardInterrupt:
    spec.pa.close()
    print("End...")