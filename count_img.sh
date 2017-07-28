#!/bin/bash

for((i=0;i<21;i++))
do
		dst=./img/img$i/
		ls -l $dst | wc -l
done
