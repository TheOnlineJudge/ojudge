#!/bin/bash

build/bin/ojudgeapp --approot approot --docroot "docroot;/favicon.ico,/css,/resources,/images" --resources-dir /usr/share/Wt/resources --http-address 0.0.0.0 --http-port 9090
