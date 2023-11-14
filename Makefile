EXEC = minishell
CFLAGS = -Wall

all : $(EXEC)

minishell: minishell.c
	gcc $(CFLAGS) -o minishell minishell.c

git:
	git add .
	git commit -m ""
	git push

clean:
	rm -f *.o *~ *.backup

vclean:	clean
	rm -f $(EXEC)