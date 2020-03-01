#!/usr/bin/python3

import matplotlib.pyplot as plt
import numpy as np

plt.rcdefaults()
fig, ax = plt.subplots(figsize=(18,5))

# Example data
demos = ('1991-10   1.0',
         '1.0.1',
         '1.1',
         '1996-07   2.0',
         '2.1',
         '1999-09   3.0',
         '3.1',
         '3.2',
         '2003-04   4.0',
         '4.1',
         '2006-07   5.0',
         '5.1',
         '5.2',
         '2010-10   6.0',
         '6.1',
         '6.2',
         '6.3',
         '2014-06   7.0',
         '2015-06   8.0',
         '2016-06   9.0',
         '2017-06  10.0',
         '2018-06  11.0',
         '2019-03  12.0',
         '12.1',
         '2020-03  13.0')
chars = (7161, 28359, 34233, 38950, 38952,
         49259, 94205, 95221, 96447, 97720,
         99089, 100713, 107361, 109449, 110181, 110182, 110187,
         113021, 120737, 128237, 136755, 137439, 137993, 137994,
         143859)

y_pos = np.arange(len(demos))
performance = 3 + 10 * np.random.rand(len(demos))
error = np.random.rand(len(demos))

for i, v in enumerate(chars):
  ax.text(v + 3, i + .25, str(v), color='blue', fontweight='bold')

ax.barh(y_pos, chars, xerr=error, align='center')
ax.set_yticks(y_pos)
ax.set_yticklabels(demos)
ax.invert_yaxis()  # labels read top-to-bottom
ax.set_xlabel('Characters')
ax.set_title('Growth of UCS, 1991–2020')

plt.show()
