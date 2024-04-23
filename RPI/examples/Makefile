EXEC = Exec.elf

# Répertoire d'installation des outils de développement
# pour Raspberry Pi (à adapter, si besoin).
# RASPBERRY_TOOLS = /<path>/<to>/<RaspberryPi>/<Tools>/
# exemple : RASPBERRY_TOOLS = /home/matth/Logiciels/RaspberryPi/Tools/

# Compilateur croisé à utiliser (à adapter)
RASPBERRY_GCC = aarch64-linux-gnu-gcc
# exemple : RASPBERRY_GCC = $(RASPBERRY_TOOLS)/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin/arm-linux-gnueabihf-gcc

# Sysroot (à adapter)
RASPBERRY_SYSROOT = /home/bapt/Projects/ESEO/S8-PMU/RPI/rootfs_pi3
# exemple : RASPBERRY_SYSROOT = $(RASPBERRY_TOOLS)/rootfs_bplus/

# Compilateur.
CC = $(RASPBERRY_GCC)
LD = $(RASPBERRY_GCC)
CCFLAGS += --sysroot=$(RASPBERRY_SYSROOT)
CCFLAGS += -std=c99 -Wall -pedantic


#Organisation des sources
SRC = 	$(wildcard *.c) 

OBJ = 	$(SRC:.c=.o)


#Regles de compilations
all: $(EXEC)

$(EXEC): $(OBJ)
	$(LD) $^ -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CCFLAGS) -c $< -o $@
	
#Clean Project
clean:
	$(RM) *.o $(EXEC)
