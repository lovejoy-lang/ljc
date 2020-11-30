#!/bin/sh
make clean
make html
cd ./_build/html || exit 1
python3 -m http.server
