#!/bin/bash

build/bin/ojudgeapp --docroot "Wt;/images,/fonts,/css,/templates,/favicon.ico" --resources-dir /usr/share/Wt/resources --http-address 0.0.0.0 --http-port 9090
