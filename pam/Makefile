

pam_game_login.o: pam_game_login.c
	gcc -fPIC -fno-stack-protector -c pam_game_login.c

install: pam_game_login.o
	gcc -fPIC -c campo_minado_linux.c FlappyBird.c ForcaPlay.c
	gcc -shared -o /lib64/lib_jogos.so campo_minado_linux.o FlappyBird.o ForcaPlay.o
	gcc -shared -o /lib64/security/pam_game_login.so pam_game_login.o -L/lib64 -l:lib_jogos.so
	sudo cp /etc/login.defs /etc/login.defs.bkp
	sudo sed -i '134c LOGIN_TIMEOUT		180' /etc/login.defs
	sudo sed -i '/preauth/a auth	required		pam_game_login.so' /etc/pam.d/system-auth

uninstall:
	rm -f /lib64/security/pam_game_login.so
	rm -f /lib64/lib_jogos.so
	sudo sed -i '4d' /etc/pam.d/system-auth
	@echo -e "\n\n      Remove any entry related to this module in /etc/pam.d/ files,\n      otherwise you're not going to be able to login.\n\n"
debug:
	gcc -E -fPIC -fno-stack-protector -c pam_game_login.c
clean:
	rm -rf *.o
