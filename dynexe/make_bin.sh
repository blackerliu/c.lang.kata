#!/bin/bash
# extract a function from ELF and copy to a binary file
# by LiuJianhua

if [ -z $1 ]; then
	symbol_name="external_add"
else
	symbol_name=$1
fi

section_offset=$(readelf -S funlib.o | grep ".text" | awk -F" " '{print $6}')
symbol_offset=$(nm -S funlib.o | grep "${symbol_name}" | awk -F" " '{print $1}')
symbol_size=$(nm -S funlib.o | grep "${symbol_name}" | awk -F" " '{print $2}')

section_offset=$(printf "%d" 0x${section_offset})
symbol_offset=$(printf "%d" 0x${symbol_offset})
symbol_size=$(printf "%d" 0x${symbol_size})

file_offset=$(expr $section_offset + $symbol_offset )

dd if=funlib.o of=funcode.bin bs=1 skip=$file_offset count=$symbol_size 2> /dev/null
echo "0000" >> funcode.bin

echo "section: .text"
echo "section offset:"$section_offset
echo "symbol name :"$symbol_name
echo "symbol offset:"$symbol_offset
echo "symbol size:"$symbol_size
