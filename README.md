# Master-1-PPN

Auteurs : Tom Budon, Thomas Roglin, Pacôme Rousselle, Maëlo Sanchez-Guenro

Ce dépôt git contient le code d'un réseau de neurones à convolutions.
Il est actuellement codé de manière séquentielle.


# SETUP

# Prepare the dataset

Download the dataset on Kaggle [here](https://www.kaggle.com/datasets/tourist55/alzheimers-dataset-4-class-of-images?resource=download)
and extract it at the root of the project in a folder named `dataset`

it should look like this :

```
apps/
data/
...
dataset/
  test/
    MildDemented/
    ModerateDemented/
    NonDemented/
    VeryMildDemented/
  train/
    MildDemented/
    ModerateDemented/
    NonDemented/
    VeryMildDemented/

```

now call the setup script with `$ bash scripts/setup.sh `


# Configuration file


