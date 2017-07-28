#!/bin/bash

for((i=0;i<21;i++))
do
		dst=./test$i.avi
		cp ./test.avi $dst
done
