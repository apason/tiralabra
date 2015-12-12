int n;

$in r1, =KBD
$store r1, =n

if ((n/2)*2 == n){
	$load r1, =1
	$jump =end
}
$load r1, =0
$end out r1, =CRT
