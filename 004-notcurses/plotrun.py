#!/usr/bin/python3

# line graphs of emulators against size

import seaborn as sns
import pandas as pd
import pandas as pd
from pandas.io.json import json_normalize
import matplotlib.pyplot as plt

sns.set(style="darkgrid")

plt.figure(figsize=(11,8))


suffixes = ['ns']#, 'bytes']
for suf in suffixes:
    ttl = pd.DataFrame()
    bases = ['xfce4-vte-52-', 'xfce4-vte2-52-', 'xfce4-vte3-52-',
             'xterm-bitmap-52-', 'xterm-bitmap2-52-',
             'alacritty-52-', 'alacritty-2-52-', 'alacritty-3-52-',
             'kitty-1-52-']
    widths = [] # widths
    times = [] # nanosecond counts
    terms = [] # terminals
    for b in bases:
        for i in range(81,191):
            print('data/' + b + str(i) + '.json')
            xterm=pd.read_json('data/' + b + str(i) + '.json')
            #print(xterm)
            #print(xterm.keys())
            demo = xterm['notcurses-demo']
            #print(demo.keys())
            runs = demo['runs']
            #print(runs.keys())
            #print(runs.values())
            nruns = json_normalize(runs);
            terms.append(b.split('-')[0])
            widths.append(i)
            sum = 0
            idx = 0
            for ii in nruns.values:
                for j in ii:
                    idx = idx + 1
                    if idx % 3 == 0:
                        #print(j)
                        sum = sum + int(j)
            times.append(sum / 1000000000)
            #print('widths: ' + str(widths[-1]) + ' term: ' + terms[-1] + ' sum: ' + str(sum))
            #print('*********************************')
            #print(ttl)

        #print('*********************************')
        #for width, row in ttl.iterrows():
        #    print('width****: ' + str(width + 80));
        #    print('******rows: ' + row);

    sns.set_style("darkgrid")
    title='52 lines, Hack 10 (save xterm), Kaby Lake 8550U → Lenovo T580'
    ax = sns.lineplot(x=widths, y=times, hue=terms)
    ax.set_title(title)
    plt.legend(bbox_to_anchor=(1, 1), loc=2)
    plt.xlabel('Width')
    plt.ylabel('Seconds')
    plt.show()
