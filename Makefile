ASSEMBLER_DIRECTORY = Assembler
VM_DIRECTORY = VirtualMachine
COMPILER_DIRECTORY = Compiler
OS_DIRECTORY = JackOS

all: assembler vm

assembler:
	@cd $(ASSEMBLER_DIRECTORY) && $(MAKE)

vm:
	@cd $(VM_DIRECTORY) && $(MAKE)

%.hack: %.jack assembler vm
	$(eval FILE := $(basename $(notdir $<)))
	$(eval DIRECTORY := $(dir $<))
	@echo "Compiling $< to VM code..."
	@cd $(COMPILER_DIRECTORY) && python3 JackCompiler.py ../$<
	@echo "Converting VM to assembly..."
	@cd $(VM_DIRECTORY) && ./VMTranslator ../$(DIRECTORY)$(FILE).vm
	@echo "Assembling to machine code..."
	@cd $(ASSEMBLER_DIRECTORY) && ./Assembler ../$(DIRECTORY)$(FILE).asm
	@echo "Generated $@"

directory: assembler vm
	@DIRECTORY=$(word 2,$(MAKECMDGOALS)); \
	if [ -z "$$DIRECTORY" ]; then \
		echo "Usage: make directory <directory>"; \
		echo "Example: make directory Compiler/Square"; \
		exit 1; \
	fi; \
	echo "Compiling all .jack files in $$DIRECTORY..."; \
	cd $(COMPILER_DIRECTORY) && python3 JackCompiler.py ../$$DIRECTORY; \
	cd ..; \
	echo "Converting VM files to assembly..."; \
	cd $(VM_DIRECTORY) && for vm in ../$$DIRECTORY/*.vm; do \
		if [ -f "$$vm" ]; then \
			echo "Converting $$vm to assembly..."; \
			./VMTranslator "$$vm"; \
		fi; \
	done; \
	cd ..; \
	echo "Assembling to machine code..."; \
	cd $(ASSEMBLER_DIRECTORY) && for asm in ../$$DIRECTORY/*.asm; do \
		if [ -f "$$asm" ]; then \
			echo "Assembling $$asm to machine code..."; \
			./Assembler "$$asm"; \
		fi; \
	done; \
	echo "Compilation complete!"

clean:
	@cd $(ASSEMBLER_DIRECTORY) && $(MAKE) clean
	@cd $(VM_DIRECTORY) && $(MAKE) clean
	@cd $(COMPILER_DIRECTORY) && find . -name "*.vm" -delete && find . -name "*.asm" -delete && find . -name "*.hack" -delete
	@cd $(OS_DIRECTORY) && find . -name "*.vm" -delete && find . -name "*.asm" -delete && find . -name "*.hack" -delete

.PHONY: all assembler vm clean directory

# Prevent make from trying to build the directory path as a target
%:
	@:
