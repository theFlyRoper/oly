## i18n, both for tests and the main library. 

## build sequence: clean up derived files, rebuild text indexes, .txt to .res language files, .txt to .res index files, make a list of all res files, compile them into a single .dat file.  
## TODO: add the ability to switch to static/files/dll.

## I18N_MODE can also be a dll, static or files 
I18N_MODE								= common

OLY_I18N_DAT						= $(OLY_I18N_NAME).dat
I18N_DEPSDIR						= .i18ndeps
MAIN_I18N_OUTPUT					= i18n/$(OLY_I18N_DAT)
TEST_I18N_OUTPUT					= tests/data/$(MAIN_I18N_OUTPUT)
INDEX_NAME							=	res_index
INDEX_TXT							= $(addsuffix .txt,$(INDEX_NAME))
INDEX_RES							= $(addsuffix .res,$(INDEX_NAME))
ROOT_RES_NAME						=	root.res
LIST_NAME							=	res_list.txt

MAIN_OUTDIR							= $(abs_top_builddir)/i18n/
TEST_OUTDIR							= $(abs_top_builddir)/tests/data/i18n/
MAIN_SRCDIR							= $(abs_top_srcdir)/i18n/
TEST_SRCDIR							= $(abs_top_srcdir)/tests/data/i18n/
REL_MAIN_SRCDIR					= $(top_srcdir)/i18n/
REL_TEST_SRCDIR					= $(top_srcdir)/tests/data/i18n/

MAIN_ROOT								= $(addprefix $(MAIN_I18N_DIR),$(ROOT_RES_NAME))
TEST_ROOT								= $(addprefix $(TEST_I18N_DIR),$(ROOT_RES_NAME))
MAIN_I18N_INDEX					= $(addprefix $(MAIN_I18N_DIR),$(INDEX_RES))
TEST_I18N_INDEX					= $(addprefix $(TEST_I18N_DIR),$(INDEX_RES))
MAIN_I18N_INDEX_TXT			= $(addprefix $(MAIN_I18N_DIR),$(INDEX_TXT))
TEST_I18N_INDEX_TXT			= $(addprefix $(TEST_I18N_DIR),$(INDEX_TXT))
MAIN_I18N_TXT 					= $(addsuffix .txt,$(MAIN_I18N))
TEST_I18N_TXT 					= $(addsuffix .txt,$(TEST_I18N))
FULL_MAIN_I18N_TXT			= $(addprefix $(MAIN_I18N_DIR),$(MAIN_I18N_TXT))
FULL_TEST_I18N_TXT			= $(addprefix $(TEST_I18N_DIR),$(TEST_I18N_TXT))
MAIN_I18N_RES 					= $(addsuffix .res,$(MAIN_I18N)) $(MAIN_I18N_INDEX)
TEST_I18N_RES 					= $(addsuffix .res,$(TEST_I18N)) $(TEST_I18N_INDEX)

MAIN_I18N_RESLIST				= $(addprefix $(MAIN_OUTDIR),$(LIST_NAME))
TEST_I18N_RESLIST				= $(addprefix $(TEST_OUTDIR),$(LIST_NAME))

MAIN_I18N_BUILT					= $(MAIN_I18N_INDEX_TXT) $(MAIN_I18N_INDEX) $(MAIN_I18N_RESLIST)
TEST_I18N_BUILT					= $(TEST_I18N_INDEX_TXT) $(TEST_I18N_INDEX) $(TEST_I18N_RESLIST)
MAIN_I18N_CLEAN		= $(addprefix $(MAIN_OUTDIR),$(MAIN_I18N_RES)) \
			$(MAIN_I18N_BUILT)
TEST_I18N_CLEAN					= $(addprefix $(TEST_OUTDIR),$(TEST_I18N_RES)) \
			$(TEST_I18N_BUILT) 

# pkgdata version.
PKGDATAOPTS		=	--name $(OLY_I18N_NAME) --mode $(I18N_MODE) \
				-e $(OLY_I18N_NAME) -v -d . --tempdir $(I18N_DEPSDIR) -s . $(LIST_NAME)

GENRBOPT		= --bundle-name $(OLY_I18N_NAME) -d .

MAIN_GENRBOPT						= -s $(MAIN_SRCDIR) $(GENRBOPT) 
TEST_GENRBOPT						= -s $(TEST_SRCDIR) $(GENRBOPT) 
INDEX_GENRBOPT						= -s . $(GENRBOPT) 

