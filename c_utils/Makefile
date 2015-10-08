all_cc=$(shell ls *.cc)
all_exe=$(all_cc:.cc=)

all : $(all_exe)

% : %.cc *.h
	$(CXX) --std=c++11 $< -o $@ -O3



