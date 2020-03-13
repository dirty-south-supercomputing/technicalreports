#!/usr/bin/python3

import seaborn as sns
import pandas as pd
from pandas.io.json import json_normalize
import matplotlib.pyplot as plt

sns.set(style="darkgrid")

plt.figure(figsize=(20,8))

suffixes = ['ns', 'bytes']
for suf in suffixes:
    total = pd.DataFrame()
    bases = ['xfce4-terminal-80x52', 'alacritty-80x55', 'kitty-80x55']
#    bases = ['xfce4-terminal-383x74', 'alacritty-382x74', 'kitty-382x74-nvidia']
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
    lax.set(yscale="log", ylim=[1000000, 40000000000])
    ax = sns.swarmplot(x="Demo", y='value', data=mtot,  hue="Term", orient='v', dodge=True, ax=lax)
    plt.legend(bbox_to_anchor=(1, 1), loc=2)
    plt.xlabel('Demo')
    plt.ylabel('Nanoseconds')
    plt.show()
    plt.savefig('swarmplot-' + suf + '.png', dpi=350)
