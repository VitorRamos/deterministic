#!/bin/bash

mkdir -p results/deater/$1

# instructions

for i in `seq 0 9` ; do 

cat /proc/interrupts > results/deater/$1/run.$i.inst_retired.int.before && \
taskset -c 0 perf stat -e instructions:u,r5000c8:u,cs:u ./binaries/retired_instr.int.x86_64 &> results/deater/$1/run.$i.inst_retired.int.counts && \
cat /proc/interrupts > results/deater/$1/run.$i.inst_retired.int.after ;

cat /proc/interrupts > results/deater/$1/run.$i.inst_retired.fp.before && \
taskset -c 0 perf stat -e instructions:u,r5000c8:u,cs:u ./binaries/retired_instr.fp.x86_64 &> results/deater/$1/run.$i.inst_retired.fp.counts && \
cat /proc/interrupts > results/deater/$1/run.$i.inst_retired.fp.after ;

cat /proc/interrupts > results/deater/$1/run.$i.inst_retired.sse.before && \
taskset -c 0 perf stat -e instructions:u,r5000c8:u,cs:u ./binaries/retired_instr.sse.x86_64 &> results/deater/$1/run.$i.inst_retired.sse.counts && \
cat /proc/interrupts > results/deater/$1/run.$i.inst_retired.sse.after ;

cat /proc/interrupts > results/deater/$1/run.$i.inst_retired.all.before && \
taskset -c 0 perf stat -e instructions:u,r5000c8:u,cs:u ./binaries/retired_instr.x86_64 &> results/deater/$1/run.$i.inst_retired.all.counts && \
cat /proc/interrupts > results/deater/$1/run.$i.inst_retired.all.after ;

done

# branches

for i in `seq 0 9` ; do 

cat /proc/interrupts > results/deater/$1/run.$i.branches_retired.int.before && \
taskset -c 0 perf stat -e branches:u,r5000c8:u,cs:u ./binaries/retired_instr.int.x86_64 &> results/deater/$1/run.$i.branches_retired.int.counts && \
cat /proc/interrupts > results/deater/$1/run.$i.branches_retired.int.after ;

cat /proc/interrupts > results/deater/$1/run.$i.branches_retired.fp.before && \
taskset -c 0 perf stat -e branches:u,r5000c8:u,cs:u ./binaries/retired_instr.fp.x86_64 &> results/deater/$1/run.$i.branches_retired.fp.counts && \
cat /proc/interrupts > results/deater/$1/run.$i.branches_retired.fp.after ;

cat /proc/interrupts > results/deater/$1/run.$i.branches_retired.sse.before && \
taskset -c 0 perf stat -e branches:u,r5000c8:u,cs:u ./binaries/retired_instr.sse.x86_64 &> results/deater/$1/run.$i.branches_retired.sse.counts && \
cat /proc/interrupts > results/deater/$1/run.$i.branches_retired.sse.after ;

cat /proc/interrupts > results/deater/$1/run.$i.branches_retired.all.before && \
taskset -c 0 perf stat -e branches:u,r5000c8:u,cs:u ./binaries/retired_instr.x86_64 &> results/deater/$1/run.$i.branches_retired.all.counts && \
cat /proc/interrupts > results/deater/$1/run.$i.branches_retired.all.after ;

done

# Cond Branches

for i in `seq 0 9` ; do 

cat /proc/interrupts > results/deater/$1/run.$i.cond_branches.int.before && \
 perf stat -e r53008b:u,r5000c8:u,cs:u ./binaries/retired_instr.int.x86_64 &> results/deater/$1/run.$i.cond_branches.int.counts && \
cat /proc/interrupts > results/deater/$1/run.$i.cond_branches.int.after ;

cat /proc/interrupts > results/deater/$1/run.$i.cond_branches.fp.before && \
 perf stat -e r53008b:u,r5000c8:u,cs:u ./binaries/retired_instr.fp.x86_64 &> results/deater/$1/run.$i.cond_branches.fp.counts && \
cat /proc/interrupts > results/deater/$1/run.$i.cond_branches.fp.after ;

cat /proc/interrupts > results/deater/$1/run.$i.cond_branches.sse.before && \
 perf stat -e r53008b:u,r5000c8:u,cs:u ./binaries/retired_instr.sse.x86_64 &> results/deater/$1/run.$i.cond_branches.sse.counts && \
