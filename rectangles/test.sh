#! /bin/bash

flag=1
pad=4
i=0
err=0

while [ $flag -eq 1 ]
do
  in=$(printf ./test/"%0*d" $pad $i)
  fi=$in"_in.txt"
  fo=$in"_out.txt"
  fm=$in"_my.txt"
  if [ -e "$fi" ]
    then
      ./rectangles < $fi > $fm;
      if ! cmp -s $fo $fm
        then
          printf "%s VS %s\n+----------------------------+\n" $fo $fm;
          cat $fi;
          printf "+----------------------------+\n";
          diff -y --suppress-common-lines $fo $fm;
          printf "+----------------------------+\nFin!\n\n"
          err=$((err+1))
      fi
      i=$((i+1))
    else 
      flag=0
      incount=$(ls ./test/*_in.txt -l | wc -l)
      printf "%d/%d input files tested, %d error(s) found!\n" $i $incount $err
  fi
done
