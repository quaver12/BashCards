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
	sudo mv -v $(TARGET) /usr/bin/
	
	#move man files
	sudo cp -v docs/bcards.1.gz /usr/share/man/man1/bcards.1.gz

	#add decksavelocation to user .config
	mkdir -pv ~/.config/bashcards
	mv -vf $(BUILD_DIR)/decksavelocation ~/.config/bashcards/decksavelocation

	#once installed make tutorial & deckformatguide decks
	bcards -d > $(shell bcards -f)/DECKFORMATGUIDE.txt
	bcards -t > $(shell bcards -f)/TUTORIAL.txt
	@echo "install complete"

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(@D)
	$(CC) -o $@ -c $<

uninstall:
	#remove config files
	rm ~/.config/bashcards/decksavelocation
	#remove program files
	sudo rm /usr/bin/bcards
	#remove man files
	sudo rm /usr/share/man/man1/bcards.1.gz
	@echo "uninstall complete"

clean:
	rm -r $(BUILD_DIR)
	@echo "clean complete"