cat /proc/interrupts > results/deater/$1/run.$i.cond_branches.sse.after ;

cat /proc/interrupts > results/deater/$1/run.$i.cond_branches.all.before && \
 perf stat -e r53008b:u,r5000c8:u,cs:u ./binaries/retired_instr.x86_64 &> results/deater/$1/run.$i.cond_branches.all.counts && \
cat /proc/interrupts > results/deater/$1/run.$i.cond_branches.all.after ;

done



# Loads

for i in `seq 0 9` ; do 

cat /proc/interrupts > results/deater/$1/run.$i.loads_retired.int.before && \
taskset -c 0 perf stat -e r5001c0:u,r5000c8:u,cs:u ./binaries/retired_instr.int.x86_64 &> results/deater/$1/run.$i.loads_retired.int.counts && \
cat /proc/interrupts > results/deater/$1/run.$i.loads_retired.int.after ;

cat /proc/interrupts > results/deater/$1/run.$i.loads_retired.fp.before && \
taskset -c 0 perf stat -e r5001c0:u,r5000c8:u,cs:u ./binaries/retired_instr.fp.x86_64 &> results/deater/$1/run.$i.loads_retired.fp.counts && \
cat /proc/interrupts > results/deater/$1/run.$i.loads_retired.fp.after ;

cat /proc/interrupts > results/deater/$1/run.$i.loads_retired.sse.before && \
taskset -c 0 perf stat -e r5001c0:u,r5000c8:u,cs:u ./binaries/retired_instr.sse.x86_64 &> results/deater/$1/run.$i.loads_retired.sse.counts && \
cat /proc/interrupts > results/deater/$1/run.$i.loads_retired.sse.after ;

cat /proc/interrupts > results/deater/$1/run.$i.loads_retired.all.before && \
taskset -c 0 perf stat -e r5001c0:u,r5000c8:u,cs:u ./binaries/retired_instr.x86_64 &> results/deater/$1/run.$i.loads_retired.all.counts && \
cat /proc/interrupts > results/deater/$1/run.$i.loads_retired.all.after ;

done


# Stores

for i in `seq 0 9` ; do 

cat /proc/interrupts > results/deater/$1/run.$i.stores_retired.int.before && \
taskset -c 0 perf stat -e r5002c0:u,r5000c8:u,cs:u ./binaries/retired_instr.int.x86_64 &> results/deater/$1/run.$i.stores_retired.int.counts && \
cat /proc/interrupts > results/deater/$1/run.$i.stores_retired.int.after ;

cat /proc/interrupts > results/deater/$1/run.$i.stores_retired.fp.before && \
taskset -c 0 perf stat -e r5002c0:u,r5000c8:u,cs:u ./binaries/retired_instr.fp.x86_64 &> results/deater/$1/run.$i.stores_retired.fp.counts && \
cat /proc/interrupts > results/deater/$1/run.$i.stores_retired.fp.after ;

cat /proc/interrupts > results/deater/$1/run.$i.stores_retired.sse.before && \
taskset -c 0 perf stat -e r5002c0:u,r5000c8:u,cs:u ./binaries/retired_instr.sse.x86_64 &> results/deater/$1/run.$i.stores_retired.sse.counts && \
cat /proc/interrupts > results/deater/$1/run.$i.stores_retired.sse.after ;

cat /proc/interrupts > results/deater/$1/run.$i.stores_retired.all.before && \
taskset -c 0 perf stat -e r5002c0:u,r5000c8:u,cs:u ./binaries/retired_instr.x86_64 &> results/deater/$1/run.$i.stores_retired.all.counts && \
cat /proc/interrupts > results/deater/$1/run.$i.stores_retired.all.after ;

done


# Uops

for i in `seq 0 9` ; do 

cat /proc/interrupts > results/deater/$1/run.$i.uops_retired.int.before && \
taskset -c 0 perf stat -e r500fc2:u,r5000c8:u,cs:u ./binaries/retired_instr.int.x86_64 &> results/deater/$1/run.$i.uops_retired.int.counts && \
cat /proc/interrupts > results/deater/$1/run.$i.uops_retired.int.after ;

