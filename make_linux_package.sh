#! /bin/sh
#set -x

SOURCE_RELEASE=src/linoleum_linux32

rm -rf $SOURCE_RELEASE/*
# create an archive of the source of the RTM from svn
svn co https://lino-linux32.svn.sourceforge.net/svnroot/lino-linux32/trunk $SOURCE_RELEASE
# prepare source folder for release
pushd $SOURCE_RELEASE
make clean
mv Makefile Makefile.in
popd
# tar and gzip source
tar -czvf linoleum_linux32_src.tar.gz $SOURCE_RELEASE/*
# build linux rtm
pushd $SOURCE_RELEASE
./configure
make install
popd
# create main/makertmp.bin
main/compile main/lib/ppkh/makertmp.txt
mv main/lib/ppkh/makertmp.bin main/
# create main/linux_compiler.bin
main/compile examples/visual_compiler/linux_compiler.txt
mv examples/visual_compiler/linux_compiler.bin main/
# create an archive with the linux compiler and supporting tools, system files and sources
tar -czvf linux_compiler.tar.gz \
    main/compile main/wincompile \
    main/linux_compiler.bin main/makertmp.bin main/sys/linux.bin \
    main/lib/ppkh/makertmp.txt examples/visual_compiler/linux_compiler.txt \
    main/lib/gen/compiler.txt \
    .k/README ./make_linux_package.sh
# create final zip file
zip linux_compiler.zip linux_compiler.tar.gz linoleum_linux32_src.tar.gz install_linux_compiler
