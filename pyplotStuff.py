#!/home/ddong/anaconda2/bin/python
'''Ploting results'''
import sys
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import numpy as np
from matplotlib.backends.backend_pdf import PdfPages

rfname = 'Results.dat'
ptname = 'targetP.dat'
adname = 'Adjoint.dat'

# Read results and assign to variables
data = np.loadtxt(rfname)
nx   = data[0]

lbound = 1
# Read results
x = data[lbound:lbound+nx]
lbound += nx
pres = data[lbound:lbound+nx]
lbound += nx
rho  = data[lbound:lbound+nx]
lbound += nx
Mach = data[lbound:lbound+nx]
lbound += nx

# Read shape results
xhalf= data[lbound:lbound+nx+1]
lbound += nx+1
S = data[lbound:lbound+nx+1]
lbound += nx+1

# Read convergence information
conv = data[lbound:len(data)]
# Read target pressure
data = np.loadtxt(ptname)
nx = data[0]

targetp=data[1:nx+1]

# Read Adjoint Results
data = np.loadtxt(adname)
adj1=data[0*nx+1:1*nx + 1]
adj2=data[1*nx+1:2*nx + 1]
adj3=data[2*nx+1:3*nx + 1]
pp=PdfPages('Figures.pdf')


# Plot Channel
plt.figure()
plt.title('Channel Shape')
PressureCurve = plt.plot(xhalf,S,'-ob',markerfacecolor='None', markeredgecolor='b')
plt.grid(b=True, which='major', color='black', linestyle='-',alpha=0.5)
pp.savefig()

# Plot Pressure and Target Pressure
plt.figure()
plt.title('Pressure Distribution')
PressureCurve = plt.plot(x,pres,'-ob',markerfacecolor='None', markeredgecolor='b', label='Current')
TargetPCurve  = plt.plot(x,targetp,'-xr',markerfacecolor='None', markeredgecolor='r', label='Target')
plt.grid(b=True, which='major', color='black', linestyle='-',alpha=0.5)
plt.legend(loc='upper right')
pp.savefig()


# Plot Mach Distribution
plt.figure()
plt.title('Mach Distribution')
adj1Curve = plt.plot(x,Mach,'-ob',markerfacecolor='None', markeredgecolor='b')
plt.grid(b=True, which='major', color='black', linestyle='-',alpha=0.5)
pp.savefig()

# Plot Convergence
plt.figure()
plt.title('Convergence')
itIndex1=0
itIndex2=len(conv)/2-1
resIndex1=len(conv)/2
resIndex2=len(conv)-1
convCurve = plt.loglog(conv[itIndex1:itIndex2],conv[resIndex1:resIndex2], '-x')
plt.grid(b=True, which='both', color='black', linestyle='-',alpha=0.5)
pp.savefig()

# Plot Adjoint Distribution
plt.figure()
plt.title('Adjoint Distribution')
adj1Curve = plt.plot(x,adj1,'-ob',markerfacecolor='None', markeredgecolor='b',label='Adj 1')
plt.grid(b=True, which='major', color='black', linestyle='-',alpha=0.5)
pp.savefig()

# Plot Adjoint Distribution
plt.figure()
plt.title('Adjoint Distribution')
adj2Curve = plt.plot(x,adj2,'-or',markerfacecolor='None', markeredgecolor='r',label='Adj 1')
plt.grid(b=True, which='major', color='black', linestyle='-',alpha=0.5)
pp.savefig()

# Plot Adjoint Distribution
plt.figure()
plt.title('Adjoint Distribution')
adj3Curve = plt.plot(x,adj3,'-og',markerfacecolor='None', markeredgecolor='g',label='Adj 3')
plt.grid(b=True, which='major', color='black', linestyle='-',alpha=0.5)
pp.savefig()


# Close PDF
pp.close()

