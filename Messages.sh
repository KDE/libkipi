#! /bin/sh
#$EXTRACTRC `find . -name "*.rc" -o -name "*.ui" -o -name "*.kcfg"` > rc.cpp
LIST=`find . -name \*.cpp`
if test -n "$LIST"; then
	$XGETTEXT $LIST -o $podir/libkipi.pot
fi
