gcc -o bcards BashCards.c
cd decks
pwd > decksavelocation
mkdir ~/.config/bashcards
mv decksavelocation ~/.config/bashcards/decksavelocation
cd ..
sudo mv bcards /usr/bin/
