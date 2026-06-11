VAR_BASICA = daje roma
bar := ${subst not,"totally", "I am not superman"}

foo := a.o b.o l.a c.o
one := $(patsubst %.o,%.c,$(foo))
# This is a shorthand for the above
two := $(foo:%.o=%.c)
# This is the suffix-only shorthand, and is also equivalent to the above.
three := $(foo:.o=.c)

# This is to test the filter gimmick
obj_files = foo.result bar.o lose.o
src_files = foo.raw bar.c lose.c

superman:
	@echo $(bar)

replacing:
	@echo $(one)
	@echo $(two)
	@echo $(three)
all:
ifeq ($(VAR_BASICA),daje roma)
	@echo "c'è quello che cerchi"
endif



filter_test: $(obj_files)
# Note: PHONY is important here. Without it, implicit rules will try to build the executable "all", since the prereqs are ".o" files.
.PHONY: all

# Ex 1: .o files depend on .c files. Though we don't actually make the .o file.
$(filter %.o,$(obj_files)): %.o: %.c
	echo "target: $@ prereq: $<"

# Ex 2: .result files depend on .raw files. Though we don't actually make the .result file.
$(filter %.result,$(obj_files)): %.result: %.raw
	echo "target: $@ prereq: $<"

%.c %.raw:
	touch $@

clean:
	rm -f $(src_files)