cat /proc/interrupts > results/deater/$1/run.$i.uops_retired.fp.before && \
taskset -c 0 perf stat -e r500fc2:u,r5000c8:u,cs:u ./binaries/retired_instr.fp.x86_64 &> results/deater/$1/run.$i.uops_retired.fp.counts && \
cat /proc/interrupts > results/deater/$1/run.$i.uops_retired.fp.after ;

cat /proc/interrupts > results/deater/$1/run.$i.uops_retired.sse.before && \
taskset -c 0 perf stat -e r500fc2:u,r5000c8:u,cs:u ./binaries/retired_instr.sse.x86_64 &> results/deater/$1/run.$i.uops_retired.sse.counts && \
cat /proc/interrupts > results/deater/$1/run.$i.uops_retired.sse.after ;

cat /proc/interrupts > results/deater/$1/run.$i.uops_retired.all.before && \
taskset -c 0 perf stat -e r500fc2:u,r5000c8:u,cs:u ./binaries/retired_instr.x86_64 &> results/deater/$1/run.$i.uops_retired.all.counts && \
cat /proc/interrupts > results/deater/$1/run.$i.uops_retired.all.after ;

done


# Muls

for i in `seq 0 9` ; do 

cat /proc/interrupts > results/deater/$1/run.$i.muls_retired.int.before && \
taskset -c 0 perf stat -e r5000c8:u,r510012:u,cs:u ./binaries/retired_instr.int.x86_64 &> results/deater/$1/run.$i.muls_retired.int.counts && \
cat /proc/interrupts > results/deater/$1/run.$i.muls_retired.int.after ;

cat /proc/interrupts > results/deater/$1/run.$i.muls_retired.fp.before && \
taskset -c 0 perf stat -e r5000c8:u,r510012:u,cs:u ./binaries/retired_instr.fp.x86_64 &> results/deater/$1/run.$i.muls_retired.fp.counts && \
cat /proc/interrupts > results/deater/$1/run.$i.muls_retired.fp.after ;

cat /proc/interrupts > results/deater/$1/run.$i.muls_retired.sse.before && \
taskset -c 0 perf stat -e r5000c8:u,r510012:u,cs:u ./binaries/retired_instr.sse.x86_64 &> results/deater/$1/run.$i.muls_retired.sse.counts && \
cat /proc/interrupts > results/deater/$1/run.$i.muls_retired.sse.after ;

cat /proc/interrupts > results/deater/$1/run.$i.muls_retired.all.before && \
taskset -c 0 perf stat -e r5000c8:u,r510012:u,cs:u ./binaries/retired_instr.x86_64 &> results/deater/$1/run.$i.muls_retired.all.counts && \
cat /proc/interrupts > results/deater/$1/run.$i.muls_retired.all.after ;

done


# Divs

for i in `seq 0 9` ; do 

cat /proc/interrupts > results/deater/$1/run.$i.divs_retired.int.before && \
taskset -c 0 perf stat -e r5000c8:u,r510013:u,cs:u ./binaries/retired_instr.int.x86_64 &> results/deater/$1/run.$i.divs_retired.int.counts && \
cat /proc/interrupts > results/deater/$1/run.$i.divs_retired.int.after ;

cat /proc/interrupts > results/deater/$1/run.$i.divs_retired.fp.before && \
taskset -c 0 perf stat -e r5000c8:u,r510013:u,cs:u ./binaries/retired_instr.fp.x86_64 &> results/deater/$1/run.$i.divs_retired.fp.counts && \
cat /proc/interrupts > results/deater/$1/run.$i.divs_retired.fp.after ;

cat /proc/interrupts > results/deater/$1/run.$i.divs_retired.sse.before && \
taskset -c 0 perf stat -e r5000c8:u,r510013:u,cs:u ./binaries/retired_instr.sse.x86_64 &> results/deater/$1/run.$i.divs_retired.sse.counts && \
cat /proc/interrupts > results/deater/$1/run.$i.divs_retired.sse.after ;

cat /proc/interrupts > results/deater/$1/run.$i.divs_retired.all.before && \
taskset -c 0 perf stat -e r5000c8:u,r510013:u,cs:u ./binaries/retired_instr.x86_64 &> results/deater/$1/run.$i.divs_retired.all.counts && \
cat /proc/interrupts > results/deater/$1/run.$i.divs_retired.all.after ;

