#!/bin/bash

bin_source="/home/pi/pianoLight/update/pianoLight"
bin_destination="/home/pi/pianoLight/pianoLight"

chown pi:pi $bin_source
chmod +x $bin_source
cp -r $bin_source $bin_destination

