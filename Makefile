SRC=cat.c blockcat randblockcat reordercat reverse

DEPENDIO=io.o test_utils.o
DEPENDSTDIO=stdio-io.o test_utils.o

EXE=$(SRC:%.c=%)
EXESTDIO=$(patsubst %,stdio-%,$(EXE))

%.o : %.c
	gcc -c $+

$(EXE): % : %.o $(DEPENDIO)
	gcc -o $@ $+ 

$(EXESTDIO): stdio-% : $(DEPENDSTDIO) %.o
	gcc -o $@ $+

fadvise : fadvise.c
	gcc -o fadvise fadvise.c

files : ./files/text20mega.txt ./files/text1mega.txt ./files/text5mega.txt ./files/binary1mega.bin ./files/text5megar.txt ./files/text20megar.txt

./files/text20mega.txt :
	tr -dc '[:graph:]' < /dev/urandom | tr -d \''\\'\` | head -c 20M >  ./files/text20mega.txt
./files/text1mega.txt :
	tr -dc '[:graph:]' < /dev/urandom | tr -d \''\\'\` | head -c 1M >  ./files/text1mega.txt
./files/text5mega.txt :
	tr -dc '[:graph:]' < /dev/urandom | tr -d \''\\'\` | head -c 5M >  ./files/text5mega.txt
./files/binary1mega.bin :
	dd if=/dev/urandom of=./files/binary1mega.bin bs=1K count=1K
./files/text20megar.txt : ./files/text20mega.txt 
	./stdio-reverse -o ./files/text20megar.txt ./files/text20mega.txt
./files/text5megar.txt : ./files/text5mega.txt 
	./stdio-reverse -o ./files/text5megar.txt ./files/text5mega.txt



all : $(EXE) $(EXESTDIO) fadvise files

clean : 
	rm -f $(EXE) $(EXESTDIO) *.o
