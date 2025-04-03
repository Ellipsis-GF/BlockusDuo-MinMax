SRC_DIR=src
BUILD_DIR=build
SRC=$(wildcard $(SRC_DIR)/*.c) # Récupère tous les fichiers .c dans src/
EXE=blokus.out # fichier exécutable

CC=gcc
CFLAGS:=-Wall -Wextra -MMD -Og -g $(sdl2-config --cflags) # options de compilation
LDFLAGS:=-lSDL2_image -lSDL2_ttf -lSDL2_gfx -lm -lSDL2  # options de l'éditeur de liens

OBJ=$(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC)) # Convertit src/*.c en build/*.o
DEP=$(OBJ:.o=.d) # Génère les fichiers de dépendance

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS) 

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ -c $< 

clean:
	rm -rf $(BUILD_DIR) core $(EXE) # supprime build/, l'exécutable et core dump

-include $(DEP) # inclut les fichiers de dépendance générés par -MMD