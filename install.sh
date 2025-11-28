gcc -o bcards BashCards.c
cd decks
pwd > decksavelocation
mkdir ~/.config/bashcards
mv decksavelocation ~/.config/bashcards/decksavelocation
cd ..

sudo cp bcards.1.gz /usr/share/man/man1/bcards.1.gz

sudo mv bcards /usr/bin/
