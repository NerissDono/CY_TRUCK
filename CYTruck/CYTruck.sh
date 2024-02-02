#!/bin/bash

if [ "$1" != "data/data.csv" ]; then          # Le fichier data.csv doit obligatoirement être le 1er argument
    echo "Usage: 'bash $0 data/data.csv <options>'
    Utilisez l'option -h pour avoir la liste des options et leur rôle.
    Assurez vous d'avoir placé le fichier data.csv dans le répertoire CYTruck/data"
    exit 1
fi

if [ $# -lt 2 ]; then               # Si il n'y a pas d'argument d'option, il faut en ajouter au moins une
    echo "Veuillez entrer au moins une option. Utilisez -h pour afficher la liste des options et leur rôle."
    exit 2
fi

for arg in "${@:2}" ; do

    if [ $arg == "-h" ]; then           # OPTION -h (aide)
        echo "Options : 
        -d1 : crée un histogramme des conducteurs ayant effectué le plus de trajets.
        -d2 : crée un histogramme des conducteurs ayant parcouru la plus grande distance totale.
        -l : crée un histogramme des 10 trajets les plus longs en distance.
        -t : crée un histogramme des 10 villes les plus traversées parmi tous les trajets, 
        indiquant le nombre de fois qu'elles ont été au départ d'un trajet.
        -s : crée une courbe min-max-moyenne des distances parcourues par étapes pour chaque trajet.
        "
        exit 0
    fi
done 

if [ -d "temp" ]; then
    # Si le dossier 'temp' existe, le vider
    rm -r temp/*
else
    # Si le dossier 'temp' n'existe pas, le créer
    mkdir temp
fi

if [ ! -d "img" ]; then
    # Si le dossier 'img' n'existe pas, le créer
    mkdir img
fi

if [ ! -f progc/T_rep/T ]; then
    echo -e "\nCompilation du programme C pour l'option -t\n"
    cd ./progc/T_rep
    make
    cd ../..
fi

if [ ! -f progc/S_rep/traitement-s ] || [ ! -f progc/S_rep/fin_traitement-s ]; then
    echo -e "\nCompilation du programme C pour l'option -s\n"
    cd ./progc/S_rep
    make
    cd ../..
fi
    
for arg in "${@:2}" ; do    
    installer_imagemagick(){
    echo "Installation d'ImageMagick en cours..."
    sudo apt-get install imagemagick
    }

    # Vérifier si ImageMagick est installé
    if ! command -v convert &> /dev/null; then
        echo "ImageMagick n'est pas installé. L'installation sera effectuée."
        installer_imagemagick
    fi
    
    # Option -d1
    if [ $arg == "-d1" ]; then

        debut=$(date +%s) # variable pour le calcul du temps d'éxecution du programme

        # Utilisation d'awk pour compter les occurrences de la colonne 6
        # et stocker les résultats dans un tableau associatif (tab)
        # La condition /;1;/ filtre les lignes contenant le numéro 1 dans n'importe quelle colonne
        awk -F ";" '/;1;/ {
            tab[$6] += 1
        } END {
            # Parcours du tableau et impression des résultats
            for(cle in tab) {
                print cle ";" tab[cle]
            }
        }' $1 | sort -t ';' -k2 -nr | head -10 > data_d1.dat

        echo -e "\n$(date)\n" > temp_d1.dat
        cat data_d1.dat >> temp_d1.dat
        cat temp_d1.dat >> ./demo/data_d1.dat
        mv temp_d1.dat ./temp

        # Enregistrement du temps de fin
        fin=$(date +%s)

        # Calcul de la durée totale d'exécution
        temps_exec=$((fin - debut))

        # Affichage du temps d'exécution
        echo -e "\nDonnées du traitement $arg enregistrées dans CYTruck/demo/data_d1.dat"
        echo "Temps d'éxecution de l'option -d1: $temps_exec secondes."

        # Code Gnuplot
        gnuplot <<EOF
        set terminal pngcairo
        set output "img_d1.png"

        set bmargin 9.2
        set rmargin 8.4
        set lmargin 3
        set xlabel 'CONDUCTEURS' rotate by 180 offset 0,-6.2 font ",9"  # Modifiez la taille de la police ici
        set ylabel 'NB ROUTES'
        set xtic rotate by 90 offset 0,-6.7 font ",9"  # Modifiez la taille de la police ici
        set ylabel offset 65,0
        set ytic offset 55,0
        set yrange [0:250]
        set ytic rotate by 90
        set style histogram rowstacked
        set style fill solid border 1
        set boxwidth 0.6  # Vous pouvez ajuster cette valeur selon vos préférences
        unset key
        set label rotate by 90 'Option -d1 : NB ROUTES = f(Driver)' at screen 0.015, 0.5 center offset 0.1
        set datafile separator ";"

        plot "data_d1.dat" using 2:xtic(1) with boxes lc rgb "green" notitle
EOF

        # Ouvrir l'image générée
        # Faire pivoter l'image de 90 degrés
        convert -rotate 90 img_d1.png img_d1_rotated.png
        # Ouvrir l'image avec le visualiseur par défaut
        xdg-open img_d1_rotated.png

        mv img_d1_rotated.png ./img/imgd_1.png && rm img_d1.png
        rm data_d1.dat
    fi

    # Option -d2
    if [ $arg == "-d2" ]; then

        debut=$(date +%s) # variable pour le calcul du temps d'éxecution du programme

        awk -F';' '{
          # Extraction des informations pertinentes du CSV
          conducteur = $6  # Récupération du nom du conducteur depuis la colonne 6
          distance = $5    # Récupération de la distance parcourue depuis la colonne 5
 
          # Accumulation des distances pour chaque conducteur
          total_distance[conducteur] += distance
        }
        END {
          # Parcours de la structure de données pour chaque conducteur
          for (conducteur in total_distance) {
   
            #Affichage du nom du conducteur et de la somme totale des distances parcourues
            print conducteur ";" total_distance[conducteur]
          }
        }' $1 | sort -t ';' -k2 -nr | head -10 > data_d2.dat

        echo -e "\n$(date)\n" >temp_d2.dat
        cat data_d2.dat >> temp_d2.dat
        cat temp_d2.dat >> ./demo/data_d2.dat
        mv temp_d2.dat ./temp

        fin=$(date +%s)

        temps_exec=$((fin - debut))

        echo -e "\nDonnées du traitement $arg enregistrées dans CYTruck/demo/data_d2.dat"
        echo "Temps d'éxecution de l'option -d2: $temps_exec secondes."
        
        gnuplot <<EOF
        set terminal pngcairo size 1000,800
        set output "img_d2.png"

        set bmargin 9.2
        set rmargin 8.4
        set lmargin 3
        set xlabel 'CONDUCTEURS' rotate by 180 offset 0,-6.2 font ",9"  # Modifiez la taille de la police ici
        set ylabel offset 9,-10 'DISTANCE (km)'
        set xtic rotate by 90 offset 0,-6.7 font ",9"  # Modifiez la taille de la police ici
        set ylabel offset 100 
        set ytic rotate by 90 offset 90
        set yrange [0:160000]  # Ajustez la plage y
        #set ytics (0, 20000, 60000, 100000, 120000, 160000)  # Ajustez les valeurs y
        # set ytic rotate by 90 font ",6"
        set style histogram rowstacked
        set style fill solid border 1
        set boxwidth 0.5  # Vous pouvez ajuster cette valeur selon vos préférences
        unset key
        set label rotate by 90 'Option -d2' at screen 0.015, 0.5 center offset 0.1
        set datafile separator ";"

        plot "data_d2.dat" using 2:xtic(1) with boxes lc rgb "green" notitle
EOF
     # Convertir l'image en la faisant pivoter de 90 degrés
        convert -rotate 90 img_d2.png img_d2_rotated.png

        # Ouvrir l'image avec le visualiseur par défaut
        xdg-open img_d2_rotated.png

        mv img_d2_rotated.png ./img/imgd_2.png && rm img_d2.png
        rm data_d2.dat
    fi


    # OPTION -t
    if [ $arg == "-t" ]; then

        debut=$(date +%s) # variable pour le calcul du temps d'éxecution du programme
        
        cut -d';' -f1,2,3,4 $1 | tr ' ' '_' | tail -n +2 | sort -t ';' -k1,1n -k2,2n > tmp_trajets.txt  # extraire les champs nécessaires au traitement, remplacer les espaces potentiels dans les noms des villes par des underscore puis trier par ID ROUTE et ID STEP, et stocker dans un fichier temporaire 
                                 
        awk -F ';' '{               
            if ($1 != prev) {
                if (NR != 1) print ""
                printf $1 " "
                delete seen
            }
            if (!seen[$3]++) printf $3 " "  
            if (!seen[$4]++) printf $4 " "
            prev = $1
        } END {print ""}' tmp_trajets.txt > routes.txt   # liste des villes traversées pour chaque ROUTE ID dans tmp_trajets.txt, en n'ajoutant que les villes que l'on n'a pas encore ajoutées dans la liste

        mv tmp_trajets.txt ./temp

        awk '{for (i=2; i<=NF; i++) count[$i]++} END {for (city in count) print city " " count[city]}' routes.txt > city_counts.txt

        awk '{count[$2]++} END {for (city in count) print city " " count[city]}' routes.txt > start_counts.txt

        mv routes.txt ./temp

        cd ./progc/T_rep
        chmod +x T
        ./T
        cd ../..

        mv city_counts.txt ./temp
        mv start_counts.txt ./temp

        head -n 10 traitement_t.txt > traitement_t_bis.txt
        tr ' ' ';' < traitement_t_bis.txt | tr '_' ' ' > data_t.dat
        mv traitement_t.txt ./temp
        mv traitement_t_bis.txt ./temp
        mv fusion.txt ./temp

        echo -e "\n$(date)\n" > temp_t.dat         # écriture de la date pour les résultats du traitement t dans un fichier situé dans /demo
        cat data_t.dat >> temp_t.dat
        cat temp_t.dat >> ./demo/data_t.dat
        mv temp_t.dat ./temp

        fin=$(date +%s)
        temps_exec=$((fin - debut))
        echo -e "\nDonnées du traitement $arg enregistrées CYTruck/demo/data_t.dat"
        echo "Temps d'exécution de l'option -t: $temps_exec secondes"
        
        # Code Gnuplot
        gnuplot << EOF
        set terminal pngcairo size 1200,800 enhanced font 'Arial,12'
        set output "img/img_t.png"

        # Définir les titres et les étiquettes
        set title 'Nombre de trajets par ville'
        set xlabel 'Villes'
        set ylabel 'Nombre de trajets'

        # Définir le style de l'histogramme
        set style data histogram
        set style histogram clustered gap 1
        set style fill solid border -1
        set boxwidth 0.9

        # Définir les paramètres des axes
        set xtics rotate by -45
        set yrange [0:*]

        # Définir le séparateur de données
        set datafile separator ";"

        # Tracer l'histogramme
        plot 'data_t.dat' using 2:xtic(1) title 'Nombre total de traversées', '' using 3 title 'Nombre de départ d’un trajet'

EOF

    # Ouvrir l'image avec le visualiseur par défaut
        xdg-open img/img_t.png
        rm data_t.dat
    fi

    # OPTION -l 
    if [ $arg == "-l" ]; then

        debut=$(date +%s) # variable pour le calcul du temps d'éxecution du programme

        awk -F ';' '{ sum[$1] += $5 } END { for (i in sum) print i, sum[i] }' $1 | sort -k2,2nr | head -n 10 | sort -k1,1nr > data_l.dat

        echo -e "\n$(date)\n" > temp_l.dat
        cat data_l.dat >> temp_l.dat
        cat temp_l.dat >> ./demo/data_l.dat
        mv temp_l.dat ./temp

        fin=$(date +%s)
        temps_exec=$((fin - debut))
        echo -e "\nDonnées du traitement $arg enregistrées dans CYTruck/demo/data_l.dat"
        echo "Temps d'exécution de l'option -l: $temps_exec secondes"

        gnuplot <<EOF
        set terminal pngcairo size 1200,800
        set output "img_l.png"

        set title 'Option -l' 
        set xlabel 'ROUTE ID'
        set ylabel 'DISTANCE (km)' 
        set yrange [0:3000]  # Ajustez la plage y
        set ytics (0, 500, 1000, 1500, 2000, 2500, 3000)
        set style histogram rowstacked
        set style fill solid border 1
        set boxwidth 0.5  # Vous pouvez ajuster cette valeur selon vos préférences
        unset key


        plot "data_l.dat" using 2:xtic(1) with boxes lc rgb "green" notitle
EOF

        # Ouvrir l'image avec le visualiseur par défaut
        xdg-open img_l.png
        mv img_l.png ./img
        rm data_l.dat

    fi

    if [ $arg == "-s" ]; then

        # Nom du fichier CSV
        fichier_csv="data/data.csv"

        # Enregistrement du début du temps d'exécution
        debut_temps_s=$(date +%s)

        # Utilisation de awk avec le fichier de données
        awk -F';' '{
          # Extraction des informations pertinentes du CSV
          conducteur = $1  # Récupération des ROUTE ID depuis la colonne 1
          distance = $5    # Récupération de la distance parcourue depuis la colonne 5
          print $1 ";" $5
        }' "$fichier_csv" > s.csv

        cd progc/S_rep
        chmod +x traitement-s fin_traitement-s
        ./traitement-s # Ca c'est pour le filtrage donc ca sort le fichier s-filtrer.csv
        ./fin_traitement-s # Ca c'est pour le tri donc ca va sortir le fichier s-final.csv
        cd ../..
        # Enregistrement du temps de fin
        fin_temps_s=$(date +%s)

        # Calcul de la durée totale d'exécution
        duree_traitement_s=$((fin_temps_s - debut_temps_s))

        # Affichage du temps d'exécution
        echo -e "\nLe temps d'exécution du traitement s est $duree_traitement_s secondes.\n"

        # Code Gnuplot
        gnuplot << EOF
          reset
          set terminal pngcairo size 1200,800 enhanced font "arial,12"
          set output "img_s.png"
          set title "TRAITEMENT-s"
          set xlabel "ROUTE ID"
          set ylabel "DISTANCE (Km)"  # Correction de la ligne manquante pour le ylabel
          set yrange [0:1000]
          set xtics rotate by 45 right 
          set datafile separator ';'
          plot 's-final.csv' using 1:3:5:xticlabel(2) with filledcurves linecolor 2 lt 1 title 'Distance (Km)', \
               '' using 1:4 with lines linecolor 0 title 'Distance moyenne (Km)' 
EOF



        # Ouvrir l'image avec le visualiseur par défaut
        xdg-open img_s.png
        echo -e "\n$(date)\n" > temp_s.dat
        cat s-final.csv > data_s.dat
        cat data_s.dat >> temp_s.dat
        cat temp_s.dat >> ./demo/data_s.dat
        mv *.csv ./temp
        mv *.dat ./temp
        mv img_s.png ./img

    fi

done