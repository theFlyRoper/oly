## doc/Makefile.am 
## Makefile.am doc automake file
## Process this file with automake to produce Makefile.in
## Copyright (C) Oly Project
##
## This program is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2, or (at your option)
## any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program; if not, write to the Free Software
## Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
## MA 02110-1301, USA.


MAINTAINERCLEANFILES 	= $(HTMLPAGES) 

CLEANFILES			= 

DISTCLEANFILES		= $(WIKIPAGES)

SUFFIXES			= .wiki

.PHONY: html-local htmlrebuild wikirebuild atool-clean wiki

wikirebuild:
	rm -f $(WIKIPAGES)

wiki: wikirebuild $(WIKIPAGES)

htmlrebuild:
	rm -f $(HTMLPAGES)

html-local: htmlrebuild $(HTMLPAGES)


maintainer-clean-local: wiki 

distclean-local: html-local

atool-clean: 
		rm -rf Makefile.in Makefile

TXTFILES				=	index.txt input_doc.txt code_standards.txt \
							inline_in_automake.txt bibliography.txt

WIKIPAGES							= $(TXTFILES:txt=wiki)
HTMLPAGES							= $(TXTFILES:txt=html)

adoc_icons_dir				= /doc/tip/doc/icons	
adoc_images_dir				= /doc/tip/doc/images

ASCIIDOC							=	asciidoc

ADOC_REVISION	= "`fossil finfo --limit 1 -b $< | \
								grep -o '^[0-9a-fA-F]*'`"
ADOC_REVDATE 	= "`fossil finfo --limit 1 -b $< | \
								grep -o '[0-9]*-[0-9]*-[0-9]*'`"

ADOCFLAGS	= -a email=@FAQ_ADDR@ -a website=@PACKAGE_URL@ \
			-a iconsdir=$(adoc_icons_dir) -a imagesdir=$(adoc_images_dir) \
			-a revdate=$(ADOC_REVDATE) -a 'newline=\n' \
			-a revision=$(ADOC_REVISION) --conf-file conf/fossil.conf

HTMLFLAGS	= -a email=@FAQ_ADDR@ -a website=@PACKAGE_URL@ \
			-a iconsdir=icons -a imagesdir=images -a data-uri \
			-a revdate=$(ADOC_REVDATE) -a revision=$(ADOC_REVISION) \
			--conf-file conf/asciidoc.conf

AD1		=	a2x

.txt.1:
	$(AD1) --doctype=manpage --format=manpage $<

.txt.wiki:
	$(ASCIIDOC) --doctype article $(ADOCFLAGS) -o $@ $<

.txt.html: 
	$(ASCIIDOC) --backend html5 --doctype article $(HTMLFLAGS) -o $@ $<

## generating an asciidoc definition for today
dist_doc_DATA					= $(HTMLPAGES)

EXTRA_DIST 						= $(TXTFILES) man/oly.1

