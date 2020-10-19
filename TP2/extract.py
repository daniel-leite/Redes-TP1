import numpy as np
from matplotlib import pyplot as plt
import sys


def getRTT (i, file):
    with open(file, 'r') as f:
        file = f.read()
    sites = file.split('\n\n\n\n\n\n\n')
    tries = sites[i].split('\n\n')
    rtt = []
    for try_ in tries:
        temp = try_.split('\n')
        n = len(temp)
        temp = temp[n-1].split('  ')
        for i, bit in enumerate(temp):
            bit = bit.replace('*','')
            bit = bit.replace(' ', '')
            bit = bit.replace('ms', '')
            temp[i] = bit
        rtt += temp[2:]
    return rtt

def getRoute(i, file):
    with open(file, 'r') as f:
        file = f.read()
    sites = file.split('\n\n\n\n\n\n\n')
    tries = sites[i].split('\n\n\n')
    route = []

    for i, try_ in enumerate(tries):
        temp = try_.split('\n')
        for ii, line in enumerate(temp):
            hop_addr = line.split('  ')
            for iii, bit in reversed(list(enumerate(hop_addr))):
                bit = bit.replace('*','')
                bit = bit.replace(' ', '')
                hop_addr[iii] = bit
                if bit.find('ms') != -1 or bit.isnumeric():
                    hop_addr.pop(iii)

            if i == 0:
                route.append(hop_addr)
            else:
                try:
                    for addr in hop_addr:
                        if addr not in route[ii]:
                            route[ii].append(addr)
                except  IndexError:
                    route.append(hop_addr)
    for hop in route:
        hop.sort()
    return route[1:]

def main (file, names):
    n = len(file)
    fig, ax = plt.subplots(2, 2, figsize=(30,2*7.5))
    for i in range (4):
        rtt_str = getRTT(i, file)
        rtt = np.array([float(x) for x in rtt_str])
    #     print (rtt.mean(), rtt)

        ax[int(i/2)][i%2].hist(rtt, density=False)
        ax[int(i/2)][i%2].set_xlabel('rtt[ms]')
        ax[int(i/2)][i%2].set_title('RTT para ' +sites[i])

        route = getRoute(i, file)
        with open(file[:n-4] + '_route2_' + names[i]+'-.txt', 'w') as f:
            for line in route:
                for ii, addr in enumerate(line):
                    if ii>0:
                        f.write(', ')
                    if addr == '':
                        f.write('*')
                    f.write(addr)
                f.write('\n')
            f.write('\n\n\n')
            for line in rtt_str:
                f.write(line + ', ')
    fig.savefig(file[:n-4] + '_rtt.png', dpi=200, bbox_inches = 'tight' )


sites =['www.ufrj.br', 'www.ucla.edu', 'www.phil.cam.ac.uk', 'www.adelaide.edu.au']

parameters = {'xtick.labelsize': 15,
              'ytick.labelsize': 15,
              'axes.labelsize' : 20,
              'axes.titlesize' : 25}
plt.rcParams.update(parameters)

file = sys.argv[1]
main(file, sites)
