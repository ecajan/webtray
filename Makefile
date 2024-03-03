
default_target: build

build:
	@mkdir -p target
	@cd target && qmake6 .. && make

run: build
	./target/webtray ${URL}


.PHONY: build
