# ft_ping
Back on the grind. A 42 School project. the aim of the project is to emulate the ping command. The subject says: "Ping is the name of a command that allows you to test the accessibility of another machine through the IP network. The command also measures the time taken to receive a response, called the round-trip time."


Ping lavora al livello 3 del modello OSI
![alt text](https://www.network-supply.com/cdn/shop/articles/ChatGPT_Image_Sep_15_2025_11_29_37_AM.png?v=1757953612)


La comunicazione di ping è gestita grazie a un echo request seguito da un echo reply
echo request type = 8
reply = 0

l'utilizzo di telnet e nc vuol dire che stai utilizzando protocolli tcp/udp under the hood.
Quindi il mio obiettivo sarebbe quello di mandare.

lavorare con rawbits
il bit è costruito così:
Tipo - Codice - 

# Parentesi sui Makefile praticamente un cheatsheet per crearne uno spiegato da Lollo per Lollo

Allora essendo che non tocco il basso livello da un po' di tempo mi sono ritrovato in questa situazione abbastanza normale direi che semplicemente non mi ricordo nullla.


Innanzitutto perché creare un mankefile invece che uno script in bash?
La risposta veloce: è più efficiente
La risposta effettiva: è più efficiente perché rispetto ad uno script in bash il  makefile prima di compilare controlla l'ultimo timestamp dei file, se sono avvenute modifiche e di conseguenza se dei file vanno ricompilati, cosa che con uno script in bash dovresti controllare manualmente. Tutal più questa cosa viene controllata dal makefile in automatico e risparmierebbe dei controlli che sarebbero da aggiungere allo script in bash.
I makefile hanno una struttura a dipendenza lavorano con i cosidetti **target** questi ultimi vengono realizzati tramite dei **prerequisiti** che sono anch'essi spesso dei target.

```makefile
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
# $@ stands for the target literally "objs/%.o" and $< stands for the requisite "srcs/%.c

objs/%.o: srcs/%.c
	@echo "$(GREEN)file object compilation$(RESET)"
	@mkdir -p $(dir $@)OBJS
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
	rm -f $(PROGRAM_NAME)

re: fclean all

.PHONY: all clean fclean re
```
Pare un casino, se non sai le regole infatti sono qui per fare da mini remainder di alcune regole chiave che fanno funzionare questo makefile in maniera pulita e sensata.
Allora nelle prime righe semplicemente c'è la dichiarazione delle variabili, bisogna fare particolarmente attenzione alle variabili che come contenuto possiedono dei valori elaborati tra parentesi. Queste io le chiamo _operazioni implicite_, in soldoni il risultato delle operazioni che stanno avvenendo tra parentesi andranno nella variabile.
Le due operazioni sono completamente diverse, e vi spiego il perché quella che avviene nella variabile C_FILES è praticamente la ricerca tramite bash di file .c infatti una cosa sorprendente è il fatto che il linguaggio make possiede della parole chiave per fare azioni speciali, nel caso mio mi serviva il risultato di un comando in bash e allora ho utilizzato la parola chiave **shell** per dire al makefile che la stringa dopo la parola chiave era un comando.

Nel caso dei file oggetto, c'è un po' di ragionamento in più diciamo che stiamo creando una variabile basandoci su di una sostituzione, infatti objs sta dicendo di essere praticamente come C_FILES però si trovano nella cartella objs e finiscono con .o un po' strano lo so ma funziona così. La cosa interessante è che questa "sostituzione" è coerente perché inserisce la stringa objs/nome_file.o dentro OBJS ma questi file non esistono ancora, infatti è solo una stringa, come vogliamo creare la variabile OBJS. infatti se vedi dopo nel makefile ci concentriamo prima a creare i file oggetto nella rispettiva cartella, e già da qui si realizza la dichiarazione di OBJS che quando verrà utillizzato, dopo la creazione dei file oggetto, funzionerà. Se i file oggetto non venissero creati prima di usare OBJS il makefile farebbe cilecca.
uff tosta eh ma non sconcetrarti adesso. Quindi ricorda: Le variabili sono stringhe finché non vengono invocate dei target

Comunque per darti un'idea una regola in makefile è strutturata così
Target: requisito
	regole per creare target $(requisito)

Ci sono delle piccolezze da ricordarsi quando si fa un makefile
1. Il makefile se viene invocato normalmente, quindi solo make, andrà dall'alto verso il basso eseguendo le regole target in ordine
2. Se una regola ha bisogno di un requisito il makefile salterà nella regola target che creerebbe il requisito della regola precedente

quindi nel nostro esempio sopra la prima regola che verrà eseguita è quella per creare i file oggetto. quella regola però a bisogno della cartella "srcs/%.c" che in italiano significa: ho bisogno che da dove eseguo il makefile sia presente la cartella srcs e che al suo interno ci siano dei file c, se ci sono la regola utilizzerà i file c presenti in srcs.

la regola successiva per esempio all ha bisogno di PROGRAM_NAME per funzionare → PROGRAM_NAME ha bisogno dei file oggetto e così via abbiamo già spiegato come viene costruita la variabile OBJS

## shortcut o cose che non sembrano aver senso pt1
hai visto $@ $< rispettivamente uno rappresenta il target e l'altro il requisito
Quindi se io avessi
Target: requisito
	echo $@ $<

mi stamperebbe Target requisito. è comodo perché quando hai una wildcard %.c ti rappresenta come si deve ogni singolo file di interesse
per aiutarti ecco un mini esempio.
```bash
hey: one two
# Outputs "hey", since this is the target name
echo $@

# Outputs all prerequisites newer than the target
echo $?

# Outputs all prerequisites
echo $^

# Outputs the first prerequisite
echo $< touch hey one: touch one two: touch two clean: rm -f hey one two
```

.PHONY viene usato semplicemente perché così eviti di utilizzare parole chiave che potrebbero essere usate dal terminale.
# Fonti
[guida alla creazione di un makefile](https://makefiletutorial.com)


## Libraries
wenet.h
sockaddress.h ecc

network -> big endian
our_machine -> little endian
h_to_h

tokenizer per le flag getopt (funzione) per le opzioni di ping
parsing solo -v e -h

getpopts -> per le flag lunghe
flag? -> vuol dire che ha un argomento dopo la flag
hmp

time to live [bonus] è una flag.
Eval sheet -> time to live basso
