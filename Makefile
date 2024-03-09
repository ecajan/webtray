
default_target: build

build:
	@mkdir -p target
	@cd target && qmake6 .. && make

run: build
	./target/webtray ${URL}

user_install: build
	install ./target/webtray ~/.local/bin/webtray

install:
	install ./target/webtray /usr/local/bin/webtray


.PHONY: build user_install install
