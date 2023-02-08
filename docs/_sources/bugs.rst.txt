####
Bugs
####

This document describes some known bugs that are not yet fixed due to various reasons. 

Dot does not handle nodesets well
=================================

Currently, the dot diagram creates a link to the members of a nodeset and not the actual
nodeset. As a result, some nodesets appear to not have a parent in the dot diagram.



Circular phase definitions
==========================
When you have circular references in your phase definitions, a C compiler error is given.
For example, referencing the phase being defined in the actions block of that phase.