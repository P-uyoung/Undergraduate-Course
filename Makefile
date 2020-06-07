test : ssu_rsync.o ssu_crontab.o ssu_crond.o

ssu_rsync.o : ssu_rsync.c ssu_rsync.h
	gcc -c ssu_rsync.c

ssu_crontab.o : ssu_crontab.c ssu_crontab.h
	gcc -c ssu_crontab.c

ssu_crond.o : ssu_crond.c ssu_crond.h
	gcc -c ssu_crond.c -lpthread

clean :
	rm *.o

