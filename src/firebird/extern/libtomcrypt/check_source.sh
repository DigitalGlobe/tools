#!/bin/bash

# output version
bash printinfo.sh

make clean > /dev/null

echo "checking..."
./helper.pl --check-source --check-makefiles --check-defines|| exit 1

exit 0

# ref:         tag: v5.0.2
# git commit:  f6d531779d267b91f2a6037c82260ce6f6d10da8
# commit time: 2025-02-11 20:17:04 +0000
