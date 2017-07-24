procs:
	proc square(int num)->(int sqr):
		assign sqr = num * num;
		assign sqr = sqr * times;
	endproc
endprocs

execute:
	int num, times, n, res;
	assign num =10;
	assign times =2;
	assign n =20;
	
	call square(n)->(res);
	put("Result "); 
	put(res); 
	put(endl);
end
