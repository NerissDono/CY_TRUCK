``# **CY-Truck _ COBB Leo - MEDDOUR Bissem - SMAIL Yuness**

## Outil Informatique de traitement de données

Cy-Truck est un outil gérant l’ensemble de la logistique d’une société nationale de transport routier. En incluant le fichier data.csv dans le répertoire qui lui est attribué, on peut récupérer les données nécessaires.

Une fois lancé, le programme guide l'utilisateur quant à son utilisation, mais vous avez toujours la rubrique **Lancement** pour vous aider.

Le programme permet de trier les données de data.csv afin de créer des graphiques via **GnuPlot** pour faciliter l'analyse des données.

## Compatibilité système

Le programme est compatible avec les dernières versions de **Ubuntu**.

## Technologies utilisées

- Langage de programmation : `C`, `Bash UNIX`
- Type de programmation : **modulaire**
- Logiciel utilisé : **Visual Studio Code**, **GnuPlot**

## Lancement

Pour utiliser le programme, ouvrez un terminal dans le dossier `CYTruck/`, assurez-vous de placer data.csv dans le répertoire CYTruck/data. 

Se placer dans la racine du dossier `CYTruck/`

Exécutez la commande `bash CYTruck.sh data/data.csv -h` pour découvrir les différents traitements disponibles.

## Téléchargement

- Téléchargez `./CYTruck.zip` vie Github et extrayez le fichier `./CYTruck`. Le fichier data.csv étant trop gros pour Github, n'oubliez pas de l'ajouter dans le répertoire assigné en le remplaçant par le fichier `data.csv` déjà présent.

# Règles

- Pour exécuter un traitement particulier, utilisez la commande `bash CYTruck.sh data/data.csv <option>`.
  - Exemple : Pour le traitement t, utilisez la commande `bash CYTruck.sh data/data.csv -t`.
 
- Il est impératif que le fichier data.csv soit rangé dans le repartoire /data pour le bon fonctionnement du programme.
``