done


# FP

for i in `seq 0 9` ; do 

cat /proc/interrupts > results/deater/$1/run.$i.fp_retired.int.before && \
taskset -c 0 perf stat -e r500010:u,r50fec1:u,cs:u ./binaries/retired_instr.int.x86_64 &> results/deater/$1/run.$i.fp_retired.int.counts && \
cat /proc/interrupts > results/deater/$1/run.$i.fp_retired.int.after ;

cat /proc/interrupts > results/deater/$1/run.$i.fp_retired.fp.before && \
taskset -c 0 perf stat -e r500010:u,r50fec1:u,cs:u ./binaries/retired_instr.fp.x86_64 &> results/deater/$1/run.$i.fp_retired.fp.counts && \
cat /proc/interrupts > results/deater/$1/run.$i.fp_retired.fp.after ;

cat /proc/interrupts > results/deater/$1/run.$i.fp_retired.sse.before && \
taskset -c 0 perf stat -e r500010:u,r50fec1:u,cs:u ./binaries/retired_instr.sse.x86_64 &> results/deater/$1/run.$i.fp_retired.sse.counts && \
cat /proc/interrupts > results/deater/$1/run.$i.fp_retired.sse.after ;

cat /proc/interrupts > results/deater/$1/run.$i.fp_retired.all.before && \
taskset -c 0 perf stat -e r500010:u,r50fec1:u,cs:u ./binaries/retired_instr.x86_64 &> results/deater/$1/run.$i.fp_retired.all.counts && \
cat /proc/interrupts > results/deater/$1/run.$i.fp_retired.all.after ;

done



# SSE

for i in `seq 0 9` ; do 

cat /proc/interrupts > results/deater/$1/run.$i.sse_retired.int.before && \
taskset -c 0 perf stat -e r5000ce:u,r5000c8:u,cs:u ./binaries/retired_instr.int.x86_64 &> results/deater/$1/run.$i.sse_retired.int.counts && \
cat /proc/interrupts > results/deater/$1/run.$i.sse_retired.int.after ;

cat /proc/interrupts > results/deater/$1/run.$i.sse_retired.fp.before && \
taskset -c 0 perf stat -e r5000ce:u,r5000c8:u,cs:u ./binaries/retired_instr.fp.x86_64 &> results/deater/$1/run.$i.sse_retired.fp.counts && \
cat /proc/interrupts > results/deater/$1/run.$i.sse_retired.fp.after ;

cat /proc/interrupts > results/deater/$1/run.$i.sse_retired.sse.before && \
taskset -c 0 perf stat -e r5000ce:u,r5000c8:u,cs:u ./binaries/retired_instr.sse.x86_64 &> results/deater/$1/run.$i.sse_retired.sse.counts && \
cat /proc/interrupts > results/deater/$1/run.$i.sse_retired.sse.after ;

cat /proc/interrupts > results/deater/$1/run.$i.sse_retired.all.before && \
taskset -c 0 perf stat -e r5000ce:u,r5000c8:u,cs:u ./binaries/retired_instr.x86_64 &> results/deater/$1/run.$i.sse_retired.all.counts && \
cat /proc/interrupts > results/deater/$1/run.$i.sse_retired.all.after ;

done

# SW Prefetch NTA

for i in `seq 0 9` ; do 

cat /proc/interrupts > results/deater/$1/run.$i.swprfnta_retired.int.before && \
taskset -c 0 perf stat -e r530007:u,r5000c8:u,cs:u ./binaries/retired_instr.int.x86_64 &> results/deater/$1/run.$i.swprfnta_retired.int.counts && \
cat /proc/interrupts > results/deater/$1/run.$i.swprfnta_retired.int.after ;

cat /proc/interrupts > results/deater/$1/run.$i.swprfnta_retired.fp.before && \
taskset -c 0 perf stat -e r530007:u,r5000c8:u,cs:u ./binaries/retired_instr.fp.x86_64 &> results/deater/$1/run.$i.swprfnta_retired.fp.counts && \
cat /proc/interrupts > results/deater/$1/run.$i.swprfnta_retired.fp.after ;

