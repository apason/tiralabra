/*
 * "c" ohjelma pyytää käyttäjältä syötteenä kokonaisluvun ja 
 * laskee fibonaccin sarjan tähän lukuun asti tulostaen
 * kaikki tulokset välillä [0, n]
 */
 
int i;
int j;
int k;
int t;
int n;
int h;

/*
 * tällä käytetty $<assembly> notaatiota, jolloin
 * kääntäjä siirtää $ merkin jälkeisen loppurivin
 * kooditiedostoon sellaisenaan
 *
 * syötteen luku ja tulostus pitää siis hoitaa tällä
 * tavalla koska funktioita ei ole implementoitu
 */
$in r1, =KBD
$store r1, =n

for(h = 0; h <= n; h = h+ 1){
	i = 0;
	j = 1;
	for(k = 1; k <= h; k = k+1){
		t = i+j;
		i = j;
		j = t;
	}
	$load r1, j
	$out r1, =CRT
}
// Ohjelma päättyy ----------------------------------------------------------------------------------------

