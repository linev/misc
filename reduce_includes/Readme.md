# Running executable

    reduce_includes '<make command>' file1 file2 ...

# Running with go4 ROOT sources

From main go4 folder:

    reduce_includes 'make $$go4obj$$ >/dev/null 2>&1' Go4StatusBase/*.cxx


# Running with go4 Qt sources

From qt4/Go4GUI folder

    reduce_includes 'make -f Makefile.qt $$qtobj$$ >/dev/null 2>&1' TGo4AnalysisWindow.cpp