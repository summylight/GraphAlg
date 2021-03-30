from multiprocessing import Pool
import threading
import time
import os
#graph = ["gnutella"]
graph = ["amazon","dblp","catster","slashdot","enron","gnutella"]
#graph = reverse(graph)
algo = ["NMSRW4","SSRW4","WRW4","NMSRW5","SSRW5","WRW5","4RW"]

def runalg(algoname,graphname,sample,jump,repeat):
    para = "%s %s %s %s %s "%("./"+algoname,graphname,str(sample),str(jump),str(repeat))
    os.system(para)

sample = 50000
repeat_times = 1000

os.system("cp *_* ./tmp/")
os.system("rm -rf *_*")
for g in graph:
    for al in algo:
        runalg(al,g,sample,1,repeat_times)
    for k in {2,5,10}:
        runalg("NMSRW5",g,sample,k,repeat_times)
        runalg("NMSRW4",g,sample,k,repeat_times)
