#!/bin/bash

echo "VeryMildDemented"
for f in $(find test/train/VeryMildDemented/. -type f -name "*.jpg"); do convert $f ${f//jpg/png}; done

echo "MildDemented"
for f in $(find test/train/MildDemented/. -type f -name "*.jpg"); do convert $f ${f//jpg/png}; done

echo "ModerateDemented"
for f in $(find test/train/ModerateDemented/. -type f -name "*.jpg"); do convert $f ${f//jpg/png}; done

echo "NonDemented"
for f in $(find test/train/NonDemented/. -type f -name "*.jpg"); do convert $f ${f//jpg/png}; done

rm test/train/NonDemented/*.jpg
rm test/train/ModerateDemented/*.jpg
rm test/train/MildDemented/*.jpg
rm test/train/VeryMildDemented/*.jpg
