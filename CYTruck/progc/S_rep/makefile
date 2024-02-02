# Compilateur à utiliser
CC = gcc

# Options de compilation
CFLAGS = -Wall -Wextra -g

# Règle par défaut
all: traitement-s fin_traitement-s

# Règle pour l'exécutable traitement-s
traitement-s: filtrage_traitement-s.c
	$(CC) $(CFLAGS) -o $@ $^

# Règle pour l'exécutable fin_traitement-s
fin_traitement-s: triee_traitement-s.c
	$(CC) $(CFLAGS) -o $@ $^

# Règle pour nettoyer les fichiers générés
clean:
	rm -f traitement-s fin_traitement-s

.PHONY: all clean