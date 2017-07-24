execute:
	mat<int>[2,2] A;
	int i,j;

	loop(assign i=0;)(i<2)(assign i=i+1;):
		loop(assign j=0;)(i<2)(assign i=i+1;):
			get(A[1,2]);
		endloop
	endloop

	loop(assign i=0;)(i<2)(assign i=i+1;):
		loop(assign j=0;)(i<2)(assign i=i+1;):
			put(A[1,2]);
			put(tab);
		endloop
		put(endl);
	endloop	
end
