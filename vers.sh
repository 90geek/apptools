#!/bin/bash

t=`env LANG=en_US.UTF-8 date`
git=`git log -1|head -n 3|perl -e 'local $/;$a=<>;$a=~s/\n/ /g;print $a;'`
ost="apptools"
osr="V1.0.0"
cat >vers.h <<eof
#define VERS "${ost} (${platform}) ${osr}"
#define BLD  "${t}"
#define CMMT "${git}"
eof
