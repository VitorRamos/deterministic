#!/bin/bash

mkdir -p results/toad4/$1

# instructions

for i in `seq 0 9` ; do 

cat /proc/interrupts > results/toad4/$1/run.$i.inst_retired.int.before && \
 perf stat -e instructions:u,r5301cb:u,cs:u ./binaries/retired_instr.int.x86_64 &> results/toad4/$1/run.$i.inst_retired.int.counts && \
cat /proc/interrupts > results/toad4/$1/run.$i.inst_retired.int.after ;

cat /proc/interrupts > results/toad4/$1/run.$i.inst_retired.fp.before && \
 perf stat -e instructions:u,r5301cb:u,cs:u ./binaries/retired_instr.fp.x86_64 &> results/toad4/$1/run.$i.inst_retired.fp.counts && \
cat /proc/interrupts > results/toad4/$1/run.$i.inst_retired.fp.after ;

cat /proc/interrupts > results/toad4/$1/run.$i.inst_retired.sse.before && \
 perf stat -e instructions:u,r5301cb:u,cs:u ./binaries/retired_instr.sse.x86_64 &> results/toad4/$1/run.$i.inst_retired.sse.counts && \
cat /proc/interrupts > results/toad4/$1/run.$i.inst_retired.sse.after ;

cat /proc/interrupts > results/toad4/$1/run.$i.inst_retired.all.before && \
 perf stat -e instructions:u,r5301cb:u,cs:u ./binaries/retired_instr.x86_64 &> results/toad4/$1/run.$i.inst_retired.all.counts && \
cat /proc/interrupts > results/toad4/$1/run.$i.inst_retired.all.after ;

done

# branches

for i in `seq 0 9` ; do 

cat /proc/interrupts > results/toad4/$1/run.$i.branches_retired.int.before && \
 perf stat -e branches:u,r5301cb:u,cs:u ./binaries/retired_instr.int.x86_64 &> results/toad4/$1/run.$i.branches_retired.int.counts && \
cat /proc/interrupts > results/toad4/$1/run.$i.branches_retired.int.after ;

cat /proc/interrupts > results/toad4/$1/run.$i.branches_retired.fp.before && \
 perf stat -e branches:u,r5301cb:u,cs:u ./binaries/retired_instr.fp.x86_64 &> results/toad4/$1/run.$i.branches_retired.fp.counts && \
cat /proc/interrupts > results/toad4/$1/run.$i.branches_retired.fp.after ;

cat /proc/interrupts > results/toad4/$1/run.$i.branches_retired.sse.before && \
 perf stat -e branches:u,r5301cb:u,cs:u ./binaries/retired_instr.sse.x86_64 &> results/toad4/$1/run.$i.branches_retired.sse.counts && \
cat /proc/interrupts > results/toad4/$1/run.$i.branches_retired.sse.after ;

cat /proc/interrupts > results/toad4/$1/run.$i.branches_retired.all.before && \
 perf stat -e branches:u,r5301cb:u,cs:u ./binaries/retired_instr.x86_64 &> results/toad4/$1/run.$i.branches_retired.all.counts && \
cat /proc/interrupts > results/toad4/$1/run.$i.branches_retired.all.after ;

done


# Loads

for i in `seq 0 9` ; do 

cat /proc/interrupts > results/toad4/$1/run.$i.loads_retired.int.before && \
 perf stat -e r5381d0:u,r5301cb:u,cs:u ./binaries/retired_instr.int.x86_64 &> results/toad4/$1/run.$i.loads_retired.int.counts && \
cat /proc/interrupts > results/toad4/$1/run.$i.loads_retired.int.after ;

cat /proc/interrupts > results/toad4/$1/run.$i.loads_retired.fp.before && \
 perf stat -e r5381d0:u,r5301cb:u,cs:u ./binaries/retired_instr.fp.x86_64 &> results/toad4/$1/run.$i.loads_retired.fp.counts && \
cat /proc/interrupts > results/toad4/$1/run.$i.loads_retired.fp.after ;

cat /proc/interrupts > results/toad4/$1/run.$i.loads_retired.sse.before && \
 perf stat -e r5381d0:u,r5301cb:u,cs:u ./binaries/retired_instr.sse.x86_64 &> results/toad4/$1/run.$i.loads_retired.sse.counts && \
cat /proc/interrupts > results/toad4/$1/run.$i.loads_retired.sse.after ;

cat /proc/interrupts > results/toad4/$1/run.$i.loads_retired.all.before && \
 perf stat -e r5381d0:u,r5301cb:u,cs:u ./binaries/retired_instr.x86_64 &> results/toad4/$1/run.$i.loads_retired.all.counts && \
cat /proc/interrupts > results/toad4/$1/run.$i.loads_retired.all.after ;

