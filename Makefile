SOURCE=$(shell find src -name "*.cpp")
OBJECT=$(SOURCE:src/%.cpp=${TARGET_DIRECTORY}/%.o)
DEPENDENCIES=$(SOURCE:src/%.cpp=${TARGET_DIRECTORY}/%.d)

include config.mk

default_target: all

option:
	@echo WebTray build options:
	@echo SOURCE = ${SOURCE}
	@echo OBJECT = ${OBJECT}
	@echo DEPENDENCIES = ${DEPENDENCIES}
	@echo CPPFLAGS = ${CPPFLAGS}
	@echo LDFLAGS = ${LDFLAGS}

${TARGET_DIRECTORY}:
	@printf "create target directory\n"
	@mkdir ${TARGET_DIRECTORY}

-include ${DEPENDENCIES}

${TARGET_DIRECTORY}/%.o: src/%.cpp Makefile
	@printf "build $<\n"
	@mkdir -p "$$(dirname $@)"
	@${CC} ${CPPFLAGS} ${LDFLAGS} -MMD -MP -c $< -o $@

${OBJECT}: | ${TARGET_DIRECTORY}

webtray: ${TARGET_DIRECTORY} ${OBJECT}
	@printf "build $@\n"
	@${CC} -o ${TARGET_DIRECTORY}/$@ ${OBJECT} ${LDFLAGS} ${CPPFLAGS}

all: | ${TARGET_DIRECTORY} webtray

clean:
	@printf "cleaning up\n"
	@rm -rf ${TARGET_DIRECTORY}

.PHONY: clean
