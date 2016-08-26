#!/usr/bin/python3
import matplotlib.pyplot as plt
import numpy as np
import sys
from mpl_toolkits.mplot3d import Axes3D
from matplotlib import cm
from matplotlib.ticker import LinearLocator,  FormatStrFormatter
import matplotlib.pyplot as plt
import numpy as np
print(sys.argv[1]);
print(sys.argv[2]);
file = sys.argv[1];
type = sys.argv[2];

if type == "path":
    """
    read a two coloums data file(data.txt), with following format:
        P1 Q1
        P2 Q2
        ...
        P5 Q5
        --- 
        X1 Y1
        X2 Y2
        ...
        Xk Yk
    where (Pi, Qi) will be scattered in the plot,  (Xi, Yi) will be linked together.
    """
    data = np.loadtxt(file);
    peaks = data[0:5, :];
    movement = data[5:, :];

    plt.scatter(peaks[:, 0], peaks[:, 1], c = 'red', marker = 'v');
    plt.plot(movement[:, 0], movement[:, 1]);
    plt.xlim(0, 100);
    plt.ylim(0, 100);
    plt.show();
elif type == "hist":
    """
    read a one coloums data file(data.txt), with following format:
        X1
        X2
        ...
        Xk
    And plot a normalized histogram base on this data.
    """
    data = np.loadtxt(file);
    n, bins, patches = plt.hist(data, 50, normed = 1);
    plt.show();
elif type == "surface":
    """
    read a three coloums data file(data.txt), with following format:
        Assuming i = numberOfX, j = numberOfY
        i   j   i*j
        X1  Y1  Z1
        X1  Y2  Z2
        ...
        X1  Yj  Zj
        ...
        ...
        ...
        Xi  Yj  Zi*j
    And plot a surface base on this data.
    """

    data = np.loadtxt(file);
    fig = plt.figure();
    ax = fig.gca(projection = '3d');
    x = data[:, 0];
    y = data[:, 1];
    z = data[:, 2];
    r = np.int(x[0]);
    c = np.int(y[0]);
    x = x[1:];
    y = y[1:];
    z = z[1:];
    print("x.size=" , x.size);
    print("r=" , r);
    print("c=" , c);
    x = x.reshape(r, c);
    y = y.reshape(r, c);
    z = z.reshape(r, c);

    numberSampleX = 40;
    numberSampleY = 40;
    if r>numberSampleX:
        rs = np.int(r/numberSampleX);
    else:
        rs = 1;
    if c>numberSampleY:
        cs = np.int(c/numberSampleY);
    else:
        cs = 1;

    surf = ax.plot_surface(x, y, z, rstride = rs, cstride = cs, cmap =
            cm.coolwarm, linewidth = 0, antialiased = False);
    plt.show();
elif type == "directionHist":
    """
    read two coloums data file(data.txt), with following format:
        X1 Y1
        X2 Y1
        ...
        Xk Yk
    And plot a histgram base on this data.
    """
    data = np.loadtxt(file);
    x = data[:, 0];
    y = data[:, 1];

    ang = np.arccos(x);
    ang1 = ang * 180/np.pi;
    ang2 = (ang1 * (y>0)) + ((360 - ang1) * (y<= 0))

    n, bins, patches = plt.hist(ang2, 50, normed = 1);
    plt.show();

