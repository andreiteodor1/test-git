build: 
	gcc -o lanParty main.c
run:
	./lanParty c.in d.in r.out

clean:
	rm lanParty