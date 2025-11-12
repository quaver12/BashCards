Small CLI-based flashcards to both practice writing C, and use for revision down the line.


*Linux*

Run the following whereever you'd like to store the files:
```
git clone https://github.com/quaver12/BashCards.git
cd BashCards
gcc -o bcards BashCards.c
mkdir decks
cd decks
pwd > decksavelocation
mkdir ~/.config/bashcards
mv decksavelocation ~/.config/bashcards/decksavelocation
cd ..
sudo mv bcards /usr/bin/
```

work in progress
