#!/bin/bash

mkdir -p results/server/$1

# instructions

for i in `seq 0 9` ; do 

cat /proc/interrupts > results/server/$1/run.$i.inst_retired.int.before && \
taskset -c 0 perf stat -e instructions:u,r5100c8:u,cs:u ./binaries/retired_instr.int.x86_64 &> results/server/$1/run.$i.inst_retired.int.counts && \
cat /proc/interrupts > results/server/$1/run.$i.inst_retired.int.after ;

cat /proc/interrupts > results/server/$1/run.$i.inst_retired.fp.before && \
taskset -c 0 perf stat -e instructions:u,r5100c8:u,cs:u ./binaries/retired_instr.fp.x86_64 &> results/server/$1/run.$i.inst_retired.fp.counts && \
cat /proc/interrupts > results/server/$1/run.$i.inst_retired.fp.after ;

cat /proc/interrupts > results/server/$1/run.$i.inst_retired.sse.before && \
taskset -c 0 perf stat -e instructions:u,r5100c8:u,cs:u ./binaries/retired_instr.sse.x86_64 &> results/server/$1/run.$i.inst_retired.sse.counts && \
cat /proc/interrupts > results/server/$1/run.$i.inst_retired.sse.after ;

cat /proc/interrupts > results/server/$1/run.$i.inst_retired.all.before && \
taskset -c 0 perf stat -e instructions:u,r5100c8:u,cs:u ./binaries/retired_instr.x86_64 &> results/server/$1/run.$i.inst_retired.all.counts && \
cat /proc/interrupts > results/server/$1/run.$i.inst_retired.all.after ;

done

# branches

for i in `seq 0 9` ; do 

cat /proc/interrupts > results/server/$1/run.$i.branches_retired.int.before && \
taskset -c 0 perf stat -e branches:u,r5100c8:u,cs:u ./binaries/retired_instr.int.x86_64 &> results/server/$1/run.$i.branches_retired.int.counts && \
cat /proc/interrupts > results/server/$1/run.$i.branches_retired.int.after ;

cat /proc/interrupts > results/server/$1/run.$i.branches_retired.fp.before && \
taskset -c 0 perf stat -e branches:u,r5100c8:u,cs:u ./binaries/retired_instr.fp.x86_64 &> results/server/$1/run.$i.branches_retired.fp.counts && \
cat /proc/interrupts > results/server/$1/run.$i.branches_retired.fp.after ;

cat /proc/interrupts > results/server/$1/run.$i.branches_retired.sse.before && \
taskset -c 0 perf stat -e branches:u,r5100c8:u,cs:u ./binaries/retired_instr.sse.x86_64 &> results/server/$1/run.$i.branches_retired.sse.counts && \
cat /proc/interrupts > results/server/$1/run.$i.branches_retired.sse.after ;

cat /proc/interrupts > results/server/$1/run.$i.branches_retired.all.before && \
taskset -c 0 perf stat -e branches:u,r5100c8:u,cs:u ./binaries/retired_instr.x86_64 &> results/server/$1/run.$i.branches_retired.all.counts && \
cat /proc/interrupts > results/server/$1/run.$i.branches_retired.all.after ;

done


# Loads

#for i in `seq 0 9` ; do 

#cat /proc/interrupts > results/server/$1/run.$i.loads_retired.int.before && \
#taskset -c 0 perf stat -e r5001c0:u,r5100c8:u,cs:u ./binaries/retired_instr.int.x86_64 &> results/server/$1/run.$i.loads_retired.int.counts && \
#cat /proc/interrupts > results/server/$1/run.$i.loads_retired.int.after ;

#cat /proc/interrupts > results/server/$1/run.$i.loads_retired.fp.before && \
#taskset -c 0 perf stat -e r5001c0:u,r5100c8:u,cs:u ./binaries/retired_instr.fp.x86_64 &> results/server/$1/run.$i.loads_retired.fp.counts && \
#cat /proc/interrupts > results/server/$1/run.$i.loads_retired.fp.after ;

#cat /proc/interrupts > results/server/$1/run.$i.loads_retired.sse.before && \
#taskset -c 0 perf stat -e r5001c0:u,r5100c8:u,cs:u ./binaries/retired_instr.sse.x86_64 &> results/server/$1/run.$i.loads_retired.sse.counts && \
#cat /proc/interrupts > results/server/$1/run.$i.loads_retired.sse.after ;

#cat /proc/interrupts > results/server/$1/run.$i.loads_retired.all.before && \
#taskset -c 0 perf stat -e r5001c0:u,r5100c8:u,cs:u ./binaries/retired_instr.x86_64 &> results/server/$1/run.$i.loads_retired.all.counts && \
#cat /proc/interrupts > results/server/$1/run.$i.loads_retired.all.after ;

