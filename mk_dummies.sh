#
#  (c) 2019 po1ytopal
#  This project and all code contained is licensed under the MIT license (see LICENSE.txt for details)
#

###
## @file mk_dummies.sh
##
## @brief Bash script for generating test input.
##
## @author po1ytopal
##
##

#!/bin/bash

RANDOM=$(date +%s%N | cut -b10-19)

#FIXME: add leading / absolute path testing

USAGE="Usage: ${0} dir\nWhere 'dir' indicates desired output directory."

if [ $# -eq 1 ]
then
   if ! [ -d $1 ]
   then
      echo -e ${USAGE}
      exit 1;
   else
      for i in {0..99}
      do
         rnd1=$(( $RANDOM % 256 + 257 ))
         dd if=/dev/urandom bs=4 count=${rnd1} of=./${1}/dummy_`printf %02d ${i}`.bin >& /dev/null
      done
   fi
else
   echo -e ${USAGE}
   exit 1;
fi

