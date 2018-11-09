# Docker

L'image Docker attaché au projet OSON permet d'avoir un environnement stable dans le but de pouvoir compiler facilement et simuler notre OS sur l'architecture RISC-V.
Elle devrait également servir dans un second temps pour réaliser des tests automatiques à travers le pipeline de Gitlab. Avec cette solution il sera possible de vérifier la non régression à chaque commit.

Cette image permet avant tout une compilation croisée et un lancement rapide de l'OS sans altérer le système invité.

## Dépendences

* Programme Docker;
* Très optionel : Pour pouvoir clonner les dépôts du projet il est nécessaire d'ajouter la partie privé de la clef RSA utilisée dans votre compte Gitlab de l'école. La clef de doit pas être chiffrée;

## Les images
### riscv 

Cette image est basée sur Debian testing et contient :

* ridcv-gnu-toolchain
* riscv-pk
* riscv-spike
* riscv-fesvr
* riscv-qemu

### riscv-gitlab-src

En plus de l'image riscv celle-ci télécharge en plus les sources du projet sur gitlab.

## Utilisation de l'image Docker avec le make

$ make … 

* build : construit l'image Docker depuis le fichier Dockerfile. Va télécharger Debian testing puis clonner et compiler les sources.
* dl ou pull : télécharge l'image depuis internet (2Go) mais pas besoind de compiler.
* bash : lance un bash dans un container Docker.

Dans le dossier src le Makefile contient également ces target dans le but de compiler, télécharger et lancer la compilation du projet à partir d'un container Docker.

# Manipulation à la main
## Création de l'image

Ce placer dans le dossier docker puis:

    docker image build -t riscv .
    
Vous pouvez aller boire un café!

## Lancement du conteneur

Pour lancer le conteneur Docker:

    docker container run -ti --name env-riscv riscv /bin/b        


## Clean

Supprimer le conteneur:

    docker container rm env-riscv

Supprimer l'image:

    docker image rm riscv

