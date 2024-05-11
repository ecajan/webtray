
default_target: build

build:
	@mkdir -p target
	@cd target && qmake6 .. && make

run: build
	./target/webtray ${URL}

clean:
	@rm -rf target

install:
	install ./target/webtray ${DESTDIR}/usr/bin/webtray


.PHONY: build install
