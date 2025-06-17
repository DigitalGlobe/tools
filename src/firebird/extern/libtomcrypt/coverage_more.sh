#!/bin/bash

set -e

./sizes
./constants

for i in $(for j in $(echo $(./hashsum -h | awk '/Algorithms/,EOF' | tail -n +2)); do echo $j; done | sort); do echo -n "$i: " && ./hashsum -a $i tests/test.key ; done > hashsum_tv.txt
difftroubles=$(diff -i -w -B hashsum_tv.txt notes/hashsum_tv.txt | grep '^<') || true
if [ -n "$difftroubles" ]; then
  echo "FAILURE: hashsum_tv.tx"
  diff -i -w -B hashsum_tv.txt notes/hashsum_tv.txt
  echo "hashsum failed"
  exit 1
else
  echo "hashsum okay"
fi


exit 0

# ref:         tag: v5.0.2
# git commit:  f6d531779d267b91f2a6037c82260ce6f6d10da8
# commit time: 2025-02-11 20:17:04 +0000
