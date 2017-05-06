#!/bin/bash
for i in `seq 1 12`;
do
	compile=$(printf './comp testFile%s' "$i")
	eval "$compile"

	case $i in
		1 ) echo '--------Print: 1-----------';;
		2 ) echo '--------Read: Echo--------';;
		3 ) echo '--------Assignment: 1--------';;
		4 ) echo '--------Negation: Neg Echo--------';;
		5 ) echo '--------Block Seq: Echo--------';;
		6 ) echo '--------Expression: 24 1 1--------';;
		7 ) echo '--------If Single: Echo If Positive Input--------';;
		8 ) echo '--------If Nested: Echo If Teen Input--------';;
		9 ) echo '--------Loop Single: Print Input Down to 0--------';;
		10 ) echo '--------Loop Nested:--------';;
		11 ) echo '--------Storage Global: Echo 3 Inputs Reversed--------';;
		12 ) echo '--------Storage Local: Echo 3 Inputs Reversed--------';;
	esac

	interpret=$(printf './VirtMach testFile%s.asm' "$i")	
	eval "$interpret"
	echo
done

