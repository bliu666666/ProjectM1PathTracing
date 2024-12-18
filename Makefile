# Compilateur et options
CXX = g++
CXXFLAGS = -Wall -std=c++11

# Répertoires
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin
PPM_DIR = ppm_writer

# Fichiers source et objets
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
EXEC = $(BIN_DIR)/test_program

# Règle par défaut
all: dirs ppm_writer $(EXEC)
	./$(EXEC)

# Visualisation de l'image avec GIMP
view:
	gimp output.ppm

# Création des répertoires nécessaires
dirs:
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BIN_DIR)

# Compilation du ppm_writer
ppm_writer:
	$(MAKE) -C $(PPM_DIR)
	cp $(PPM_DIR)/ppm_writer.o $(BUILD_DIR)/

# Compilation du programme final
$(EXEC): $(OBJS)
	$(CXX) $(OBJS) $(BUILD_DIR)/ppm_writer.o -o $(EXEC) -lm

# Compilation des fichiers objets
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Nettoyage
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR) output.ppm
	$(MAKE) -C $(PPM_DIR) clean

# Recompilation complète
rebuild: clean all

.PHONY: all clean rebuild dirs ppm_writer view