#Makefile for ionosphere delay calculation
# By Yangsheng
#

CFLAGS = -Wall -O0 -g
var = test.o preprocess.o read_dcb.o brdc_orb.o coordinate_transformation.o global_variables.o matrix.o read_GMN.o read_GMO.o Timestruct.o
all : iono_calc

iono_calc : $(var)
	g++ $(CFLAGS) -o iono_calc $(var)

%.o : %.cpp
	g++ $(CFLAGS) -c $<

%.o : %.cc
	g++ $(CFLAGS) -c $<

clean:
	rm -f *.o



