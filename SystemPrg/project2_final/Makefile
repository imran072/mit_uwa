C11	= cc -std=c11 -Wall -Werror

mysync : mysync.c comp_dir.h linked_list.c comp_dir.c sync_dir.c mysync.h global.c copy_sync_dir.c
	$(C11) -o mysync mysync.c linked_list.c comp_dir.c sync_dir.c global.c copy_sync_dir.c

clean:
	rm -f *.o mysync