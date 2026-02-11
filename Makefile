CC = gcc

#directories
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin
TARGET = $(BIN_DIR)/bcards

SOURCES := $(shell find $(SRC_DIR) -name '*.c')
OBJECTS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SOURCES))
PWD = $(shell pwd)

.PHONY: clean install uninstall

#bcards: build/main.o build/bcdeck.o build/bcutil.o build/help.o docs/bcards.1
$(TARGET): $(OBJECTS) docs/bcards.1
	#$(CC) -o bcards src/main.o src/bcdeck.o src/bcutil.o src/help.o

	mkdir -p $(BIN_DIR)
	$(CC) $(OBJECTS) -o $(TARGET)

	#prep config file
	echo $(PWD)/decks > $(BUILD_DIR)/decksavelocation

	#prep man files
	gzip -vf -k docs/bcards.1

	@echo "build complete"

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


$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(@D)
	$(CC) -o $@ -c $<

uninstall:
	#remove program files
	rm -f /usr/bin/bcards
	#remove man files
	rm -f /usr/share/man/man1/bcards.1.gz
	@echo "uninstall complete"

clean:
	rm -r $(BUILD_DIR)
	@echo "clean complete"




