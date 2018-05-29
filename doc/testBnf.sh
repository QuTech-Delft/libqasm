./qtools/gsample < ./qasm.bnf | tr -d '\n' | sed 's/" "//g' | sed 's/ "//g' | sed 's/"$//g' | sed 's/\\n/\n/g'
