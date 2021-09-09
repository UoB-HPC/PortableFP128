
CC ?= clang
CCBASE = $(notdir $(CC))

# Check for gcc and add libquadmath if we are using it.
ifneq "$(findstring gcc, $(CCBASE))" ""
# LDFLAGS += -lquadmath
endif

testPFP128_$(CCBASE): pfp128.h

%_$(CCBASE).o: %.c
	$(CC) -o $@ -c $(CFLAGS) $<

%: %.o
	$(CC) -o $@ $< -lm  $(LDFLAGS) 

clean:
	rm testPFP128_* *.o
