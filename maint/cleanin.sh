#!/bin/bash



for dirola in tests src replace;
do
  for file in `find $dirola -name *.c -o -name *.h`;
  do
    sed -i "$file" -e 's/UFILE/OFILE/g'
  done
done

for file in `ls src/wrappers`; do \fossil mv src/wrappers/$file src/core/$file; mv src/wrappers/$file src/core/$file; done
