# Projet_majeure

## Compilation

Pour compiler et executer à partir du CMakeLists.txt

```sh
mkdir build
cd build
cmake ..
make
cd ..
./build/programme_1
```

ou 

```sh
mkdir build
cmake . -B build
make -C ./build && ./build/programme_1
```

**Note sur l'utilisation des IDE (QtCreator, etc)**

Le repertoire d'execution doit être ici.
C'est a dire que le repertoire shaders/ et data/ doit être accessible.

**Important**

Il est nécessaire d'ajouter le dossier extern/glew au projet afin que le programme fonctionne.

## Commandes:

- Clic gauche souris + déplacement 
    => rotation de la caméra.
- Clic droit souris + déplacement 
    => zoom avant/arrière de la caméra.
- Shift + Clic gauche souris + déplacement 
    => Translation dans le plan de la caméra.
- Shift + Clic droit souris + déplacement 
    => Translation avant/arrière de la caméra.