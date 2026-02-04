CC = gcc

BUILD_DIR := ./build
SRC_DIR := ./src

build: $(BUILD_DIR)/%.o docs/bcards.1

	$(CC) $(BUILD_DIR)/bcards -o $<
	echo "$(pwd)/decks" > decksavelocation
	mkdir -v ~/.config/bashcards
	mv -v decks/decksavelocation ~/.config/bashcards/decksavelocation

	gzip -v -k docs/bcards.1
	echo "done!"

install: bcards docs/bcards.1.gz
	sudo cp -v docs/bcards.1.gz /usr/share/man/man1/bcards.1.gz
	sudo mv -v $(BUILD_DIR)/bcards /usr/bin/

	#once installed make tutorial & deckformatguide decks
	deckSaveLoc=$(bcards -f)
	bcards -d > "$deckSaveLoc/DECKFORMATGUIDE.txt"
	bcards -t > "$deckSaveLoc/TUTORIAL.txt"

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -o $@ -c $(SRC_DIR)/%.c

uninstall:

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)
