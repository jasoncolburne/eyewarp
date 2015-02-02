# Copyright (C) 2010, Redbeard Enterprises Ltd.
#
# Not to be used in any form without express written consent.
# All rights reserved.
#
# $Id: makefile 2 2010-03-29 03:41:56Z jason $

RUMPATH := ../redrum

include $(RUMPATH)/target.mk

APPS := $(strip $(subst /, ,$(dir $(wildcard */makefile))))

apps:
	@$(foreach APP, $(APPS),$(MAKE) -w -C $(APP) &&) $(ECHO) -n

%:
	@$(foreach APP, $(APPS),$(MAKE) -w -C $(APP) $@ &&) $(ECHO) -n
