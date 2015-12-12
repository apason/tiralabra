float avg;
float sum;
float tmp;
int i;

sum = 0.0;

for(i = 0; i < 3; i = i+1){
	$fin r1, =KBD
	$store r1, =tmp
	sum = sum + tmp;
}

avg = sum / 3.0;

$load r1, avg
$fout r1, =CRT


