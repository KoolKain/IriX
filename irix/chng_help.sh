sed s/" $1 "/" $2 "/g < help/main/$1 > help/main/$2
sed s/" $1 "/" $2 "/g < syntax/main/$1 > syntax/main/$2
rm -f help/main/$1
rm -f syntax/main/$1
if [ $3 ]
then 
    ls help/$3 |
    while read x
    do
      sed s/" $1 "/" $2 "/g < help/$3/$x > help/$3/tmp
      sed s/" $1 "/" $2 "/g < syntax/$3/$x > syntax/$3/tmp
      mv  help/$3/tmp  help/$3/$x
      mv  syntax/$3/tmp  syntax/$3/$x
    done
    mv help/$3 help/$4
    mv syntax/$3 syntax/$4
    rm -f syntax/$3/tmp help/$3/tmp
fi
