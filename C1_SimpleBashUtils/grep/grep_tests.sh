#!/bin/bash
array=(e i s f)

array2=(i v c l n h s o) #not all flags in pairs can be the first (flag e and flag f)

declare -a filenames=(
"1.txt"
"2.txt"
"3.txt"
"1.txt 2.txt"
"2.txt 3.txt"
"1.txt 2.txt 3.txt"
)

declare -a patterns=(
"hello"
"123"
"net"
"^1"
)

FAIL=0
SUCCESS=0

function test {
for j in "${filenames[@]}"
do
  for k in "${patterns[@]}"
  do
    if [[ $1 != *"f"* ]]
    then
      echo "diff <(./s21_grep -$1 $k $j) <(grep -$1 $k $j)"
      diff <(./s21_grep -$1 $k $j) <(grep -$1 $k $j) > log.txt 2>&1
 #   else
 #     echo "diff <(./s21_grep -$1 pat.txt $j) <(grep -$1 pat.txt $j)"
 #     diff <(./s21_grep -$1 pat.txt $j) <(grep -$1 pat.txt $j) > log.txt 2>&1
    fi
      error=$?
      if [ $error -eq 2 ] 
      then
          echo "There was something wrong with the diff command"
      elif [ $error -eq 1 ]
      then
          echo -e "\033[31mFAIL -$1 $k $j\033[0m"
          datetime=$(date +"%Y-%m-%d %H:%M")
          echo "$datetime FAIL -$1 $k $j" >> fail.txt
          (( FAIL++ ))
      else
          echo "SUCCESS -$1 $k $j"
          (( SUCCESS++ ))
      fi
  done
done
}

rm -rf fail.txt
touch log.txt
touch fail.txt
echo "One flag"
for i in "${array[@]}"
do
test $i
done

echo "Pair combination of flags"
for i in "${array2[@]}"
do
  for l in "${array[@]}"
  do
    if [ $i != $l ]
    then
      test $i$l
    fi
  done
done

rm -rf log.txt

echo "Total: $SUCCESS SUCCESS, $FAIL FAIL"
# echo "If in fail.txt there is a FAIL in -if combination it's OK because of difference of error messages."

if [[ $FAIL > 0 ]]; then exit 1
fi
