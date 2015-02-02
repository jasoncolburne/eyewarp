# Copyright (C) 2010, Redbeard Enterprises Ltd.
#
# Not to be used in any form without express written consent.
# All rights reserved.
#
# $Id: makefile 1 2010-04-07 05:05:06Z jason $

RUMPATH := ../redrum

ifeq ("", "$(wildcard $(RUMPATH)/target.mk)")
  $(error Please run 'make prepare' from top level to retrieve the redrum build system)
endif

include $(RUMPATH)/target.mk

MODULES := $(strip $(subst /, ,$(dir $(wildcard */makefile))))

modules:
	@$(foreach MODULE, $(MODULES),$(MAKE) -w -C $(MODULE) &&) $(ECHO) -n

%:
	@$(foreach MODULE, $(MODULES),$(MAKE) -w -C $(MODULE) $@ &&) $(ECHO) -n
