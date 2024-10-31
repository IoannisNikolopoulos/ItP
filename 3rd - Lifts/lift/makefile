vpath %.c src
vpath %.h include

EXEDIR := bin
EXES := liftrec liftbf liftmem liftdp

INCLUDES := $(wildcard include/*.h)
DEPS := lift.o utils.o

OBJS := $(addsuffix .o, $(EXES))
OBJS += $(DEPS)
OBJSDIR := build

OBJS := $(addprefix $(OBJSDIR)/, $(OBJS))
EXES := $(addprefix $(EXEDIR)/, $(EXES))

CFLAGS += -g -O0 -Iinclude

.PHONY: clean

all: $(EXES)

$(EXEDIR)/liftrec: $(addprefix $(OBJSDIR)/,liftrec.o $(DEPS))
	@mkdir -p $(EXEDIR)
	$(CC) -o $@ $^

$(EXEDIR)/liftbf: $(addprefix $(OBJSDIR)/,liftbf.o $(DEPS))
	@mkdir -p $(EXEDIR)
	$(CC) -o $@ $^

$(EXEDIR)/liftmem: $(addprefix $(OBJSDIR)/,liftmem.o $(DEPS))
	@mkdir -p $(EXEDIR)
	$(CC) -o $@ $^

$(EXEDIR)/liftdp: $(addprefix $(OBJSDIR)/,liftdp.o $(DEPS))
	@mkdir -p $(EXEDIR)
	$(CC) -o $@ $^

$(OBJSDIR)/%.o: %.c $(INCLUDES)
	@mkdir -p $(OBJSDIR)
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	$(RM) -r $(EXEDIR) $(OBJSDIR)
