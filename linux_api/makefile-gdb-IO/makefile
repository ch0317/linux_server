ScrFiles=$(wildcard *.c)
ObjFiles=$(patsubst %c,%o,$(ScrFiles))
all:app

app:$(ObjFiles)
	gcc -o $@ -I./include $(ObjFiles)

%.o:%.c
	gcc -c $< -I./include -o $@

.PHONY:clean all

clean:
	-@rm -f *.o

