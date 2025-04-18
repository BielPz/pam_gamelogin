

pam_game_login.o: pam_skel_block.c
	gcc -fPIC -fno-stack-protector -c pam_game_login.c

install: pam_game_login.o
	ld -x --shared -o /lib64/security/pam_game_login.so pam_game_login.o

uninstall:
	rm -f /lib64/security/pam_game_login.so
	@echo -e "\n\n      Remove any entry related to this module in /etc/pam.d/ files,\n      otherwise you're not going to be able to login.\n\n"
debug:
	gcc -E -fPIC -fno-stack-protector -c pam_game_login.c
clean:
	rm -rf *.o
