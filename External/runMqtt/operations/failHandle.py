from termcolor import colored

from Settings import names

def failHandle(self, pyld, espNum):
    msgld = ' '.join(pyld)
    print(colored(names.idsToName[espNum] + "-" + msgld, 'red'))
    self.publishGlobal("stp", qos=0)
    self.publishGlobal("stp", qos=1)
    return