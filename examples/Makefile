all:    testxversion \
        allknownelemspkg.gz	\
        allknownwarnings.gz	\
        badcontentsxml.gz	\
        dependency1.gz		\
        dependency2.gz		\
        duplicates.gz           \
        empty.gz		\
        missingfile.gz		\
        multiplecontents.gz	\
        nocontents.gz		\
        unknownelem.gz		\
        unsupportedprereq.gz

distclean: clean

clean:
	rm -f *.gz testxversion

allknownelemspkg.gz:  allknownelemspkg			\
	              allknownelemspkg/dropifexists.sql	\
                      allknownelemspkg/initUpgrade	\
                      allknownelemspkg/package.xml	\
                      allknownelemspkg/packageschema.sql	\
                      allknownelemspkg/pkgtest.sql	\
                      allknownelemspkg/pkgtestitem.sql \
                      allknownelemspkg/pkgtesttrigger.sql	\
                      allknownelemspkg/pkgtesttriggerfunction.sql	\
                      allknownelemspkg/pkgtestview.sql	\
                      allknownelemspkg/telephone.jpeg	\
                      allknownelemspkg/telephone.png	\
                      allknownelemspkg/telephone_ascii.jpg	\
                      allknownelemspkg/telephonelookup.mql	\
                      allknownelemspkg/telephonelookup.script	\
                      allknownelemspkg/telephonelookup.ui	\
                      allknownelemspkg/telephonelookup.xml      \
                      allknownelemspkg/finalize.sql
	tar czf $@ --exclude .svn $<

allknownwarnings.gz:  allknownwarnings			\
                      allknownwarnings/contents.xml	\
                      allknownwarnings/initUpgrade	\
                      allknownwarnings/telephone.jpeg	\
                      allknownwarnings/telephone.png	\
                      allknownwarnings/telephone_ascii.jpg	\
                      allknownwarnings/telephonelookup.script	\
                      allknownwarnings/telephonelookup.ui	\
                      allknownwarnings/telephonelookup.xml
	tar czf $@ --exclude .svn $<

badcontentsxml.gz: badcontentsxml	\
                   badcontentsxml/contents.xml
	tar czf $@ --exclude .svn $<

dependency1.gz: dependency1			\
                dependency1/contents.xml	\
                dependency1/telephone.jpeg
	tar czf $@ --exclude .svn $<

dependency2.gz: dependency2			\
                dependency2/contents.xml	\
                dependency2/telephone.png
	tar czf $@ --exclude .svn $<
duplicates.gz:  duplicates                      \
                duplicates/package.xml          \
                duplicates/loadimage.png        \
                duplicates/loadmetasql.mql      \
                duplicates/loadreport.xml       \
                duplicates/sampledisplay.script \
                duplicates/sampledisplay.ui
	tar czf $@ --exclude .svn $<

empty.gz:
	touch empty.gz

missingfile.gz: missingfile			\
                missingfile/contents.xml
	tar czf $@ --exclude .svn $<

multiplecontents.gz:  multiplecontents	\
                      multiplecontents/a_contents.xml	\
                      multiplecontents/b_contents.xml
	tar czf $@ --exclude .svn $<

nocontents.gz: nocontents nocontents/a
	tar czf $@ --exclude .svn $<

unknownelem.gz: unknownelem			\
                unknownelem/contents.xml	\
                unknownelem/initUpgrade		\
                unknownelem/setVersion.sql
	tar czf $@ --exclude .svn $<

unsupportedprereq.gz: unsupportedprereq			\
                      unsupportedprereq/contents.xml
	tar czf $@ --exclude .svn $<

testxversion: testxversion.cpp ../lib/libupdatercommon.a
	g++ -o testxversion testxversion.cpp \
	                      -g -pipe -Wall \
	                      -DQT_NO_DEBUG -DQT_CORE_LIB -DQT_SHARED \
	                      -I../common -I$(QTDIR)/include/QtCore -I$(QTDIR)/include \
	                      -L../lib    -L$(QTDIR)/lib -lupdatercommon -lQtCore
