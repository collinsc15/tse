#!/bin/bash

x=1
score=0

if [ ! -e vfile.txt ] ; then
		touch vfile.txt
fi

while [ ${x} -lt 2 ] ; do
		valgrind --leak-check=full --log-file=vfile.txt test${x}
		if [ $(cat vfile.txt | grep -c "in use at exit: 0 bytes in 0 blocks") -gt 0 ] ; then
				if [ $(cat vfile.txt | grep -c "ERROR SUMMARY: 0 errors from 0 contexts") -gt 0 ] ; then
						let score=$score+1
				fi
		fi
		let x=$x+1
done

if [ $score -eq 1 ] ; then
		echo "no leaks or errors"
fi

