# ft_ping
Back on the grind. A 42 School project. the aim of the project is to emulate the ping command. The subject says: "Ping is the name of a command that allows you to test the accessibility of another machine through the IP network. The command also measures the time taken to receive a response, called the round-trip time."

# Parentesi sui Makefile praticamente un cheatsheet per crearne uno spiegato da Lollo per Lollo

Allora essendo che non tocco il basso livello da un po' di tempo mi sono ritrovato in questa situazione abbastanza normale direi che semplicemente non mi ricordo nullla.


Innanzitutto perché creare un mankefile invece che uno script in bash?
La risposta veloce: è più efficiente
La risposta effettiva: è più efficiente perché rispetto ad uno script in bash il  makefile prima di compilare controlla l'ultimo timestamp dei file, se sono avvenute modifiche e di conseguenza se dei file vanno ricompilati, cosa che con uno script in bash dovresti controllare manualmente. Tutal più questa cosa viene controllata dal makefile in automatico e risparmierebbe dei controlli che sarebbero da aggiungere allo script in bash.
I makefile hanno una struttura a dipendenza lavorano con i cosidetti **target** questi ultimi vengono realizzati tramite dei **prerequisiti** che sono anch'essi spesso dei target.

```bash
COMPILING_RULES := gcc
PROGRAM_NAME := ft_ping
C_FILES := $(shell find srcs -name '*.c')
OBJS := $(C_FILES:srcs/%.c=objs/%.o)
FLAGS := -Wall -Wextra -Werror -g

GREEN := e[0;32m
RESET := e[0m
RED := e[1;31m

# objs folder rule creation without the -c the c files that don't have a main
# would go in an error the % is an expander it matches every file that has a .o or .c
# YOU HAVE to specify the folder or it would search in the same folder of the makefile

objs/%.o: srcs/%.c
	@echo "$(GREEN)file object compilation$(RESET)"
	@mkdir -p $(dir $@)
	$(COMPILING_RULES) $(FLAGS) -c $< -o $@


all: $(PROGRAM_NAME)

$(PROGRAM_NAME): $(OBJS)
	@echo "$(GREEN)program name rule$(RESET)"
	$(COMPILING_RULES) $(FLAGS) $(OBJS) -o $(PROGRAM_NAME)


clean:	
	@echo "$(GREEN)cleaning directory of objs$(RESET)"
	rm -rf objs


fclean: clean
	@echo "$(GREEN)removing every file created by Makefile$(RESET)"
	rm -f $(PROGRAM\_NAME)

re: fclean all

.PHONY: all clean fclean re
```
Pare un casino, se non sai le regole infatti sono qui per fare da mini remainder di alcune regole chiave che fanno funzionare questo makefile in maniera pulita e sensata.
Allora nelle prime righe semplicemente c'è la dichiarazione delle variabili, bisogna fare particolarmente attenzionem alle variabili che come contenuto possiedono dei valori elaborati tra parentesi. Queste io le chiamo _operazioni implicite_, in soldoni il risultato delle operazioni che stanno avvenendo tra parentesi andranno nella variabile.
Le due operazioni sono completamente diverse, e vi spiego il perché quella che avviene nella variabile C_FILES è praticamente la ricerca tramite bash di file .c infatti una cosa sorprendente è il fatto che il linguaggio make possiede della parole chiave per fare azioni speciali, nel caso mio mi serviva il risultato di un comando in bash e allora ho utilizzato la parola chiave **shell** per dire al makefile che la stringa dopo la parola chiave era un comando.

Nel caso dei file oggetto, c'è un po' di ragionamento in più diciamo che stiamo creando una variabile basandoci su di una sostituzione, infatti objs sta dicendo di essere praticamente come C_FILES però si trovano nella cartella objs e finiscono con .o un po' strano lo so ma funziona così. La cosa interessante è che questa "sostituzione" è coerente perché inserisce la stringa objs/nome_file.o dentro OBJS ma questi file non esistono ancora, infatti è solo una stringa, come vogliamo creare la variabile OBJS. infatti se vedi dopo nel makefile ci concentriamo prima a creare i file oggetto nella rispettiva cartella, e già da qui si realizza la dichiarazione di OBJS che quando verrà utillizzato, dopo la creazione dei file oggetto, funzionerà. Se i file oggetto non venissero creati prima di usare OBJS il makefile farebbe cilecca.
uff tosta eh ma non sconcetrarti adesso. Quindi ricorda: Le variabili sono stringhe finché non vengono invocate dei target

Comunque per darti un'idea una regola in makefile è strutturata così
Target: requisito

# Fonti
[guida alla creazione di un makefile](https://makefiletutorial.com/#-wildcard-1)