#done


# Stores

#for i in `seq 0 9` ; do 

#cat /proc/interrupts > results/server/$1/run.$i.stores_retired.int.before && \
#taskset -c 0 perf stat -e r5002c0:u,r5100c8:u,cs:u ./binaries/retired_instr.int.x86_64 &> results/server/$1/run.$i.stores_retired.int.counts && \
#cat /proc/interrupts > results/server/$1/run.$i.stores_retired.int.after ;

#cat /proc/interrupts > results/server/$1/run.$i.stores_retired.fp.before && \
#taskset -c 0 perf stat -e r5002c0:u,r5100c8:u,cs:u ./binaries/retired_instr.fp.x86_64 &> results/server/$1/run.$i.stores_retired.fp.counts && \
#cat /proc/interrupts > results/server/$1/run.$i.stores_retired.fp.after ;

#cat /proc/interrupts > results/server/$1/run.$i.stores_retired.sse.before && \
#taskset -c 0 perf stat -e r5002c0:u,r5100c8:u,cs:u ./binaries/retired_instr.sse.x86_64 &> results/server/$1/run.$i.stores_retired.sse.counts && \
#cat /proc/interrupts > results/server/$1/run.$i.stores_retired.sse.after ;

#cat /proc/interrupts > results/server/$1/run.$i.stores_retired.all.before && \
#taskset -c 0 perf stat -e r5002c0:u,r5100c8:u,cs:u ./binaries/retired_instr.x86_64 &> results/server/$1/run.$i.stores_retired.all.counts && \
#cat /proc/interrupts > results/server/$1/run.$i.stores_retired.all.after ;

#done


# Uops

for i in `seq 0 9` ; do 

cat /proc/interrupts > results/server/$1/run.$i.uops_retired.int.before && \
taskset -c 0 perf stat -e r5010c2:u,r5100c8:u,cs:u ./binaries/retired_instr.int.x86_64 &> results/server/$1/run.$i.uops_retired.int.counts && \
cat /proc/interrupts > results/server/$1/run.$i.uops_retired.int.after ;

cat /proc/interrupts > results/server/$1/run.$i.uops_retired.fp.before && \
taskset -c 0 perf stat -e r5010c2:u,r5100c8:u,cs:u ./binaries/retired_instr.fp.x86_64 &> results/server/$1/run.$i.uops_retired.fp.counts && \
cat /proc/interrupts > results/server/$1/run.$i.uops_retired.fp.after ;

cat /proc/interrupts > results/server/$1/run.$i.uops_retired.sse.before && \
taskset -c 0 perf stat -e r5010c2:u,r5100c8:u,cs:u ./binaries/retired_instr.sse.x86_64 &> results/server/$1/run.$i.uops_retired.sse.counts && \
cat /proc/interrupts > results/server/$1/run.$i.uops_retired.sse.after ;

cat /proc/interrupts > results/server/$1/run.$i.uops_retired.all.before && \
taskset -c 0 perf stat -e r5010c2:u,r5100c8:u,cs:u ./binaries/retired_instr.x86_64 &> results/server/$1/run.$i.uops_retired.all.counts && \
cat /proc/interrupts > results/server/$1/run.$i.uops_retired.all.after ;

done


# Muls

for i in `seq 0 9` ; do 

cat /proc/interrupts > results/server/$1/run.$i.muls_retired.int.before && \
taskset -c 0 perf stat -e r5100c8:u,r538112:u,cs:u ./binaries/retired_instr.int.x86_64 &> results/server/$1/run.$i.muls_retired.int.counts && \
cat /proc/interrupts > results/server/$1/run.$i.muls_retired.int.after ;

cat /proc/interrupts > results/server/$1/run.$i.muls_retired.fp.before && \
taskset -c 0 perf stat -e r5100c8:u,r538112:u,cs:u ./binaries/retired_instr.fp.x86_64 &> results/server/$1/run.$i.muls_retired.fp.counts && \
cat /proc/interrupts > results/server/$1/run.$i.muls_retired.fp.after ;

cat /proc/interrupts > results/server/$1/run.$i.muls_retired.sse.before && \
taskset -c 0 perf stat -e r5100c8:u,r538112:u,cs:u ./binaries/retired_instr.sse.x86_64 &> results/server/$1/run.$i.muls_retired.sse.counts && \
cat /proc/interrupts > results/server/$1/run.$i.muls_retired.sse.after ;

cat /proc/interrupts > results/server/$1/run.$i.muls_retired.all.before && \
taskset -c 0 perf stat -e r5100c8:u,r538112:u,cs:u ./binaries/retired_instr.x86_64 &> results/server/$1/run.$i.muls_retired.all.counts && \
cat /proc/interrupts > results/server/$1/run.$i.muls_retired.all.after ;

