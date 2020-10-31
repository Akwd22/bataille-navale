# Tutoriel : https://gl.developpez.com/tutoriel/outil/makefile/

CC 	 = gcc 		# Variable désignant le compilateur
EXEC = main 	# Variable désignant le nom de l'exécutable

$(EXEC) : main.o options.o utils.o joueur.o bateau.o plateau.o ia.o jeu.o divers.o combat.o prep.o menu.o dao.o
	$(CC) $^ -o $@ -g

main.o : ../src/main.c
	$(CC) -c ../src/main.c -g

options.o : ../src/metier/options.c
	$(CC) -c ../src/metier/options.c -g

utils.o : ../src/metier/utils.c
	$(CC) -c ../src/metier/utils.c -g

joueur.o : ../src/metier/joueur.c
	$(CC) -c ../src/metier/joueur.c -g

bateau.o : ../src/metier/bateau.c
	$(CC) -c ../src/metier/bateau.c -g

plateau.o : ../src/metier/plateau.c
	$(CC) -c ../src/metier/plateau.c -g

jeu.o : ../src/metier/jeu.c
	$(CC) -c ../src/metier/jeu.c -g

ia.o : ../src/metier/ia.c
	$(CC) -c ../src/metier/ia.c -g

divers.o : ../src/ihm/divers.c
	$(CC) -c ../src/ihm/divers.c -g

combat.o : ../src/ihm/combat.c
	$(CC) -c ../src/ihm/combat.c -g

prep.o : ../src/ihm/prep.c
	$(CC) -c ../src/ihm/prep.c -g

menu.o : ../src/ihm/menu.c
	$(CC) -c ../src/ihm/menu.c -g

dao.o : ../src/dao/dao.c
	$(CC) -c ../src/dao/dao.c -g

# Nettoyer les fichiers intermédiaires
clean :
	rm -rf *.o

# Nettoyer les exécutables
mrproper : clean
	rm -rf $(EXEC)