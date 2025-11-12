Small CLI-based flashcards to both practice writing C, and use for revision down the line.


*Linux*

Run the following whereever you'd like to store the files:
```
git clone https://github.com/quaver12/BashCards.git
cd BashCards
gcc -o bcards BashCards.c
cd decks
pwd > decksavelocation
cd ..
mkdir ~/.config/bashcards
mv decksavelocation ~/.config/bashcards/decksavelocation
sudo cp bcards /usr/bin/
```

work in progress
