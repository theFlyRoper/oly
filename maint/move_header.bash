#!/bin/bash
# Copyright (C) Oly project 2014

# initial setup
year="2014"
author="Jonathan Towne"
scriptname=$0
company=""
version="0.1"

description=" "

usage()
{
    echo -e "    $scriptname
    Copyright $year $author $company\n
    Usage:
        -h | --help : display this help information and exit.
        -v | --version : display version information and exit.
        
        -o          : old header name.
        -n          : new header name.
        -d          : dir where you start.
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
            -o )
                shift
                if [[ "$1" == -* || -z "$1" ]] 
                then
                    echo "ERROR: Please specify the old header name with -o"
                    exit
                fi
                oldname="$1"
                ;;
            -n )
                shift
                if [[ "$1" == -* || -z "$1" ]] 
                then
                    echo "ERROR: Please specify the new name with -n"
                    exit
                fi
                newname="$1"
                ;;
            -d )
                shift
                if [[ "$1" == -* || -z "$1" ]] 
                then
                    echo "ERROR: Please specify the working directory with -d"
                    exit
                fi
                dirval="$1"
                ;;
            * )
                usage
            ;;
        esac
        shift
    done
else
        usage
fi

if [ -z "${dirval}" || -z "${oldname}" || -z "${newname}" ]
then
    echo "ERROR: -d, -o and -n are all required."
    exit
fi



for file in $( find $dirval -type f ); 
do 
  sed -i "$file" -e "s@${oldname}@${newname}@g"
done

