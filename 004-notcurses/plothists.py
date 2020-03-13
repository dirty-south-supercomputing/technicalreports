#!/usr/bin/python3

import seaborn as sns
import pandas as pd
from pandas.io.json import json_normalize

total = pd.DataFrame()
bases = ['xfce4-terminal-80x52', 'alacritty-80x55', 'kitty-80x55']
for b in bases:
    for i in range(1,6):
        print(b + str(i))
        xterm=pd.read_json('data/' + b + '-' + str(i))
        print(xterm)
        print(xterm.keys())
        demo = xterm['notcurses-demo']
        print(demo.keys())
        runs = demo['runs']
        print(runs.keys())
        print(runs.values())
        nruns = json_normalize(runs);
        total = pd.concat([total, nruns], ignore_index=True)
print(total)
sns.catplot(data=total)
