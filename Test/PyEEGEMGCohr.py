import numpy as np
import matplotlib.pyplot as plt

#GLOBAL VARIABLE
MATRIX=19
PHI2=6.28318530717958648
FILESUU=5
CHANNELSUU=32
EPOCH=58
INTERVAL=1024
SAMPLING_FRE=1000
EMG_CH=19
EEG_START=0
EEG_END=18
MAX_VOLT=100
EXP_TIME=60000
WANTEDDATA=60000

def channel(subjectname):
	namafile=subjectname+"-1.eeg_emg"
	data=np.fromfile(namafile,np.uint16)
	data=np.delete(data,np.arange(0,2))
	dataarray=np.array(data)
	
	data_ch=np.zeros([CHANNELSUU,WANTEDDATA])
	
	print(data_ch.shape)
	#for i in range
	
	return dataarray


	
	
#main function	

ss=channel("mutou2014317")
print(ss,len(ss))