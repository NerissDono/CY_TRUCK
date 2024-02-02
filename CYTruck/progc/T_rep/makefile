# Nom de l'exécutable
EXE = T

# Compilateur à utiliser
CC = gcc

# Options de compilation
CFLAGS = -Wall -Wextra -g

# Fichiers source
SRC = T.c

# Fichiers objets générés à partir des fichiers source
OBJ = $(SRC:.c=.o)

# Règle par défaut
all: $(EXE)

# Règle pour l'exécutable
$(EXE): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Règle pour les fichiers objets
%.o: %.c header.h
	$(CC) $(CFLAGS) -c $< -o $@

# Règle pour nettoyer les fichiers générés
clean:
	rm -f $(EXE) $(OBJ)

.PHONY: all clean