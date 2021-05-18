# Checking existing streamer infos on duplications

Try to check if there are data members in base classes with same name
TStyle::fLineStyle is one example

First generate list of all linkdefs :

     [shell] ./makelist.sh

Then run macro

     [shell] root -l check_si.cxx -q > log.txt 2>&1

And check for "DUPL" in output string