INC=/home/beta/cilk/include/cilk

innerproduct: innerproduct.cpp submit.cpp
	cilk++ -fcilkscreen -o innerproduct innerproduct.cpp submit.cpp -I $(INC) -lcilkutil
clean: 
	rm innerproduct


