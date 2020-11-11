# PRODeep
PRODeep: A Platform for Robustness Verification of Deep Neural Networks
## PRODeep dependency
### dependency for ubuntu
	1) sudo apt-get install g++ qt5-default make
	2）sudo apt-get install libboost-all-dev
	3）sudo apt-get install -yqq libgmp-dev libmpfr-dev
	4）git clone https://github.com/antoinemine/apron 
	   	cd apron
	   	./configure -no-ppl -no-java
       	sudo make 
       	sudo make install

## Compile PRODeep
	1) cd \path\to\PRODeep
	2) qmake
	3) make
