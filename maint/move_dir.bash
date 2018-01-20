#!/bin/bash
# Copyright (C) Oly project 2014

# initial setup
year="2014"
author="Jonathan Towne"
scriptname=$0
company=""
version="0.1"

description="Move files in a directory from one place to another. not recursive."

usage()
{
    echo -e "    $scriptname
    Copyright $year $author $company\n
    Usage: $scriptname [OLD DIR] [NEW DIR]
        -h | --help : display this help information and exit.
        -v | --version : display version information and exit.
" 
    exit
}

version()
{
     echo -e "    $scriptname
    $description
    version $version
    
    Copyright $year $author $company\n$licensenotice"
    exit
}

olddir=$1
newdir=$2

echo $olddir
echo $newdir

if [ $# -gt 0 ];
then
    while [ "$1" != "" ]; 
    do
        case $1 in
            -h | --help )
                usage
                ;;
            -v | --version )
                version
                ;;
            -*)
                usage
            ;;
        esac
        shift
    done
else
        usage
fi

for file in `ls ${olddir}/*.c` `ls ${olddir}/*.h`; 
do 
  fossil mv $file ${newdir?}
  mv $file ${newdir?}
done

