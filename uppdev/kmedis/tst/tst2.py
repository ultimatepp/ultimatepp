import os

PYTHON_EXE = 'C:\Prog\Python25\python.exe'

for i in range(20):
    os.spawnl(os.P_NOWAIT, PYTHON_EXE, PYTHON_EXE, 'tst.py')
