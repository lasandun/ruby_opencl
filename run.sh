cd cl
make clean ; make
cd ..
make clean ; ruby extconf.rb ; make ; ruby example.rb