done


# Stores

for i in `seq 0 9` ; do 

cat /proc/interrupts > results/toad4/$1/run.$i.stores_retired.int.before && \
 perf stat -e r5382d0:u,r5301cb:u,cs:u ./binaries/retired_instr.int.x86_64 &> results/toad4/$1/run.$i.stores_retired.int.counts && \
cat /proc/interrupts > results/toad4/$1/run.$i.stores_retired.int.after ;

cat /proc/interrupts > results/toad4/$1/run.$i.stores_retired.fp.before && \
 perf stat -e r5382d0:u,r5301cb:u,cs:u ./binaries/retired_instr.fp.x86_64 &> results/toad4/$1/run.$i.stores_retired.fp.counts && \
cat /proc/interrupts > results/toad4/$1/run.$i.stores_retired.fp.after ;

cat /proc/interrupts > results/toad4/$1/run.$i.stores_retired.sse.before && \
 perf stat -e r5382d0:u,r5301cb:u,cs:u ./binaries/retired_instr.sse.x86_64 &> results/toad4/$1/run.$i.stores_retired.sse.counts && \
cat /proc/interrupts > results/toad4/$1/run.$i.stores_retired.sse.after ;

cat /proc/interrupts > results/toad4/$1/run.$i.stores_retired.all.before && \
 perf stat -e r5382d0:u,r5301cb:u,cs:u ./binaries/retired_instr.x86_64 &> results/toad4/$1/run.$i.stores_retired.all.counts && \
cat /proc/interrupts > results/toad4/$1/run.$i.stores_retired.all.after ;

done


# Uops

for i in `seq 0 9` ; do 

cat /proc/interrupts > results/toad4/$1/run.$i.uops_retired.int.before && \
 perf stat -e r5301c2:u,r5301cb:u,cs:u ./binaries/retired_instr.int.x86_64 &> results/toad4/$1/run.$i.uops_retired.int.counts && \
cat /proc/interrupts > results/toad4/$1/run.$i.uops_retired.int.after ;

cat /proc/interrupts > results/toad4/$1/run.$i.uops_retired.fp.before && \
 perf stat -e r5301c2:u,r5301cb:u,cs:u ./binaries/retired_instr.fp.x86_64 &> results/toad4/$1/run.$i.uops_retired.fp.counts && \
cat /proc/interrupts > results/toad4/$1/run.$i.uops_retired.fp.after ;

cat /proc/interrupts > results/toad4/$1/run.$i.uops_retired.sse.before && \
 perf stat -e r5301c2:u,r5301cb:u,cs:u ./binaries/retired_instr.sse.x86_64 &> results/toad4/$1/run.$i.uops_retired.sse.counts && \
cat /proc/interrupts > results/toad4/$1/run.$i.uops_retired.sse.after ;

cat /proc/interrupts > results/toad4/$1/run.$i.uops_retired.all.before && \
 perf stat -e r5301c2:u,r5301cb:u,cs:u ./binaries/retired_instr.x86_64 &> results/toad4/$1/run.$i.uops_retired.all.counts && \
cat /proc/interrupts > results/toad4/$1/run.$i.uops_retired.all.after ;

done


# Cond Branches

for i in `seq 0 9` ; do 

cat /proc/interrupts > results/toad4/$1/run.$i.cond_branches.int.before && \
 perf stat -e r5301c4:u,r5301cb:u,cs:u ./binaries/retired_instr.int.x86_64 &> results/toad4/$1/run.$i.cond_branches.int.counts && \
cat /proc/interrupts > results/toad4/$1/run.$i.cond_branches.int.after ;

cat /proc/interrupts > results/toad4/$1/run.$i.cond_branches.fp.before && \
 perf stat -e r5301c4:u,r5301cb:u,cs:u ./binaries/retired_instr.fp.x86_64 &> results/toad4/$1/run.$i.cond_branches.fp.counts && \
cat /proc/interrupts > results/toad4/$1/run.$i.cond_branches.fp.after ;

cat /proc/interrupts > results/toad4/$1/run.$i.cond_branches.sse.before && \
 perf stat -e r5301c4:u,r5301cb:u,cs:u ./binaries/retired_instr.sse.x86_64 &> results/toad4/$1/run.$i.cond_branches.sse.counts && \
cat /proc/interrupts > results/toad4/$1/run.$i.cond_branches.sse.after ;

cat /proc/interrupts > results/toad4/$1/run.$i.cond_branches.all.before && \
 perf stat -e r5301c4:u,r5301cb:u,cs:u ./binaries/retired_instr.x86_64 &> results/toad4/$1/run.$i.cond_branches.all.counts && \
cat /proc/interrupts > results/toad4/$1/run.$i.cond_branches.all.after ;

