# scons build file for building MRIDSS outside of xcode

import os

# Compiler flags, libraries etc.
env = Environment(CXX = 'mpic++',CCFLAGS = '-Wall -Ofast -ffast-math -march=native')
print "CXX is:", env['CXX']
env.Append(CPPPATH = ['/usr/local/include/','/usr/local/hdf5/include'])
env.Append(LIBPATH = ['/usr/local/lib/','/usr/local/hdf5/lib'])
env.Append(LIBS = ['fftw3',File('/usr/local/hdf5/lib/libhdf5_hl.a'),File('/usr/local/hdf5/lib/libhdf5.a'),'z','dl','m'])

# Source files

base = 'QL_DNS/'
models = base+'Models/'
aux = base+'Auxiliary/'
integs = base+'Integrators/'
s = '   '

basesrc = Split(base + 'main.cpp'    +s+    base + 'solution.cpp')
modsrc = Split(models+'ConstantDamping.cpp'   +s+   models+'MHD_BQlin.cpp'    +s+   models+'MHD_FullQlin.cpp'    +s+   models+'CompMHDKinetic_FixedMF.cpp'   +s+   models+'CompMHDIso_FixedMF.cpp'   +s+    models+'HD_fullU.cpp')
intsrc = Split(integs+'Euler.cpp'   +s+     integs+'EulerCN.cpp'    +s+   integs+'RK3CN.cpp'    +s+   integs+'RK3CN_Elin.cpp')
auxsrc = Split(aux+'Input_parameters.cpp'     +s+     aux+'MPIdata.cpp'    +s+    aux+'fftwPlans.cpp'    +s+    aux+'Kdata.cpp'    +s+   aux+'FullSave_Load.cpp'   +s+   aux+'TimeVariables.cpp'    +s+    aux+'InterfaceOutput.cpp')

# Compile
env.Program('mridns_prog',basesrc+modsrc+intsrc+auxsrc)



