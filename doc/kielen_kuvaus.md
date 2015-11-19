#kielen kuvaus#

En ota heti alkuun tavoitteeksi koko ansi c kielen tarkastusta vaan olen päätynyt seuraavaan osajoukkoon:
 * tietotyypit int ja float
 * muuttujien määrittely ja alustus. esim. "int i"
 * laskulausekkeet ja sijoitus matemaattisille perusoperaatioille + - * / esim. "i = 10*(a+b)"
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
 * iwKey varatut sanat if ja while
 * forkey varattu sana for
 * typeKey varatut sanat int, float
 * muuttujat: [a-z][a-z,0-9,_-]*
 * literaaleja: kokonaisluvut [0-9]* ja liukuluvut [0-9]*.[0-9]*
 * virhetokeneita: kaikki merkkijonot mitkä eivät ole yllä mainittuja tokeneita
   tuottavat virhetokenin.
 * lauseen päättävä merkkijono ;

#skannaaminen#
HUOM! tämä osuus ei ole enää voimassa, sillä kielioppia on päivitetty!

tokenit saadaan palautettua hyvin yksinkertaisesti seuraavalla algoritmilla:
lue merkki
jos merkki on
 * jokin yhden merkin operaattori (pois lukien /, <, > ja =) niin luo ja palauta token
 * kirjain
   * lue seuraava merkki kunnes se ei ole kirjain, numero tai alaviiva
   * vertaa merkkijonoa varattuihin sanoihin. jos se on varattu sana luo ja palauta key word token
   * muuten palauta muuttuja token
 * numero
   * lue seuraava merkki niin kauan kun se on numero
   * jos merkki on . niin lue seuraavia merkkejä niin kauan kun ne ovat numeroita ja palauta liukulukuliteraali
   * jos merkki ei ole piste niin palauta kokonaislukuliteraali
 * /
   * lue seuraava merkki
   * jos se on / niin lue kunnes \n ja lopeta
   * jos se on * niin lue kunnes löytyy sekvenssi "*/" ja lopeta
   * muuten palauta / operaattoritoken.
 * <
   * lue seuraava merkki
   * jos merkki on = niin palauta <= vertailu token, muuten palauta <
 * >
   * lue seuraava merkki
   * jos merkki on = niin palauta >= vertailutoken, muuten palauta >
 * =
   * lue seuraava merkki
   * jos merkki on = niin palauta == vertailutoken, muuten = sijoitustoken

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
 * if              = if ( expression compop expression ) statement
 * while           = while ( expression compop expression ) statement
 * for             = for (assignment expression compop expression ; id = expression) statement
 * declaration     = type id ; 
 * assignment      = id = expression ; | ;

tyhjän laiuseen mahdollisuus vain assgnmenttiin?!
deklaraatioon vois laittaa tyylii type id dectail ... dectail =  = expression | eps
