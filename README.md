# PRODeep
PRODeep: A Platform for Robustness Verification of Deep Neural Networks
## PRODeep dependency
### dependency for ubuntu
	1）apt-get install libboost-all-dev
	2）apt-get install -yqq libgmp-dev libmpfr-dev
	3）git clone https://github.com/antoinemine/apron 
	   	cd apron
	   	./configure -no-ppl -no-java
       	sudo make 
       	sudo make install

### dependency for MACOS
	1)Install [Homebrew](https://brew.sh/)
	```
	/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install.sh)"
	```

	2)Install [Boost](https://www.boost.org/)

	```
	brew install boost
	```

	3)Compile Apron 

	```
	git clone https://github.com/antoinemine/apron.git
	cd ./apron
	./configure -no-ocaml -no-ocamlfind -no-ocaml-plugins -no-ppl -no-java 
	make
	make install
	```

### Qt environment
#### Ubuntu 18.04+
	sudo apt-get install qt5-default make
#### MACOS
	brew install qt make
