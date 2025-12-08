gcc -o bcards src/main.c src/bcdeck.c src/bcutil.c
cd decks
pwd > decksavelocation
mkdir ~/.config/bashcards
mv decksavelocation ~/.config/bashcards/decksavelocation
cd ..
gzip -k docs/bcards.1
sudo cp docs/bcards.1.gz /usr/share/man/man1/bcards.1.gz

sudo mv bcards /usr/bin/
