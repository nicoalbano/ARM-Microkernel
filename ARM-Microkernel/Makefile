# Definición del compilador y las opciones de compilación
CHAIN=arm-none-eabi
CFLAGS=-std=gnu99 -Wall -mfpu=neon -mhard-float -mcpu=cortex-a8 -DCPU_A8
AFLAGS=-mfpu=neon
LDFLAGS =

# Directorios
INCDIR = inc
SRCDIR = src
OBJDIR = obj
BINDIR = bin
LSTDIR = lst

# Encuentra todos los archivos .c y .s en los subdirectorios de src
SRCFILES = $(shell find $(SRCDIR) -name '*.c' -o -name '*.s')
# Genera una lista de archivos .o correspondientes en el directorio obj
OBJFILES = $(patsubst $(SRCDIR)/%, $(OBJDIR)/%, $(SRCFILES:.c=.o))
OBJFILES := $(patsubst $(SRCDIR)/%, $(OBJDIR)/%, $(OBJFILES:.s=.o))

# Nombre del ejecutable
BINFILE = bios
TARGET = $(BINDIR)/$(BINFILE)
TARGET_ELF = $(TARGET).elf


# $^ es una variable automática que representa a una lista de todos los pre-requisitos de un objetivo
# $@ es una variable automática que representa el objetivo a "buildear"
# $< es una variable automática que representa el PRIMER pre-requisito del objetivo

# Regla principal
all: $(TARGET)

# Regla para generar el binario a partir del ELF
$(TARGET): $(TARGET_ELF)
	$(CHAIN)-objcopy -O binary $< $@

# Regla para enlazar el ejecutable
$(TARGET_ELF): $(OBJFILES)
	@echo "Linkeando ... "
	mkdir -p $(BINDIR)
	mkdir -p $(LSTDIR)
	$(CHAIN)-ld -T memmap.ld $(LDFLAGS) -o $@ $^ -Map $(LSTDIR)/$(BINFILE).map
	@echo "Linkeo finalizado!!"
	@echo ""
	@echo "Generando archivos de información: mapa de memoria y simbolos"
	readelf -a $@  > $(LSTDIR)/$(BINFILE)_readelf.txt
	$(CHAIN)-objdump -D $@  > $(LSTDIR)/$(BINFILE).lst

# Regla para compilar archivos .c a .o
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@echo ""
	mkdir -p $(dir $@)
	mkdir -p $(LSTDIR)
	@echo "Ensamblando $(notdir $<)..."
	$(CHAIN)-gcc $(CFLAGS) -g -c -o $@ $<

# Regla para ensamblar archivos .s a .o
$(OBJDIR)/%.o: $(SRCDIR)/%.s
	@echo ""
	mkdir -p $(dir $@)
	mkdir -p $(LSTDIR)
	@echo "Compilando $(notdir $<)..."
	$(CHAIN)-as $(AFLAGS) -o $@ $< -a > $(LSTDIR)/$(basename $(notdir $<)).lst

# Limpia los archivos generados
clean:
	rm -rf $(OBJDIR) $(BINDIR) $(LSTDIR)

# Phony targets
.PHONY: all clean
