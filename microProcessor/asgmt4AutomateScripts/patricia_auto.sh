#!/bin/bash
for i in '16kB' '32kB' '64kB'
do
	for j in '16kB' '32kB' '64kB'
	do
		for k in '64kB' '128kB' '256kB'
		do
			build/ARM/gem5.opt configs/lab7/complex_opts.py --binary='/home/gem5/tests/test-progs/mibench/network/patricia/patricia' --input='/home/gem5/tests/test-progs/mibench/network/patricia/small.udp > /home/gem5/tests/test-progs/mibench/network/patricia/output_small.txt' --cpu_clock='1GHz' --l1i_size="$i" --l1d_size="$j" --l2_size="$k"

			echo "l1i_size: $i, l1d_size: $j, l2_size: $k" >> result/patricia.txt
			cat m5out/stats.txt | grep "simTicks\|system.cpu.dcache.overallMissRate::total\|system.cpu.icache.overallMissRate::total\|system.l2cache.overallMissRate::total" >> result/patricia.txt
			
		done
	done
done