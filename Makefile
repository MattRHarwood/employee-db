TARGET = bin/dbview
SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c, obj/%.o, $(SRC))

run: clean default
	./$(TARGET) -n -f test.db
	./$(TARGET) -f test.db
	./$(TARGET) -f test.db -a "Trevor Test, 1 Aperture Lane, 80"
	./$(TARGET) -f test.db -a "Tina Test, 2 Aperture Lane, 85" -l
	./$(TARGET) -f test.db -r "Tina Test" -l
	./$(TARGET) -f test.db -u "Trevor Test,165" -l

default: $(TARGET)

clean:
	rm -rf obj/*.o
	rm -rf bin/*

$(TARGET) : $(OBJ)
	gcc -o $@ $?

obj/%.o : src/%.c
	gcc -c -g $< -o $@ -Iinclude

