#!/bin/bash

# this script is deprecated for now





 echo "VeryMildDemented"
 for f in $(find dataset/train/VeryMildDemented/. -type f -name "*.jpg"); do convert $f ${f//jpg/png}; done

 echo "MildDemented"
 for f in $(find dataset/train/MildDemented/. -type f -name "*.jpg"); do convert $f ${f//jpg/png}; done

 echo "ModerateDemented"
 for f in $(find dataset/train/ModerateDemented/. -type f -name "*.jpg"); do convert $f ${f//jpg/png}; done

 echo "NonDemented"
 for f in $(find dataset/train/NonDemented/. -type f -name "*.jpg"); do convert $f ${f//jpg/png}; done

 rm dataset/train/NonDemented/*.jpg
 rm dataset/train/ModerateDemented/*.jpg
 rm dataset/train/MildDemented/*.jpg
 rm dataset/train/VeryMildDemented/*.jpg

echo "VeryMildDemented"
 for f in $(find dataset/test/VeryMildDemented/. -type f -name "*.jpg"); do convert $f ${f//jpg/png}; done

echo "MildDemented"
 for f in $(find dataset/test/MildDemented/. -type f -name "*.jpg"); do convert $f ${f//jpg/png}; done

echo "ModerateDemented"
 for f in $(find dataset/test/ModerateDemented/. -type f -name "*.jpg"); do convert $f ${f//jpg/png}; done

echo "NonDemented"
 for f in $(find dataset/test/NonDemented/. -type f -name "*.jpg"); do convert $f ${f//jpg/png}; done

rm dataset/test/NonDemented/*.jpg
 rm dataset/test/ModerateDemented/*.jpg
 rm dataset/test/MildDemented/*.jpg
rm dataset/test/VeryMildDemented/*.jpg

# magik formula
# for f in *\ *; do mv "$f" "${f// /_}"; done

