EXEC = minishell
CFLAGS = -Wall

all : $(EXEC)

minishell: minishell.c
	gcc $(CFLAGS) -o minishell minishell.c

clean:
	rm -f *.o *~ *.backup

vclean:	clean
	rm -f $(EXEC)