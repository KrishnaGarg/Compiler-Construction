records:
	rec student:
		int rollno;
		int marks;
	endrec
	rec teacher:
		int name;
		int psrn;
	endrec
endrecords
procs:
	proc calculateInterest(real principal, real rate, real time)->(real interest, real amount):
		put("In func");
		assign interest = principal * (rate/100) * time ; 
		assign amount = principal + interest;
	endproc
	proc calculateInterest(real principal, real rate, real time)->(real interest, real amount):
		put("In func");
		assign interest = principal * (rate/100) * time ; 
		assign amount = principal + interest;
	endproc
endprocs
execute:
	int a,b;
	int a;
	assign a = 100;
	assign b = 20;
	strassign z = "a"; 
	loop(assign j=0;)(i<2)(assign i=i+1;):
		char p;
		loop(assign j=0;)(i<2)(assign i=i+1;):
			char p2;
		endloop
	endloop
	call calculateInterest(P, R, T)->(I, A);
	call calculateInterest12(P, R, T)->(I, A);

	if(z>=90):
		int var11;
		int var12;
		put(endl);
	elif (z>=90):
		int var2;
		put(endl);
	elif (z>=90):
		int var3;
		put(endl);
	else:
		int var4;
		put(z);
	endif
	put(endl);
end
