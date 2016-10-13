#!/usr/bin/env zsh

for lib in $(ls lib*.so.*.*.*); do
    linked_lib=$(echo ${lib} | awk -F "." '{print $1"."$2"."$3"."$4}')
    ln -s $lib $linked_lib
done
