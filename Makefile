include config.mk
PREFIX="/usr/local"

default: all done
.PHONY: lib

all: mkdirs
	@make -s -C lib
	@make -s -C tests

debug: mkdirs
	@make -C lib
	@make -C tests

mkdirs: rmdirs
	@mkdir build
	@mkdir build/obj
	@mkdir build/bin

rmdirs:
	@rm -rf build

run_tests: scripts/test_runner.sh
	@./$<

clean: rmdirs
	@echo "cleaning ..."
	@echo "done! :)"

install:
	@echo "installing cog to [${PREFIX}/lib]"
	# @cp ./lib/libcog.a ${PREFIX}/lib
	@mkdir -p ${PREFIX}/include/cog
	@cp include/* ${PREFIX}/include/cog/
	@echo "cog installed!"

uninstall:
	@echo "removing cog"
	@rm ${PREFIX}/lib/libcog.a
	@rm -rf ${PREFIX}/include/cog

done:
	@echo "done! :)"
