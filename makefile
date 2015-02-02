# Copyright (C) 2010, Redbeard Enterprises Ltd.
#
# Not to be used in any form without express written consent.
# All rights reserved.
#
# $Id: makefile 28 2011-02-10 07:06:19Z jason $

RUMPATH = redrum
include $(RUMPATH)/target.mk

REVISION="`git symbolic-ref HEAD 2> /dev/null | cut -b 12-`-`git log --pretty=format:\"%h\" -1`"

VERSION_MAJOR = $(word 1,$(subst _, ,$(shell cat version)))
VERSION_MINOR = $(word 2,$(subst _, ,$(shell cat version)))
VERSION_POINT = $(word 3,$(subst _, ,$(shell cat version)))

VERSION_STRING =$(VERSION_MAJOR).$(VERSION_MINOR).$(VERSION_POINT)-$(REVISION)

all:
	@$(MAKE) -w -C src
	@$(MAKE) -w -C lib
	@$(MAKE) -w -C src buildtest
	@$(MAKE) -w -C app

#dist:
#	: creating release package...
#	@cp -f changelog.txt changelogtmp.txt
#	@$(SED_IN_PLACE) "s/???/$(REVISION)/g" changelogtmp.txt
#	@ruby $(RUMPATH)/scripts/package.rb release_manifest.yaml $(VERSION_STRING)
#	@rm -f changelogtmp.txt

%:
	@$(MAKE) -w -C src $@
	@$(MAKE) -w -C lib $@
	@$(MAKE) -w -C app $@
