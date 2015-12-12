int i;
int j;
int n;
int t;

$in r1, =kbd
$store r1, =n
$table ds 30000
$load fp, =1

for (i = 2; i <= n; i = i+1) {
	$load r5, i
	$load r4, table(r5)
	$store r4, =t
	if (t == 1) {
		$jump =label
	}
	for (j = i*i; j <= n; j = j+i) {
		$load r3, j
		$store fp, =table(r3)
	}
	$label nop
}

for (i = 2; i <= n; i = i+1) {
	$load fp, i
	$load r3, table(fp)
	$store r3, =t
	if (t == 0) {
		$out fp, =crt
	}
}
