scriptmine.so:
	gcc -shared -fPIC -o scriptmine.so scriptmine.c src/duktape.c -Isrc/ `pkg-config --cflags --libs luajit` -lm

clean:
	rm -f scriptmine.so
