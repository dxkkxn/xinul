# Docker image

Cette image Docker permet d'avoir un environnement stable dans le but de pouvoir compiler et simuller notre OS sur l'architecture RISC-V.
## Dépendences

* Programme Docker;
* Pour pouvoir clonner les dépôts du projet il est nécessaire d'ajouter la partie privé de la clef RSA utilisée dans votre compte Gitlab de l'école. La clef de doit pas être chiffrée;

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



# Emplacement des fichiers

* /riscv-qemu contient les sources du projet RISCV-QEMU. Les fichier objets sont dans /riscv-qemu/build/;
* /sle/ contient les dépôt du projet hébergés sur Gitlab;