cat /proc/interrupts > results/deater/$1/run.$i.swprfnta_retired.sse.before && \
taskset -c 0 perf stat -e r530007:u,r5000c8:u,cs:u ./binaries/retired_instr.sse.x86_64 &> results/deater/$1/run.$i.swprfnta_retired.sse.counts && \
cat /proc/interrupts > results/deater/$1/run.$i.swprfnta_retired.sse.after ;

cat /proc/interrupts > results/deater/$1/run.$i.swprfnta_retired.all.before && \
taskset -c 0 perf stat -e r530007:u,r5000c8:u,cs:u ./binaries/retired_instr.x86_64 &> results/deater/$1/run.$i.swprfnta_retired.all.counts && \
cat /proc/interrupts > results/deater/$1/run.$i.swprfnta_retired.all.after ;

done

# SW Prefetch L1

for i in `seq 0 9` ; do 

cat /proc/interrupts > results/deater/$1/run.$i.swprfl1_retired.int.before && \
taskset -c 0 perf stat -e r530107:u,r5000c8:u,cs:u ./binaries/retired_instr.int.x86_64 &> results/deater/$1/run.$i.swprfl1_retired.int.counts && \
cat /proc/interrupts > results/deater/$1/run.$i.swprfl1_retired.int.after ;

cat /proc/interrupts > results/deater/$1/run.$i.swprfl1_retired.fp.before && \
taskset -c 0 perf stat -e r530107:u,r5000c8:u,cs:u ./binaries/retired_instr.fp.x86_64 &> results/deater/$1/run.$i.swprfl1_retired.fp.counts && \
cat /proc/interrupts > results/deater/$1/run.$i.swprfl1_retired.fp.after ;

cat /proc/interrupts > results/deater/$1/run.$i.swprfl1_retired.sse.before && \
taskset -c 0 perf stat -e r530107:u,r5000c8:u,cs:u ./binaries/retired_instr.sse.x86_64 &> results/deater/$1/run.$i.swprfl1_retired.sse.counts && \
cat /proc/interrupts > results/deater/$1/run.$i.swprfl1_retired.sse.after ;

cat /proc/interrupts > results/deater/$1/run.$i.swprfl1_retired.all.before && \
taskset -c 0 perf stat -e r530107:u,r5000c8:u,cs:u ./binaries/retired_instr.x86_64 &> results/deater/$1/run.$i.swprfl1_retired.all.counts && \
cat /proc/interrupts > results/deater/$1/run.$i.swprfl1_retired.all.after ;

done


# SW Prefetch L2

for i in `seq 0 9` ; do 

cat /proc/interrupts > results/deater/$1/run.$i.swprfl2_retired.int.before && \
taskset -c 0 perf stat -e r530207:u,r5000c8:u,cs:u ./binaries/retired_instr.int.x86_64 &> results/deater/$1/run.$i.swprfl2_retired.int.counts && \
cat /proc/interrupts > results/deater/$1/run.$i.swprfl2_retired.int.after ;

cat /proc/interrupts > results/deater/$1/run.$i.swprfl2_retired.fp.before && \
taskset -c 0 perf stat -e r530207:u,r5000c8:u,cs:u ./binaries/retired_instr.fp.x86_64 &> results/deater/$1/run.$i.swprfl2_retired.fp.counts && \
cat /proc/interrupts > results/deater/$1/run.$i.swprfl2_retired.fp.after ;

cat /proc/interrupts > results/deater/$1/run.$i.swprfl2_retired.sse.before && \
taskset -c 0 perf stat -e r530207:u,r5000c8:u,cs:u ./binaries/retired_instr.sse.x86_64 &> results/deater/$1/run.$i.swprfl2_retired.sse.counts && \
cat /proc/interrupts > results/deater/$1/run.$i.swprfl2_retired.sse.after ;

cat /proc/interrupts > results/deater/$1/run.$i.swprfl2_retired.all.before && \
taskset -c 0 perf stat -e r530207:u,r5000c8:u,cs:u ./binaries/retired_instr.x86_64 &> results/deater/$1/run.$i.swprfl2_retired.all.counts && \
cat /proc/interrupts > results/deater/$1/run.$i.swprfl2_retired.all.after ;

done

