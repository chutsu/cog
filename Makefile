include config.mk
PREFIX="/usr/local"

default: all done
.PHONY: lib

all: mkdirs
	@make -s -C lib -j4
	@make -s -C tests -j4

debug: mkdirs
	@make -C lib
	@make -C tests

mkdirs: rmdirs
	@mkdir build
	@mkdir build/obj
	@mkdir build/bin

rmdirs:
	@rm -rf build

run_tests:
	@./scripts/test_runner.sh

clean: rmdirs
	@echo "cleaning ..."
	@echo "done! :)"

install:
	@echo "installing cog to [${PREFIX}/lib]"
	@cp ./build/libcog.a ${PREFIX}/lib
	@mkdir -p ${PREFIX}/include/cog
	@cp include/* ${PREFIX}/include/cog/
	@echo "cog installed!"

uninstall:
	@echo "removing cog"
	@rm ${PREFIX}/lib/libcog.a
	@rm -rf ${PREFIX}/include/cog

done:
	@echo "done! :)"
