import traceback
from termcolor import colored
import codecs
import struct

def getDataLog(self, pyld, espNum):
    # print(colored(espNum + "-", 'yellow'), end="")
    # for py in pyld:
    #     print(colored(py, 'yellow'), end=" ")    
    # print()
    pld_str = pyld[1]
    num_hex = int(len(pyld[1]))
    values = [None]*int((num_hex)/2)
    for i in range(int(num_hex/2)):
        values[i] = int(pld_str[i*2:i*2+2], 16)

    # print(pld_str)
    # print(values)
    time = 0
    spd_floats = [0]*3
    edges = [0]*3
    orient = [0]*3
    velocity = [0]*3
    angle_floats = [0]*3
    i = 5;
    try:
        time = (values[1] << 24) | (values[2] << 16) | (values[3] << 8) | values[4];
        if values[0] & 0b00000001:    #angles
            for j in range(0,3):
        if values[0] & 0b00000010:    #edges
            for j in range(0,3):
                edges[j] = values[i]
                i += 1
        if values[0] & 0b00000100:    #orient
            for j in range(0,3):
                orient[j] = values[i]*256 + values[i+1]
                if orient[j] >= 32768:
                    orient[j] -= 65536 

                # orient[j] = values[i]*256 + values[i+1]
                i += 2                
        if values[0] & 0b00001000:    #velocity
            for j in range(0,3):
                tmp = pld_str[i*2:i*2+8]
                # if(espNum=="000AC171" and j==0): # Uncomment to debug
                #     print(round(struct.unpack('<f', bytes.fromhex(tmp))[0], 3))

                velocity[j] = struct.unpack('<f', bytes.fromhex(tmp))[0]
                i += 4                 
        if values[0] & 0b00010000:    #angles, but as floaty boiiiissss
            for j in range(0,3):
                tmp = pld_str[i*2:i*2+8]

                angle_floats[j] = struct.unpack('>f', bytes.fromhex(tmp))[0]
                i += 4    

        cscString = [espNum, str(time), str(spd_floats).strip('[]'), 
                str(edges).strip('[]'), str(orient).strip('[]'),
                str(velocity).strip('[]'), str(angle_floats).strip('[]') + '\n']
        self.writeDataLogFile(','.join(cscString))
    except:
        print(colored("IN TRACEBACK", 'red'))
        traceback.print_exc()
    
    return