#!/bin/bash

npages=`tiffinfo $1 | grep -c "Page Number"`

#launch an instance of tesseract per page
t0=`date +%s%3N`
for (( i=0;i<$npages;i++ )); do
		tessapi-quality $1 $i &
done

wait
t1=`date +%s%3N`

echo "elapsed time: " $((t1 - t0)) 
