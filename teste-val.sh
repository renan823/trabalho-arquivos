#!/bin/bash

# Clear previous log
rm -f valgrind.log

# Rebuild
make clean
make all

# Run all commands under Valgrind, appending to valgrind.log
{
    # 1. Create binary
    valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./trabalho <<< "1 teste.csv teste.bin"

    # 2. Print table
    valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./trabalho <<< "2 teste.bin"

    # 3. Remove India
    valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./trabalho <<< "4 teste.bin 1
2 idAttack 3 country \"INDIA\""

    # 4. Print table
    valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./trabalho <<< "2 teste.bin"

    # 5. Print Germany
    valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./trabalho <<< "3 teste.bin 1
1 country \"GERMANY\""

    # 6. Add two entries
    valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./trabalho <<< "5 teste.bin 2
5889 2017 82.93 \"GERMANY\" \"PHISHING\" NULO \"VPN\"
6845 NULO 13.13 \"FRANCE\" \"SQL INJECTION\" NULO \"ANTIVIRUS\""

    # 7. Print table
    valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./trabalho <<< "2 teste.bin"

    # 8. Remove China
    valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./trabalho <<< "4 teste.bin 1
1 country \"CHINA\""

    # 9. Print table
    valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./trabalho <<< "2 teste.bin"

    # 10. Modify entries
    valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./trabalho <<< "6 teste.bin 2
1 country \"GERMANY\"
1 country \"BRAZILI\"
1 idAttack 5889
1 country \"FORTALEZA\""

    # 11. Final print
    valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./trabalho <<< "2 teste.bin"
} >> valgrind.log 2>&1  # Append all output (stdout + stderr)

grep -A 5 "LEAK SUMMARY" valgrind.log