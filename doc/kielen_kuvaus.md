#kielen kuvaus#

En ota heti alkuun tavoitteeksi koko ansi c kielen tarkastusta vaan olen päätynyt seuraavaan osajoukkoon:
 * tietotyypit int ja float
 * muuttujien määrittely ja alustus. esim. "int i;"
 * laskulausekkeet ja sijoitus matemaattisille perusoperaatioille + - * / esim. "i = 10*(a+b);"
 * while ja for rakenteetn
 * if lause
 * c:n kommentit. sekä yhden rivin että usean rivin muodossa.


#leksikaalinen analyysi#
leksikaalinen analyysi tuottaa seuraavan laisia tokeneita:
 * lBra operaattori (
 * rBra operaattori )
 * assOp operaattori = 
 * compOp operaattorit: ==, !=, >, <, >=, <=
 * addOp operaattorit: +, -
 * mulOp operaattorit: *, /
 * whileKey varattu sana while
 * ifKey varattu sana if
 * forkey varattu sana for
 * typeKey varatut sanat int, float
 * muuttujat: [a-z][a-z,0-9,_-]*
 * literaaleja: kokonaisluvut [0-9]* ja liukuluvut [0-9]*.[0-9]*
 * virhetokeneita: kaikki merkkijonot mitkä eivät ole yllä mainittuja tokeneita
   tuottavat virhetokenin.
 * lauseen päättävä merkkijono ;

##kieli##
Kyseessä on selvästi yhteydetön eli kontekstiton kieli. (CFL) Formaalisti CFL on
nelikko (T, N, S, P), jossa T on terminaalien joukko, N on epäterminaalien joukko, S
on aloitussymboli ja P on sääntöjoukko, joka määrää mitä mistäkin epäterminaalista
on johdettavissa.

Oletetaan, että lukija ymmärtää säännöllisten kielten merkintätavat.

###T###
Koska parseri ei näe merkkejä vaan pelkkiä tokeneita, on terminaalien joukko sama
kuin leksikaalisen analyysin tuottamien tokenien joukko

###S###
keilen alkusymboli on program

###N ja P###
Kielessä on seuraavat nonterminaalit: (ja niiden säännöt)
 * program         = statement_list | EOF
 * statement_list  = statement statement_list | epsilon       
 * statement       = if | while | for | declaration | assignment
 * expression      = term term_tail
 * term_tail       = addOp term term_tail | epsilon
 * term            = factor factor_tail
 * factor_tail     = mulOp factor factor_tail | epsilon
 * factor          = ( expression ) | identifier | literal
 * if              = if ( comparation ) statement
 * while           = while ( comparation ) statement
 * for             = for (assignment comparation ; id = expression) statement
 * comparation     = expression compop expression
 * declaration     = type id ; 
 * assignment      = id = expression ; | ;
