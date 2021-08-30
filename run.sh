#!/bin/bash

BUFFER_SIZE=1024
FLAG="-Wall -Werror -Wextra"
DEFINE="-D BUFFER_SIZE=$BUFFER_SIZE"
GREEN='\033[0;32m'
RED='\033[0;31m'
NO='\033[0m'
YELLOW='\033[1;33m'
I=1
SUCCESS=0
rm -rf result.txt

main ()
{
cat <<EOF
	#include <stdio.h>
	#include "get_next_line.h"
	int main(void)
	{
		int ret = 1;
		char *line;

		while (ret)
		{
			ret = get_next_line(&line);
			printf("%s\n", line);
			free(line);
		}
		return (0);
	}
EOF
}

next()
{
	printf "\n"
	((I++))
}

result ()
{
	diff  -y --suppress-common-line  output/tester output/tested >> output/test_$1
	rm output/tester output/tested
	if [ "$?" != 0 ]; then
		printf "$RED[KO]$NO"
	else
		printf "$GREEN[OK]$NO"
		rm output/test_$1
		((SUCCESS++))
	fi;
	cat valgrind.log | grep "ERROR SUMMARY: 0" > /dev/null
	if [ "$?" != 0 ]; then
		printf "$YELLOW [LEAK]$NO"
	fi;
	rm -rf valgrind.log
	next
}

cd .. && cat get_next_line.c > /dev/null
if [ "$?" != 0 ]; then
	echo "get_next_line.c not found in parent folder...."
	exit 1
fi;

cd gnl_tester

main > main.c
cp main.c ../main.c
# TESTED GNL
gcc ../get_next_line.c ../main.c $FLAG $DEFINE -o tested
if [ "$?" != 0 ]; then
	printf "${RED}Compilation fail....\n"
	exit 1
fi;
# TESTER GNL
cp main.c src/main.c
gcc src/*.c  $FLAG $DEFINE -o tester

rm -rf ../main.c
rm -rf output_tested
rm -rf output_tester
rm -rf src/main.c

mkdir output 2> /dev/null


for i in $(ls test)
do
	printf "TEST$I | $(echo $i | tr '_' ' ') : "
	valgrind --leak-check=full --log-file="valgrind.log" ./tested < test/$i | cat -e > output/tested
	./tester < test/$i | cat -e > output/tester
	result $i
done

((I--))
printf "\n${SUCCESS}/${I} test\n"
if [ "$SUCCESS" = "$I" ]; then
	printf "${GREEN}All test passed !\n"
else
	print "${RED}You fail...\n"
fi;

rm -rf main.c
rm -rf tested
rm -rf tester