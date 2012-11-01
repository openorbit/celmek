#! /bin/sh

if [ $# == 1 ] ; then
  cd $1

  header=`ls header*`
  
  cat $header > merged.deascii
  for f in `ls ascm* | sort -r` ; do
    cat $f >> merged.deascii
    echo "$f done"
  done;
  for f in `ls ascp* | sort` ; do
    cat $f >> merged.deascii
    echo "$f done"
  done;
fi
