default : add addmuch

objectadd = add.o

objectaddmuch = addmuch.o common.o 
add : add.o
	cc -o add $(objectadd)
addmuch : $(objectaddmuch)
	cc -o addmuch $(objectaddmuch)

common.o : ../defs.h

add.o : ../defs.h

addmuch.o : ../defs.h


