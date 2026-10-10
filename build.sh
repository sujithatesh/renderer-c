#!/bin/bash

code="$PWD"
opts="-g -mavx2 -lwayland-client"
cd . > /dev/null
clang $opts $code/wayland.c $code/xdg-shell-protocol.c -o bar
cd $code > /dev/null