done


# Divs

for i in `seq 0 9` ; do 

cat /proc/interrupts > results/server/$1/run.$i.divs_retired.int.before && \
taskset -c 0 perf stat -e r5100c8:u,r538113:u,cs:u ./binaries/retired_instr.int.x86_64 &> results/server/$1/run.$i.divs_retired.int.counts && \
cat /proc/interrupts > results/server/$1/run.$i.divs_retired.int.after ;

cat /proc/interrupts > results/server/$1/run.$i.divs_retired.fp.before && \
taskset -c 0 perf stat -e r5100c8:u,r538113:u,cs:u ./binaries/retired_instr.fp.x86_64 &> results/server/$1/run.$i.divs_retired.fp.counts && \
cat /proc/interrupts > results/server/$1/run.$i.divs_retired.fp.after ;

cat /proc/interrupts > results/server/$1/run.$i.divs_retired.sse.before && \
taskset -c 0 perf stat -e r5100c8:u,r538113:u,cs:u ./binaries/retired_instr.sse.x86_64 &> results/server/$1/run.$i.divs_retired.sse.counts && \
cat /proc/interrupts > results/server/$1/run.$i.divs_retired.sse.after ;

cat /proc/interrupts > results/server/$1/run.$i.divs_retired.all.before && \
taskset -c 0 perf stat -e r5100c8:u,r538113:u,cs:u ./binaries/retired_instr.x86_64 &> results/server/$1/run.$i.divs_retired.all.counts && \
cat /proc/interrupts > results/server/$1/run.$i.divs_retired.all.after ;

done


# FP

for i in `seq 0 9` ; do 

cat /proc/interrupts > results/server/$1/run.$i.fp_retired.int.before && \
taskset -c 0 perf stat -e r508110:u,r5100c8:u,cs:u ./binaries/retired_instr.int.x86_64 &> results/server/$1/run.$i.fp_retired.int.counts && \
cat /proc/interrupts > results/server/$1/run.$i.fp_retired.int.after ;

cat /proc/interrupts > results/server/$1/run.$i.fp_retired.fp.before && \
taskset -c 0 perf stat -e r508110:u,r5100c8:u,cs:u ./binaries/retired_instr.fp.x86_64 &> results/server/$1/run.$i.fp_retired.fp.counts && \
cat /proc/interrupts > results/server/$1/run.$i.fp_retired.fp.after ;

cat /proc/interrupts > results/server/$1/run.$i.fp_retired.sse.before && \
taskset -c 0 perf stat -e r508110:u,r5100c8:u,cs:u ./binaries/retired_instr.sse.x86_64 &> results/server/$1/run.$i.fp_retired.sse.counts && \
cat /proc/interrupts > results/server/$1/run.$i.fp_retired.sse.after ;

cat /proc/interrupts > results/server/$1/run.$i.fp_retired.all.before && \
taskset -c 0 perf stat -e r508110:u,r5100c8:u,cs:u ./binaries/retired_instr.x86_64 &> results/server/$1/run.$i.fp_retired.all.counts && \
cat /proc/interrupts > results/server/$1/run.$i.fp_retired.all.after ;

done



# SSE

for i in `seq 0 9` ; do 

cat /proc/interrupts > results/server/$1/run.$i.sse_retired.int.before && \
taskset -c 0 perf stat -e r501fc7:u,r5100c8:u,cs:u ./binaries/retired_instr.int.x86_64 &> results/server/$1/run.$i.sse_retired.int.counts && \
cat /proc/interrupts > results/server/$1/run.$i.sse_retired.int.after ;

cat /proc/interrupts > results/server/$1/run.$i.sse_retired.fp.before && \
taskset -c 0 perf stat -e r501fc7:u,r5100c8:u,cs:u ./binaries/retired_instr.fp.x86_64 &> results/server/$1/run.$i.sse_retired.fp.counts && \
cat /proc/interrupts > results/server/$1/run.$i.sse_retired.fp.after ;

cat /proc/interrupts > results/server/$1/run.$i.sse_retired.sse.before && \
taskset -c 0 perf stat -e r501fc7:u,r5100c8:u,cs:u ./binaries/retired_instr.sse.x86_64 &> results/server/$1/run.$i.sse_retired.sse.counts && \
cat /proc/interrupts > results/server/$1/run.$i.sse_retired.sse.after ;

cat /proc/interrupts > results/server/$1/run.$i.sse_retired.all.before && \
taskset -c 0 perf stat -e r501fc7:u,r5100c8:u,cs:u ./binaries/retired_instr.x86_64 &> results/server/$1/run.$i.sse_retired.all.counts && \
cat /proc/interrupts > results/server/$1/run.$i.sse_retired.all.after ;

done
