ASSEMBLER_DIR = Assembler
VM_DIR = VirtualMachine
COMPILER_DIR = Compiler

all: assembler virtual-machine

assembler:
	@cd $(ASSEMBLER_DIR) && $(MAKE)

virtual-machine:
	@cd $(VM_DIR) && $(MAKE)

%.hack: %.jack assembler virtual-machine
	$(eval FILE := $(basename $(notdir $<)))
	$(eval DIRECTORY := $(dir $<))
	@echo "Compiling $< to VM code..."
	@cd $(COMPILER_DIR) && python3 JackCompiler.py ../$<
	@echo "Converting VM to assembly..."
	@cd $(VM_DIR) && ./VMTranslator ../$(DIRECTORY)$(FILE).vm
	@echo "Assembling to machine code..."
	@cd $(ASSEMBLER_DIR) && ./Assembler ../$(DIRECTORY)$(FILE).asm
	@echo "Generated $@"

compile-dir: assembler virtual-machine
	@if [ -z "$(DIR)" ]; then \
		echo "Usage: make compile-dir DIR=<directory>"; \
		exit 1; \
	fi
	@echo "Compiling all .jack files in $(DIR)..."
	@cd $(COMPILER_DIR) && python3 JackCompiler.py ../$(DIR)
	@echo "Converting VM files to assembly..."
	@cd $(VM_DIR) && for vm in ../$(COMPILER_DIR)/$(DIR)/*.vm; do \
		if [ -f "$$vm" ]; then \
			./VMTranslator "$$vm"; \
		fi; \
	done
	@echo "Assembling to machine code..."
	@cd $(ASSEMBLER_DIR) && for asm in ../$(COMPILER_DIR)/$(DIR)/*.asm; do \
		if [ -f "$$asm" ]; then \
			./Assembler "$$asm"; \
		fi; \
	done
	@echo "Compilation complete!"

clean:
	@cd $(ASSEMBLER_DIR) && $(MAKE) clean
	@cd $(VM_DIR) && $(MAKE) clean
	@cd $(COMPILER_DIR) && find . -name "*.vm" -delete && find . -name "*.asm" -delete && find . -name "*.hack" -delete

.PHONY: all assembler virtual-machine clean compile-dir
