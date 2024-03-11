from numpy import sin,pi
import numpy as np
import matplotlib.pyplot as plt
import pandas as pd

t = np.linspace(0,0.045,100000)

Vdc = 800
L = 53e-6
Imax = 16.5
M = 0.81
w = 2*pi*50

A = sin(w*t)

# set the minimum frequency
fmin = 20e3

# calculate the switching frequency for the phase A
fsw = Vdc/(4*L*Imax)*(1-abs(Sa))*abs(Sa)/(0.9*abs(A)+0.1)
# and then limit it
fsw = [fmin if f < fmin else f for f in fsw]

# calculate the sampling time for each frequency in the range
sampling = [1./f for f in fsw]
# create the Data Frame
df = pd.DataFrame({'t':t,'freq':fsw,'samp':sampling})

# reset initial values of the PWM sample hit and initial carrier
df.loc[0,'new time'] = 0
df.loc[0,'carrier'] = -1

# generate alternative carrier points at each sampling
for i in range(df.shape[0]):
    df.loc[i+1,'new time'] = df.loc[i,'samp']*0.5+df.loc[i,'new time']
    df.loc[i+1,'carrier'] = df.loc[i,'carrier']*-1
    
# offset the carrier and split into the upper and lower 
df['top'] = df['carrier']*0.5+0.5
df['bot'] = df['carrier']*0.5-0.5

# save both carriers in separate txt files for LTSpice/QSpice
df.loc[:,['new time','top']].to_csv(f"var_freq_top.txt",index=False,header=False)
df.loc[:,['new time','bot']].to_csv(f"var_freq_bot.txt",index=False,header=False)

plt.plot(t,fsw)
plt.ylim(0,150e3)
