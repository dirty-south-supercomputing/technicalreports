#!/usr/bin/python3

import seaborn as sns
import pandas as pd
from pandas.io.json import json_normalize
import matplotlib.pyplot as plt

sns.set(style="darkgrid")

suf='ns'
total = pd.DataFrame()
order=['xfce4', 'kitty', 'alacritty']
#bases = ['xfce4-terminal-80x52', 'alacritty-80x55', 'kitty-80x55']
bases = ['xfce4-terminal-383x74', 'alacritty-382x74', 'kitty-382x74-nvidia']
for b in bases:
    for i in range(1,6):
        print('data/d0-' + b + '-' + str(i))
        xterm=pd.read_json('data/d0-' + b + '-' + str(i))
        print(xterm)
        print(xterm.keys())
        demo = xterm['notcurses-demo']
        print(demo.keys())
        runs = demo['runs']
        print(runs.keys())
        print(runs.values())
        nruns = json_normalize(runs);
        nruns.insert(0, "Term", b.split('-')[0])
        total = pd.concat([total, nruns], ignore_index=True)
print(total)
print('*********************************')
for k in total.keys():
    tokes = k.split('.')
    if len(tokes) == 2 and tokes[1] != suf:
        total.drop(columns=k, inplace=True)
    elif len(tokes) == 2:
        total.rename(columns={k:tokes[0]}, inplace=True)
print(total)
print('*********************************')
print(total.keys())
mtotal = pd.melt(total,
                id_vars=["Term"],
                var_name="Demo")
print(mtotal)
mtot = mtotal.astype({'value':'int64'}).sort_values("value", ascending=True)
print(mtot)

fig, lax = plt.subplots()
#lax.set(yscale="log", ylim=[1000000, 10000000000])
lax.set(yscale="log", ylim=[8000000, 400000000000])
title='382x74, Hack 10, Broadwell-E 6950X+NVIDIA GTX 1080 → Dell U3417W'
#title='80x52, Hack 10, Kaby Lake 8550U → Lenovo T580'
ax = sns.stripplot(x="Demo", y='value', data=mtot,  hue="Term", orient='v', dodge=True, ax=lax, hue_order=order).set_title(title)
fig.set_size_inches(18, 8)
plt.legend(bbox_to_anchor=(1, 1), loc=2)
plt.xlabel('Demo')
plt.ylabel('Nanoseconds')
plt.show()
