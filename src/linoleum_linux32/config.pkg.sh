TEST_INSTALL="install ./install.sh"

USER_HELP=""

USERRULES='AS=as
ASFLAGS=--32
.SUFFIXES: .s
.s.o:
	$(AS) $(ASFLAGS) -o $@ $<
'

config() {

	# Test for assembler
	clean .config.s .config.o .config.c .config
	cat > .config.s <<EOF
	.text
	.globl linoleum
	.type linoleum, @function
linoleum:
pushl	%ebp
movl	%esp, %ebp
popl	%ebp
ret
	.size linoleum,. - linoleum

EOF
	test_compile "assembler" || {
	    clean .config.s .config.o
	    check_message_results "!! Could not compile with as. Make sure you have an assembler."
	    exit 1
	}
	clean .config.s .config.o

	# Test for all required functionality

	cat > .config.c <<EOF
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
	FILE *file;
	int fd;
	fd = mkstemp("fooXXXXXX");
	fdopen(fd, "r+");
	umask(~S_IRWXU);
	file = popen("echo", "r");
	pclose(file);
	return 0;
}
EOF

	test_link "required functions" || {
	    check_message_result "Testing required functions seperatly for debugging purposes"
	    cat > .config.c <<EOF
#include <stdlib.h>

int main(int argc, char *argv[]) {
	mkstemp("fooXXXXXX");
	return 0;
}
EOF
		test_compile "mkstemp"

		cat > .config.c <<EOF
#include <stdio.h>

int main(int argc, char *argv[]) {
	fdopen(0, "r+");
	return 0;
}
EOF
		test_compile "fdopen"
			
		cat > .config.c <<EOF
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
	umask(~S_IRWXU);
	return 0;
}
EOF
		test_compile "umask"
		
		cat > .config.c <<EOF
#include <stdio.h>

int main(int argc, char *argv[]) {
	popen("echo", "r");
	return 0;
}
EOF
		test_compile "popen"

		cat > .config.c <<EOF
#include <stdio.h>

int main(int argc, char *argv[]) {
	pclose((void *)0);
	return 0;
}
EOF
		test_compile "pclose"

		exit 1
	}
	clean .config.o .config.c

	create_makefile
}

sed_lines() {
	add_replace_settings "$@"
}
