#!/bin/bash

function make_test {
	tries="1 2 3 4 5"
	echo -e "\nCOMMAND ./$1 $2"
	echo -n "STDIO "
	t1=0;
	ok=1;
	for try in $tries;
	do
		$4 > /dev/null
		timeout 10 ./stdio-$1 $2
		if [[ $? -ne 0 ]]; then
			echo -e "\033[31mKILLED (timeout after 10s)\033[00m"
			ok=0
			break
		fi

		t=$(cat .testutils)
		t1=$(echo $t+$t1|bc -l)
	done
	echo  $t1 
	echo -n "YOUR CODE "
	t2=0
	ok=1
	for try in $tries;
	do
		$4 >/dev/null
		#t2=$(timeout 10 ./$1 $2)
		timeout 10 ./$1 $2
		if [[ $? -ne 0 ]]; then
			echo -e "\033[31mKILLED (timeout after 10s)\033[00m"
			ok=0
			break
		fi
		t=$(cat .testutils)
		t2=$(echo $t+$t2|bc -l)
	done
	if [[ $ok -ne 0 ]];then
		echo "$t2"
		echo -n "RATIO "
		ratio=$(bc -l <<< "$t1/$t2")
		win=$(bc -l <<< "$ratio < 1")
		if [[ $win -eq 1 ]]; then 

			LANG=C printf "\033[31m%.2f" $ratio	
			echo -e "(stdio)\033[00m"
		else 
			LANG=C printf "\033[32m%.2f" $ratio	
			echo -e "(your code)\033[00m"
		fi
	fi
	if [[ $ok -ne 1 ]];then
		return
	fi
	$3 > /dev/null
	if [[ $? -ne 0 ]]; then
		echo -e "\033[31mFiles differ\033[00m"
	fi
}

### cat ###

args_cat=( 
	"-o files/out.txt files/text1mega.txt"
	"-o files/out.bin files/binary1mega.bin"
	"-o files/out.txt files/text20mega.txt"
)

post_conditions=(
	"diff -q files/out.txt files/text1mega.txt"
	"diff -q files/out.bin files/binary1mega.bin"
	"diff -q files/out.txt files/text20mega.txt"
)
pre_conditions=(
	"./fadvise files/text1mega.txt POSIX_FADV_DONTNEED"
	"./fadvise files/binary1mega.bin POSIX_FADV_DONTNEED"
	"./fadvise files/text20mega.txt POSIX_FADV_DONTNEED"
)
i=0
for args in "${args_cat[@]}";
do
	make_test "cat" "$args" "${post_conditions[$i]}" "${pre_conditions[$i]}"
	i=$((i+1))
done

### blockcat ###

args_blockcat=( 
	"-o files/out.txt files/text1mega.txt"
	"-o files/out.bin files/binary1mega.bin"
	"-o files/out.txt files/text20mega.txt"
	"-b 509 -o files/out.txt files/text5mega.txt"
)

pre_conditions=(
	"./fadvise files/text1mega.txt POSIX_FADV_DONTNEED"
	"./fadvise files/binary1mega.bin POSIX_FADV_DONTNEED"
	"./fadvise files/text20mega.txt POSIX_FADV_DONTNEED"
	"./fadvise files/text5mega.txt POSIX_FADV_DONTNEED"
)


post_conditions=(
	"diff -q files/out.txt files/text1mega.txt"
	"diff -q files/out.bin files/binary1mega.bin"
	"diff -q files/out.txt files/text20mega.txt"
	"diff -q files/out.txt files/text5mega.txt"
)
i=0
for args in "${args_blockcat[@]}";
do
	make_test "blockcat" "$args" "${post_conditions[$i]}" "${pre_conditions[$i]}"
	i=$(($i+1))
done

### randblockcat ###

args_randblockcat=( 
	"-o files/out.txt files/text20mega.txt"
	"-r 6582 -o files/out.txt files/text20mega.txt"
)

pre_conditions=(
	"./fadvise files/text20mega.txt POSIX_FADV_DONTNEED"
	"./fadvise files/text20mega.txt POSIX_FADV_DONTNEED"
)


post_conditions=(
	"diff -q files/out.txt files/text20mega.txt"
	"diff -q files/out.txt files/text20mega.txt"
)
i=0
for args in "${args_randblockcat[@]}";
do
	make_test "randblockcat" "$args" "${post_conditions[$i]}" "${pre_conditions[$i]}"
	i=$((i+1))
done

### reverse ###

args_reverse=( 
	"-o files/out.txt files/text5mega.txt"
	"-o files/out.txt files/text20mega.txt"
)

pre_conditions=(
	"./fadvise files/text5mega.txt POSIX_FADV_DONTNEED"
	"./fadvise files/text20mega.txt POSIX_FADV_DONTNEED"
)


post_conditions=(
	"diff -q files/out.txt files/text5megar.txt"
	"diff -q files/out.txt files/text20megar.txt"
)
i=0
for args in "${args_reverse[@]}";
do
	make_test "reverse" "$args" "${post_conditions[$i]}" "${pre_conditions[$i]}"
	i=$((i+1))
done

### reordercat ###

args_reordercat=( 
	"-o files/out.txt files/text20mega.txt"
	"-r 6582 -o files/out.txt files/text20mega.txt"
)

pre_conditions=(
	"./fadvise files/text20mega.txt POSIX_FADV_DONTNEED"
	"./fadvise files/text20mega.txt POSIX_FADV_DONTNEED"
)


post_conditions=(
	"diff -q files/out.txt files/text20mega.txt"
	"diff -q files/out.txt files/text20mega.txt"
)
i=0
for args in "${args_reordercat[@]}";
do
	make_test "reordercat" "$args" "${post_conditions[$i]}" "${pre_conditions[$i]}"
	i=$((i+1))
done



