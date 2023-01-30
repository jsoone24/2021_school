#!/bin/bash
for i in '16kB' '32kB' '64kB'
do
	for j in '16kB' '32kB' '64kB'
	do
		for k in '64kB' '128kB' '256kB'
		do
			build/ARM/gem5.opt configs/lab7/complex_opts.py --binary='/home/gem5/tests/test-progs/mibench/security/blowfish/bf' --input='d /home/gem5/tests/test-progs/mibench/security/blowfish/output_small.enc /home/gem5/tests/test-progs/mibench/security/blowfish/output_small.asc 1234567890abcdeffedcba0987654321' --cpu_clock='1GHz' --l1i_size="$i" --l1d_size="$j" --l2_size="$k" 

			echo "l1i_size: $i, l1d_size: $j, l2_size: $k" >> result/blowfishdec.txt
			cat m5out/stats.txt | grep "simTicks\|system.cpu.dcache.overallMissRate::total\|system.cpu.icache.overallMissRate::total\|system.l2cache.overallMissRate::total" >> result/blowfishdec.txt
			
		done
	done
done