gcc -o bcards src/main.c src/bcdeck.c src/bcutil.c docs/help.c
cd decks
pwd > decksavelocation
mkdir -v ~/.config/bashcards
mv -v decksavelocation ~/.config/bashcards/decksavelocation
cd ..
gzip -v -k docs/bcards.1
sudo cp -v docs/bcards.1.gz /usr/share/man/man1/bcards.1.gz
sudo mv -v bcards /usr/bin/
echo "done!"
