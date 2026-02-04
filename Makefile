CC = gcc

build:

	gcc -o bcards src/main.c src/bcdeck.c src/bcutil.c src/help.c
	cd decks
	pwd > decksavelocation
	mkdir -v ~/.config/bashcards
	mv -v decksavelocation ~/.config/bashcards/decksavelocation
	cd ..

	gzip -v -k docs/bcards.1
	echo "done!"


install: bcards docs/bcards.1.gz
	sudo cp -v docs/bcards.1.gz /usr/share/man/man1/bcards.1.gz
	sudo mv -v bcards /usr/bin/

	#once installed make tutorial & deckformatguide decks
	deckSaveLoc=$(bcards -f)
	bcards -d > "$deckSaveLoc/DECKFORMATGUIDE.txt"
	bcards -t > "$deckSaveLoc/TUTORIAL.txt"



uninstall:

clean:
