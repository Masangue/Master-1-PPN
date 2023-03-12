#!/bin/sh


mkdir -p build
mkdir -p out
mkdir -p out/storage
mkdir -p out/img


bash scripts/clean_rename.sh
bash scripts/convert2png.sh

