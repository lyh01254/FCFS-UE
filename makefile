#------------------------------------------------------------
# CPLEX system setting
#------------------------------------------------------------
SYSTEM     = arm64_osx
LIBFORMAT  = static_pic

#------------------------------------------------------------
# Paths
#------------------------------------------------------------

CPLEXDIR      = /Applications/CPLEX_Studio2211/cplex
CONCERTDIR    = /Applications/CPLEX_Studio2211/concert
CPLEXLIBDIR   = $(CPLEXDIR)/lib/$(SYSTEM)/$(LIBFORMAT)
CONCERTLIBDIR = $(CONCERTDIR)/lib/$(SYSTEM)/$(LIBFORMAT)
CONCERTINCDIR = $(CONCERTDIR)/include
CPLEXINCDIR   = $(CPLEXDIR)/include

ALGO = ./source/algo
COMMON = ./source/common
DATA = ./source/data_process
EXP = ./source/experiment
SIM = ./source/simulation 

INCDIR = ./include
OBJDIR = ./objects
OUTDIR = ./exec

# ---------------------------------------------------------------------
# Complier
# ---------------------------------------------------------------------

CCC = clang++

# ---------------------------------------------------------------------
# Options and libraries
# ---------------------------------------------------------------------

# with CPLEX: Compliler options
CPCOPT = -m64 -O -fPIC -fexceptions -DNDEBUG -stdlib=libc++ -std=c++11
# with CPLEX: Include options
CPINCOPT = -I$(CPLEXINCDIR) -I$(CONCERTINCDIR) -I$(INCDIR)
# with CPLEX: Link options
CPLOPT = -lconcert -lilocplex -lcplex -lm -lpthread -framework CoreFoundation -framework IOKit
# with CPLEX: Library options
CPLIBOPT = -L$(CPLEXLIBDIR) -L$(CONCERTLIBDIR)

# Without CPLEX: Compiler options
COPT = -std=c++11#no options required
# Without CPLEX: Include options
INCOPT = -I$(INCDIR)
# Without CPLEX: Link options
LOPT = #no options required
# Without CPLEX: Library options
LIBOPT = #no options required

#---------------------------------------------------------------
# make file
#---------------------------------------------------------------


$(OBJDIR)/curve.o: $(ALGO)/curve.cpp
	$(CCC) -c $(COPT) $(INCOPT) $(ALGO)/curve.cpp -o $(OBJDIR)/curve.o

$(OBJDIR)/myIOS.o: $(ALGO)/myIOS.cpp
	$(CCC) -c $(COPT) $(INCOPT) $(ALGO)/myIOS.cpp -o $(OBJDIR)/myIOS.o

$(OBJDIR)/algo_core.o: $(ALGO)/algo_core.cpp
	$(CCC) -c $(COPT) $(INCOPT) $(ALGO)/algo_core.cpp -o $(OBJDIR)/algo_core.o

$(OBJDIR)/common.o: $(COMMON)/common.cpp
	$(CCC) -c $(COPT) $(INCOPT) $(COMMON)/common.cpp -o $(OBJDIR)/common.o

$(OBJDIR)/generate.o: $(DATA)/generate.cpp
	$(CCC) -c $(COPT) $(INCOPT) $(DATA)/generate.cpp -o $(OBJDIR)/generate.o
generate: $(OBJDIR)/generate.o $(OBJDIR)/common.o
	$(CCC) $(OBJDIR)/generate.o $(OBJDIR)/common.o -o $(OUTDIR)/generate 

generate_1: $(DATA)/generate.cpp $(COMMON)/common.cpp 
	$(CCC) $(COPT) $(INCOPT) $(DATA)/generate.cpp $(COMMON)/common.cpp -o $(OUTDIR)/generate

sample: $(EXP)/sample.cpp $(COMMON)/*.cpp $(ALGO)/algo_core.cpp $(ALGO)/curve.cpp
	$(CCC) $(COPT) $(INCOPT) $(EXP)/sample.cpp $(COMMON)/*.cpp $(ALGO)/algo_core.cpp $(ALGO)/curve.cpp -o $(OUTDIR)/sample

DetSeq_sample: $(EXP)/DetSeq_sample.cpp $(COMMON)/*.cpp $(ALGO)/algo_core.cpp $(ALGO)/curve.cpp
	$(CCC) $(COPT) $(INCOPT) $(EXP)/DetSeq_sample.cpp $(COMMON)/*.cpp $(ALGO)/algo_core.cpp $(ALGO)/curve.cpp -o $(OUTDIR)/DetSeq_sample

test_allocation: $(EXP)/test_allocation.cpp $(COMMON)/*.cpp $(ALGO)/algo_core.cpp $(ALGO)/curve.cpp
	$(CCC) $(COPT) $(INCOPT) $(EXP)/test_allocation.cpp $(COMMON)/*.cpp $(ALGO)/algo_core.cpp $(ALGO)/curve.cpp -o $(OUTDIR)/test_allocation

generate_single: $(EXP)/generate_single_entry.cpp $(DATA)/generate_single.cpp $(COMMON)/common.cpp 
	$(CCC) $(COPT) $(INCOPT) $(EXP)/generate_single_entry.cpp $(DATA)/generate_single.cpp $(COMMON)/common.cpp -o $(OUTDIR)/generate_single_entry

single_entry: $(EXP)/cplex_single_entry.cpp $(COMMON)/*.cpp $(ALGO)/algo_core.cpp $(ALGO)/curve.cpp
	$(CCC) $(CPCOPT) $(CPINCOPT) $(CPLOPT) $(CPLIBOPT) $(EXP)/cplex_single_entry.cpp $(COMMON)/*.cpp $(ALGO)/algo_core.cpp $(ALGO)/curve.cpp -o $(OUTDIR)/single_entry

algo_single_entry: $(EXP)/algo_single_entry.cpp $(COMMON)/*.cpp $(ALGO)/algo_core.cpp $(ALGO)/curve.cpp
	$(CCC) $(CPCOPT) $(CPINCOPT) $(CPLOPT) $(CPLIBOPT) $(EXP)/algo_single_entry.cpp $(COMMON)/*.cpp $(ALGO)/algo_core.cpp $(ALGO)/curve.cpp -o $(OUTDIR)/algo_single_entry