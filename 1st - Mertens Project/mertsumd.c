#include <stdio.h>

#define MAXEXP 7

int main(void) {
  int n = 0, k = 0, i = 1, t = 1, p = 1, temp, factor, ZP = 0, term, sum, countde, countab, span = 1000;
  char v = 1;
  char quantity = 0;
  char q;
  char next;
  for (q = 1; q <= MAXEXP; q++) {
    p *= 10;
  }
  p += 9; // FIND SPAN OF nE[1,(10^MAXEXP)+9]
  for (; v <= MAXEXP; v++) { // THE START OF MAIN LOOP
    t *= 10;
    i *= 10;
    t -= 9;
    i += 9; // FIND SPAN OF nE[(10^K)-9,(10^K)+9] TO PRINT MERTENS FUNCTION
    for (; n <= i; n++) { // THE START OF N COUNT
      if (n % 4 == 0 || n % 8 == 0 || n % 9 == 0) { // CHECKS IF N ISN'T ET
        if (n > 0 && k == 0) {
          ZP++; // COUNT THAT CALCULATES HOW MANY ZERO POINTS ARE IN MERTENS
        }
        for (; n >= t && n <= i;) {
          printf("\nM(%d) = %d", n, k); // PRINTS MERTENS IF nE[(10^K)-9,(10^K)+9]
          break;
        }
        continue;
      }
      temp = n; // LOCAL VARIABLE THAT TAKES THE VALUE OF N
      factor = 2; // SET DIVISOR TO 2 WHICH IS THE FIRST PRIME NUMBER
      next = 0; // INITIALLY CONSIDER N ET UNLESS PROVED OTHERWISE
      quantity = 0; // SET QUANTITY OF PRIME FACTORS TO 0
        while (factor * factor <= temp){ // WHILE DIVISOR^2 IS <= THAN N
        	while (temp % factor == 0){ // IF THE SAME NUMBER IS DIVISED BY THE SAME PRIME FACTOR TWICE, IT IS NOT ET
        		temp /= factor;
        		quantity++; 
				if (temp % factor == 0){
				next = 1; // HERE THE PROGRAM CONCLUDES THAT N IS NOT ET THEREFORE THE LOOP BREAKS
				break;
				}
			}
        	if (factor == 2){ // IF DIVISOR WAS PRIME NUMBER 2 MAKE IT PRIME NUMBER 3 (DUE TO PRIME NUMBER ORDER)
        		factor = 3;
			}
			else if (factor == 3) { // IF DIVISOR WAS PRIME NUMBER 3 MAKE IT PRIME NUMBER 5 (DUE TO PRIME NUMBER ORDER)
			factor = 5;
			}
			else if (factor % 6 == 5){ // IF REMAINDER OF DIVISOR WITH 6 IS 5 ADD 2 TO DIVISOR (DUE TO PRIME NUMBER ORDER)
				factor += 2;
			}
			else {
				factor += 4; // OTHERWISE ADD 4 TO DIVISOR (DUE TO PRIME NUMBER ORDER)
			}
		}
		if (temp != 1) { // IF N ISN'T 1 IT HAS AT LEAST ONE PRIME FACTOR THEREFORE ADD ONE PRIME FACTOR TO THE PRIME FACTOR COUNT
			quantity++;}
		if (next == 0){ // IF PROGRAM HAS CONCLUDED THAT N IS ET 
        if (quantity % 2 == 1) { // IF A NUMBER IS EVENLY DIVISIBLE BY 2 WITH REMAINDER = 1, THEN IT IS ODD AND IN OUR CASE IT IS ODD SO WE NEED TO SUBTRACT 1 FROM MERTENS SUM BECAUSE PRIME FACTOR SUM IS AN ODD NUMBER
                k -= 1;
				}
        else {
                k += 1; // IF A NUMBER IS EVENLY DIVISIBLE BY 2 WITH NO REMAINDER, THEN IT IS EVEN AND IN OUR CASE IT IS EVEN SO WE NEED TO ADD 1 TO MERTENS SUM BECAUSE PRIME FACTOR SUM IS AN EVEN NUMBER
                } 
		}
      if (k == 0) { // IF K (MERTENS SUM) ENDS UP BEING 0 ADD TO ZERO POINT SUM
        ZP++;
      }
      for (; n >= t && n <= i;) {  
        printf("\nM(%d) = %d", n, k);
        break;
      }
    }
    if (i == p) { // IF WE HAVE REACHED THE SUPREMUM OF N ((10^MAXEXP)+9) LOOP CLOSES
      break;
    }
    t += 9; // ADD 9 TO (10^K)-9 IN ORDER TO GET THE RIGHT NUMBER SPAN IN NEXT ITERATION
    i -= 9; // SUBTRACT 9 FROM (10^K)+9 IN ORDER TO GET THE RIGHT NUMBER SPAN IN NEXT ITERATION
    printf("\n..........");
  }
  printf("\n\nFound %d zero points of the mertens function\n", ZP); // PRINTS ZERO POINTS OF MERTENS
  span *= ZP; // CALCULATES THE NUMBER SPAN [2, 1000*ZP]
  countde = 0; // SETS DEFICIENT NUMBER COUNT TO 0
  countab = 0; // SETS ABUNDANT NUMBER COUNT TO 0
  printf("\nChecking numbers in the range [%d,%d]\n", 2, span);
  for (n = 2; n <= span; n++) { // START OF THE PROCESS OF CALCULATION OF PERFECT, DEFICIENT AND ABUNDANT NUMBERS IN DEFINED NUMBER SPAN
    sum = 1; // SUM OF FACTORS OF N THAT WILL MULTIPLY EVERY TIME WITH SUM OF PRIME FACTORS OF N FROM 1 TO THEIR BIGGEST EXPONENTIAL PLUS 1
    temp = n; // LOCAL VARIABLE THAT TAKES THE VALUE OF N
    factor = 2; // SET DIVISOR TO 2 WHICH IS THE FIRST PRIME NUMBER
    while (factor * factor <= temp) { // WHILE DIVISOR^2 IS <= THAN N
      term = 1;
      while (temp % factor == 0) {
        term = factor * term + 1; // TERM IS LOCAL VARIABLE USED TO CALCULATE THE MULTIPLICATION OF THE PREVIOUS TERM WITH PRIME FACTOR FOUND, PLUS 1
        temp /= factor;
      }
      sum *= term; // SUM IS UPDATED EVERYTIME THE LOOPS RUNS
      if (factor == 2) { // THE SAME PROCESS OF FACTORIZATION OF N AND FINDING ITS PRIME FACTORS AS USED ABOVE
        factor = 3;
      } else if (factor == 3) {
        factor = 5;
      } else if (factor % 6 == 5) {
        factor += 2;
      } else {
        factor += 4;
      }
    }
    if (temp != 1) { // IF N ISN'T 1 SUM IS LAST PRIME FACTOR OF N IN FIRST EXPONENT
      sum *= (1 + temp); // LAST PRIME FACTOR OF N IN FIRST EXPONENTIAL THEREFORE IT HAS TO BE MULTIPLIED 
    }
    if (sum - n == n) { // PRINTS PERFECT NUMBER WHEN FOUND
      printf("\nFound perfect number: %d", n);
    }
    if (sum - n < n) { // ADDS 1 TO DEFICIENT NUMBER COUNT
      countde++;
    }
    if (sum - n > n) { // ADDS 1 TO ABUNDANT NUMBER COUNT
      countab++;
    }
  }
  printf("\n\nFound %d of deficient numbers.", countde); // PRINTS NUMBER OF DEFICIENT NUMBERS FOUND
  printf("\nFound %d of abundant numbers.\n", countab); // PRINTS NUMBER OF ABUNDANT NUMBERS FOUND
}
