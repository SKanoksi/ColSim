import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import sys

col0 = ['net_ke_0']
col1 = ['net_ke_1'] #,'total_ke']
#col0 = ['net_vx_0','net_vy_0','net_vz_0']
#col1 = ['net_vx_1','net_vy_1','net_vz_1']
#col0 = ['net_px_0','net_py_0','net_pz_0']
#col1 = ['net_px_1','net_py_1','net_pz_1']

x   = 'nt'
data = pd.read_csv( str(sys.argv[1]) )

print("Total number of collision =", np.sum(data['num_collision']))

plt.figure(0)
plt.clf()

x = data[x].to_numpy()
for i in range(len(col0)):
    value = data[col0[i]].to_numpy()/data['net_num_0']
    plt.plot(x, value, label=col0[i])
for i in range(len(col1)):
    value = data[col1[i]].to_numpy()/data['net_num_1']
    plt.plot(x, value, label=col1[i])

plt.legend()
plt.tight_layout()
plt.grid(True, linestyle=':', linewidth=1)

plt.show()



