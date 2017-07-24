execute:
	real P,R,T;
	assign P = 10.0; 
	assign R = 7.5; 
	assign T = 4.0;

	real I, A;
	call calculateInterest(P, R, T)->(I, A);

	put("Interest "); 
	put(I); 
	put(endl);
	put("Amount "); 
end
