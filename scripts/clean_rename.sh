
for f in dataset/test/*/*; do mv "$f" "${f// /_}"; done
for f in dataset/train/*/*; do mv "$f" "${f// /_}"; done
