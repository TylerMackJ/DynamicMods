all : bin/modloader bin/mods/hello.mod bin/mods/test.mod

bin/modloader : src/modloader.c bin/
	gcc -Wall src/modloader.c -o bin/modloader

bin/mods/%.mod : src/mods/%.c bin/mods/
	gcc -Wall -shared -rdynamic -fPIC $< -o $@

bin/mods/ :
	mkdir -p bin/mods/
	
bin/ :
	mkdir -p bin/

clean : bin/
	rm -r bin/
