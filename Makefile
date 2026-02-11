CC = gcc
CFLAGS = -Wall

#directories
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin
TARGET = $(BIN_DIR)/bcards

#locates all .c files in src for you
SOURCES := $(shell find $(SRC_DIR) -name '*.c')
OBJECTS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SOURCES))

.PHONY: clean install install-user uninstall

$(TARGET): $(OBJECTS) docs/bcards.1.gz $(BUILD_DIR)/decksavelocation
	#compile program
	mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET)
	@echo "build complete"


$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -o $@ -c $<


docs/bcards.1.gz: docs/bcards.1
	#prepare man files
	gzip -vf -k docs/bcards.1


$(BUILD_DIR)/decksavelocation:
	#prepare config file
	echo "$$PWD/decks" > $@



install:
	#move program to bin
	cp -v $(TARGET) /usr/bin/
	
	#move man files
	cp -v docs/bcards.1.gz /usr/share/man/man1/bcards.1.gz
	@echo "install complete, run 'make install-user' to install default user config"

install-user:
	#add decksavelocation to user .config
	mkdir -pv ~/.config/bashcards
	cp -vf $(BUILD_DIR)/decksavelocation ~/.config/bashcards/decksavelocation

	#once make tutorial & deckformatguide decks
	bcards -d > $$(bcards -f)/DECKFORMATGUIDE.txt
	bcards -t > $$(bcards -f)/TUTORIAL.txt
	@echo "installed default config setup for this user"

uninstall:
	#remove program files
	rm -f /usr/bin/bcards
	#remove man files
	rm -f /usr/share/man/man1/bcards.1.gz
	@echo "uninstall complete"

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)
	@echo "clean complete"




