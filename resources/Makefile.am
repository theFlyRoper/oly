# makefile.am for resources

# uses gencmn to build the resource data
# gencmn can also make a C header, thus:
# gencmn --name oly_lang -t dat -S fortune_resources.c fortune_resources.txt 
datadir									= @datadir@
datarootdir							= @datarootdir@
localedir								= @localedir@

DEFS 										= @DEFS@ @DEV_DEF@ -DLOCALEDIR=\"$(localedir)\"

.PHONY									= clean-local-res

SUFFIXES								= .res 

# Resource shortname
RESNAME									=	oly_lang

RESLIST									=	$(RESNAME).txt

RESDIR									=	$(top_srcdir)/resources

RESFILES								= root.res 

RESTEXT									= root.txt \
													en.txt

RESLDFLAGS= 						-L$(RESDIR) -l$(RESNAME)

PKGDATAOPTS							=	-v -d . --tempdir .deps -e $(RESNAME)

LINK.cc									= $(CC) $(CFLAGS) $(LDFLAGS)

GENRBOPT								= -v --bundle-name $(RESNAME)

pkgdata_DATA						=	oly_lang.dat


.txt.res:
	@echo "generating $@"
	$(GENRB) $(GENRBOPT) $^ -d $(RESDIR)

root.res:
	@echo "generating $@"
	$(GENRB) $(GENRBOPT) $^ -d $(RESDIR) $(RESTEXT)

$(RESLIST): $(RESFILES)
	ls *.res > $@

# $(GENRB) $(GENRBOPT) $^ -d $(RESDIR) $(RESFILES)

# gencmn can generate a .dat file too using these commands
oly_lang.dat: $(RESFILES) $(RESLIST)
	mkdir -p .deps
	$(PKGDATA) --name $(RESNAME) --mode common $(PKGDATAOPTS) $(RESLIST)

# liboly_lang.a: $(RESFILES) $(RESLIST)
#	mkdir -p .deps
#	$(PKGDATA) --name $(RESNAME) --mode static $(PKGDATAOPTS) $(RESLIST)

#	$(LINK.cc) $^ $(LOADLIBES) $(LDLIBS) -o $@ $(XTRALIBS)
# gencmn --name $(RESNAME) -t dat 0 $(RESLIST)

CLEANFILES							= $(RESFILES) \
													$(RESLIST) \
													$(RESTARGET) \
													$(RESNAME).dat \
													$(RESNAME)_dat.s 

clean-local-res:
	rm -rf .deps

clean-local: clean-local-res

distclean-local: clean-local 

$(TARGET): $(OBJECTS)
	$(LINK.cc) $^ $(LOADLIBES) $(LDLIBS) -o $@ $(XTRALIBS)


