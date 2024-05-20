#!/bin/sh

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

base_command="grep"
s21_command="./s21_grep"

# can be changed
flags="-e -i -l -n -c -v"

# can be changed
patterns="no yi fie div"

# can be changed
file_path_one="files/gistfile1.txt"
file_path_two="files/yippee.txt"

# can be changed
files="$file_path_one $file_path_two"

every_check=0
approved=0
wrong=0

app_name="s21_grep"

if [ -e "$app_name" ]; then

    log1="log1.txt"
    log2="log2.txt"
    if [ -e "$log1" ]; then
        echo "$(rm $log1)"
    fi
    if [ -e "$log2" ]; then
        echo "$(rm $log2)"
    fi
    
    echo "MAY NOT BE THE SAME FOR DIFFERENT OPERATING SYSTEMS"
    
    for file in $files; do

        for pattern in $patterns; do

            # debug line, can be commented out
            echo "Parameters for the test are: $pattern $file"
        
            current_base_output=$($base_command $pattern $file)
            s21_base_output=$($s21_command $pattern $file)

            if [ "$current_base_output" = "$s21_base_output" ]; then
                echo -e "${GREEN}MATCHES${NC}"
                approved=$((approved + 1))
            else

                echo $current_base_output > "$log2"
                echo $s21_base_output > "$log1"

                if cmp -s "$log1" "$log2"; then
                    echo -e "${GREEN}MATCHES${NC}"
                    approved=$((approved + 1))
                    echo "$(rm $log1 $log2)"
                else
                    echo -e "${RED}DO NOT MATCH${NC}"
                    wrong=$((wrong + 1))
                fi

            fi

            every_check=$((every_check + 1))

        done

    done

    if [ -e "$log1" ]; then
        echo "$(rm $log1)"
    fi
    if [ -e "$log2" ]; then
        echo "$(rm $log2)"
    fi

    echo "Tests are done"
    echo "Amount of tests: $every_check"
    echo "           GOOD: $approved"
    echo "            BAD: $wrong"
else
    echo "Cannot do checks because there is no s21_grep"
fi