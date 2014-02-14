# makefile.am for resources

# uses gencmn to build the resource data
# gencmn can also make a C header, thus:
# gencmn --name oly_lang -t dat -S fortune_resources.c fortune_resources.txt 

.PHONY									= clean-local-res

SUFFIXES								= .res 

# Resource shortname
RESNAME									=	oly_lang

RESLIST									=	$(RESNAME).txt

RESDIR									=	$(top_srcdir)/resources

RESFILES								= root.res \
													es.res

RESLDFLAGS= 						-L$(RESDIR) -l$(RESNAME)

PKGDATAOPTS							=	-v -d . --tempdir .deps -e oly_lang 

LINK.cc									= $(CC) $(CFLAGS) $(LDFLAGS)

pkgdata_DATA						=	oly_lang.dat

.txt.res:
	@echo "generating $@"
	$(GENRB) $(GENRBOPT) $^ -d $(RESDIR)

$(RESLIST): $(RESFILES)
	ls *.res > $@

# gencmn can generate a .dat file too using these commands
oly_lang.dat: $(RESFILES) $(RESLIST)
	mkdir -p .deps
	gencmn --name $(RESNAME) -t dat 0 $(RESLIST)

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