i18n-main-built-clean:
	$(RMV) -rf $(MAIN_I18N_BUILT) $(I18N_DEPSDIR)

i18n-test-built-clean:
	$(RMV) -rf $(TEST_I18N_BUILT) $(TEST_I18N_DEPSDIR) 

## if the build_index breaks again this is useful for looking at the vars.

$(MAIN_I18N_INDEX_TXT): i18n-main-built-clean
	@echo "generating $@ (ICU primary resource index)"; \
	echo "// Warning this file is automatically generated" > $@; \
	echo "$(INDEX_NAME):table(nofallback) {" >> $@; \
	echo "    InstalledLocales {" >> $@; \
	for file in $(notdir $(MAIN_I18N)) ; do \
		echo "        $$file {\"\"}" >> $@; \
	done; \
	echo "    }" >> $@; \
	echo "}" >> $@;

$(TEST_I18N_INDEX_TXT): i18n-test-built-clean
	@echo "generating $@ (ICU test resource index)"; \
	echo "// Warning this file is automatically generated" > $@; \
	echo "$(INDEX_NAME):table(nofallback) {" >> $@; \
	echo "    InstalledLocales {" >> $@; \
	for file in $(notdir $(TEST_I18N)) ; do \
		echo "        $$file {\"\"}" >> $@; \
	done; \
	echo "    }" >> $@; \
	echo "}" >> $@;

$(MAIN_ROOT): 
	@echo "generating $@ (ICU primary resources)"
	cd $(MAIN_OUTDIR); \
		$(GENRB) $(MAIN_GENRBOPT) $(MAIN_I18N_TXT)

$(TEST_ROOT): 
	@echo "generating $@ (ICU test resources)"
	cd $(TEST_OUTDIR); \
		$(GENRB) $(TEST_GENRBOPT) $(TEST_I18N_TXT)

$(MAIN_I18N_INDEX): $(MAIN_I18N_INDEX_TXT) $(MAIN_ROOT)
	@echo "generating $@ (ICU resource index output)"
	cd $(MAIN_OUTDIR); \
		$(GENRB) $(INDEX_GENRBOPT) $(INDEX_TXT)

$(TEST_I18N_INDEX): $(TEST_I18N_INDEX_TXT) $(TEST_ROOT)
	@echo "generating $@ (test ICU resource index output)"
	cd $(TEST_OUTDIR); \
		$(GENRB) $(INDEX_GENRBOPT) $(INDEX_TXT)

i18n/$(LIST_NAME): $(MAIN_I18N_INDEX) 
	@echo "generating $@ (List of resource files for pkgdata)"
	cd $(MAIN_OUTDIR); \
	ls *.res > $(LIST_NAME)

tests/data/i18n/$(LIST_NAME): $(TEST_I18N_INDEX) 
	@echo "generating $@ (List of test resource files for pkgdata)"
	cd $(TEST_OUTDIR); \
	ls *.res > $(LIST_NAME)

## i18n-links: $(MAIN_I18N_INDEX) $(MAIN_ROOT) i18n/$(LIST_NAME)
## 	@echo "Creating file system links so ICU can find the resource files."
## 	cd $(MAIN_OUTDIR); \
## 	for item in `ls *.res`; do $(LN_S) $$item $(OLY_I18N_NAME)_$$item; done;
## 
## test-i18n-links: $(TEST_I18N_INDEX) $(TEST_ROOT) i18n/$(LIST_NAME)
## 	@echo "Creating file system links so ICU can find the resource files."
## 	cd $(TEST_OUTDIR); \
## 	for item in `ls *.res`; do $(LN_S) $$item $(OLY_I18N_NAME)_$$item; done;

i18n/@OLY_RESOURCE@.dat: i18n/$(LIST_NAME) 
	@echo "Running pkgdata on primary resources."
	cd $(MAIN_OUTDIR); \
	$(MKDIR_P) $(I18N_DEPSDIR); \
	$(PKGDATA) $(PKGDATAOPTS); \
	$(RMV) -rf $(I18N_DEPSDIR); 

tests/data/i18n/@OLY_RESOURCE@.dat: tests/data/i18n/$(LIST_NAME) 
	@echo "Running pkgdata on test resources."
	cd $(TEST_OUTDIR); \
	$(MKDIR_P) $(I18N_DEPSDIR); \
	$(PKGDATA) $(PKGDATAOPTS); \
	$(RMV) -rf $(I18N_DEPSDIR); 

# end of i18n. 
