objects = vmain.o volcontrol.o util.o

vol : $(objects)
	gcc -o volcontrol $(objects) -lasound

$(objects) : util.h 
vmain.o : common.h volcontrol.h
volcontrol.o : volcontrol.h

clean : 
	rm -f $(objects) vol
