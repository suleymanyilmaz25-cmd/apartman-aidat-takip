# Apartman Aidat Takip Sistemi - Makefile

CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11 `pkg-config --cflags gtk+-3.0 sqlite3`
LDFLAGS = `pkg-config --libs gtk+-3.0 sqlite3` -lm

# Dizinler
SRC_DIR = src
INCLUDE_DIR = include
OBJ_DIR = obj
BIN_DIR = bin
DATA_DIR = data

# Dosyalar
SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SOURCES))
TARGET = $(BIN_DIR)/apartman-aidat-takip

# Hedefler
all: directories $(TARGET)

directories:
	@mkdir -p $(OBJ_DIR) $(BIN_DIR) $(DATA_DIR)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)
	@echo "✅ Build başarılı: $@"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

install-deps:
	@echo "GTK+ 3 ve SQLite3 kütüphaneleri yükleniyor..."
	@command -v apt-get >/dev/null 2>&1 && \
		sudo apt-get update && \
		sudo apt-get install -y build-essential libgtk-3-dev libsqlite3-dev || \
		echo "Lütfen GTK+ 3 ve SQLite3 geliştirme paketlerini yükleyin."

clean:
	@rm -rf $(OBJ_DIR) $(BIN_DIR)
	@echo "✨ Derleme dosyaları temizlendi."

distclean: clean
	@rm -rf $(DATA_DIR)/*.db
	@echo "🗑️  Tüm dosyalar temizlendi (veritabanı dahil)."

.PHONY: all run install-deps clean distclean directories
