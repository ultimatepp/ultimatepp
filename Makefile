SHELL := /bin/bash
UNAME := $(shell uname)

.PHONY: all deps config umk theide run clean

all: deps config umk theide
	@if [ -x "./theide" ] || [ -d "./theide.app" ]; then \
		echo "Install process has been finished, TheIDE is built successfully."; \
		read -p "Do you want to start TheIDE now? (Y/n): " answer; \
		if [[ ! "$$answer" =~ ^[nN] ]]; then \
			$(MAKE) run; \
		fi; \
	else \
		echo "Something went wrong."; \
		echo "Please notify developers."; \
	fi

deps:
	@chmod +x install_deps.sh
	@./install_deps.sh $(RUN_ARGS)

config:
	mkdir -p .config

umk: config
	@echo "Building umk"
	@if clang++ --version >/dev/null 2>&1; then \
		clang -O2 -pthread -Iuppsrc -I. -DflagMAIN -DflagNO_FONTSYS mkumk_blitz.cpp uppsrc/Draw/FontCR.cpp uppsrc/Core/heaputil.cpp uppsrc/Core/hheap.cpp uppsrc/Core/sheap.cpp uppsrc/Core/lheap.cpp uppsrc/Core/heap.cpp uppsrc/Core/heapdbg.cpp uppsrc/Core/String.cpp uppsrc/Core/CharSet.cpp uppsrc/Core/CvFlt.cpp uppsrc/Core/xxHash_chk.cpp uppsrc/Core/Http.cpp -lstdc++ -llzma -lpcre2-8 -lpcre2-32 -llz4 -lzstd -lxxhash -lm -lz -lbz2 $$(pkg-config --libs libpng) $$(pkg-config --libs libpng) -o umk; \
	else \
		gcc -O2 -pthread -Iuppsrc -I. -DflagMAIN -DflagNO_FONTSYS mkumk_blitz.cpp uppsrc/Draw/FontCR.cpp uppsrc/Core/heaputil.cpp uppsrc/Core/hheap.cpp uppsrc/Core/sheap.cpp uppsrc/Core/lheap.cpp uppsrc/Core/heap.cpp uppsrc/Core/heapdbg.cpp uppsrc/Core/String.cpp uppsrc/Core/CharSet.cpp uppsrc/Core/CvFlt.cpp uppsrc/Core/xxHash_chk.cpp uppsrc/Core/Http.cpp -lstdc++ -llzma -lpcre2-8 -lpcre2-32 -llz4 -lzstd -lxxhash -lm -lz -lbz2 $$(pkg-config --libs libpng) $$(pkg-config --libs libpng) -o umk; \
	fi

theide: umk
	@echo "Building TheIDE"
	@if clang++ --version >/dev/null 2>&1; then \
		./umk ./uppsrc ide CLANG -brsH ./theide; \
	else \
		./umk ./uppsrc ide GCC -brsH ./theide; \
	fi

run:
	@if [ "$(UNAME)" = "Darwin" ]; then \
		open theide.app; \
		exit 0; \
	fi; \
	./theide

clean:
	rm -rf .config
	rm -rf .cache
	rm -rf .local
	rm -f umk theide
