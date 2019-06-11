# %%
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

# %%
x = np.arange(-2, 2, 0.01).reshape([-1, 1])
x = np.repeat(x, x.shape[0], axis=1)
y = x.T
x.shape, y.shape
# %%
plt.figure(figsize=(6,6))
plt.contourf(abs(x)+abs(y))
#%%
plt.figure(figsize=(6,6))

plt.contourf((x**2+y**2)**0.25)
#%%
plt.figure(figsize=(6,6))

plt.contourf(abs(x)>abs(y))


#%%

labels = ['Const 0','L0','L1','L2','Linf']
mem = [96402,9463,666,2160,2135]
t = [1000368435,5112500,26309,283524,232786]
df = pd.DataFrame(index=labels,data={
    'Memory':mem,'Time':t
})

#%%
df.plot(logy=True,style="-o")
plt.show()
#%%