done


# Divs

for i in `seq 0 9` ; do 

cat /proc/interrupts > results/toad4/$1/run.$i.divs_retired.int.before && \
 perf stat -e r5301cb:u,r1570114:u,cs:u ./binaries/retired_instr.int.x86_64 &> results/toad4/$1/run.$i.divs_retired.int.counts && \
cat /proc/interrupts > results/toad4/$1/run.$i.divs_retired.int.after ;

cat /proc/interrupts > results/toad4/$1/run.$i.divs_retired.fp.before && \
 perf stat -e r5301cb:u,r1570114:u,cs:u ./binaries/retired_instr.fp.x86_64 &> results/toad4/$1/run.$i.divs_retired.fp.counts && \
cat /proc/interrupts > results/toad4/$1/run.$i.divs_retired.fp.after ;

cat /proc/interrupts > results/toad4/$1/run.$i.divs_retired.sse.before && \
 perf stat -e r5301cb:u,r1570114:u,cs:u ./binaries/retired_instr.sse.x86_64 &> results/toad4/$1/run.$i.divs_retired.sse.counts && \
cat /proc/interrupts > results/toad4/$1/run.$i.divs_retired.sse.after ;

cat /proc/interrupts > results/toad4/$1/run.$i.divs_retired.all.before && \
 perf stat -e r5301cb:u,r1570114:u,cs:u ./binaries/retired_instr.x86_64 &> results/toad4/$1/run.$i.divs_retired.all.counts && \
cat /proc/interrupts > results/toad4/$1/run.$i.divs_retired.all.after ;

done


# FP

for i in `seq 0 9` ; do 

cat /proc/interrupts > results/toad4/$1/run.$i.fp_retired.int.before && \
 perf stat -e r530110:u,r5302c0:u,cs:u ./binaries/retired_instr.int.x86_64 &> results/toad4/$1/run.$i.fp_retired.int.counts && \
cat /proc/interrupts > results/toad4/$1/run.$i.fp_retired.int.after ;

cat /proc/interrupts > results/toad4/$1/run.$i.fp_retired.fp.before && \
 perf stat -e r530110:u,r5302c0:u,cs:u ./binaries/retired_instr.fp.x86_64 &> results/toad4/$1/run.$i.fp_retired.fp.counts && \
cat /proc/interrupts > results/toad4/$1/run.$i.fp_retired.fp.after ;

cat /proc/interrupts > results/toad4/$1/run.$i.fp_retired.sse.before && \
 perf stat -e r530110:u,r5302c0:u,cs:u ./binaries/retired_instr.sse.x86_64 &> results/toad4/$1/run.$i.fp_retired.sse.counts && \
cat /proc/interrupts > results/toad4/$1/run.$i.fp_retired.sse.after ;

cat /proc/interrupts > results/toad4/$1/run.$i.fp_retired.all.before && \
 perf stat -e r530110:u,r5302c0:u,cs:u ./binaries/retired_instr.x86_64 &> results/toad4/$1/run.$i.fp_retired.all.counts && \
cat /proc/interrupts > results/toad4/$1/run.$i.fp_retired.all.after ;

done



# SSE

for i in `seq 0 9` ; do 

cat /proc/interrupts > results/toad4/$1/run.$i.sse_retired.int.before && \
 perf stat -e r53f010:u,r5301cb:u,cs:u ./binaries/retired_instr.int.x86_64 &> results/toad4/$1/run.$i.sse_retired.int.counts && \
cat /proc/interrupts > results/toad4/$1/run.$i.sse_retired.int.after ;

cat /proc/interrupts > results/toad4/$1/run.$i.sse_retired.fp.before && \
 perf stat -e r53f010:u,r5301cb:u,cs:u ./binaries/retired_instr.fp.x86_64 &> results/toad4/$1/run.$i.sse_retired.fp.counts && \
cat /proc/interrupts > results/toad4/$1/run.$i.sse_retired.fp.after ;

cat /proc/interrupts > results/toad4/$1/run.$i.sse_retired.sse.before && \
 perf stat -e r53f010:u,r5301cb:u,cs:u ./binaries/retired_instr.sse.x86_64 &> results/toad4/$1/run.$i.sse_retired.sse.counts && \
cat /proc/interrupts > results/toad4/$1/run.$i.sse_retired.sse.after ;

cat /proc/interrupts > results/toad4/$1/run.$i.sse_retired.all.before && \
 perf stat -e r53f010:u,r5301cb:u,cs:u ./binaries/retired_instr.x86_64 &> results/toad4/$1/run.$i.sse_retired.all.counts && \
cat /proc/interrupts > results/toad4/$1/run.$i.sse_retired.all.after ;

done
