#kielen kuvaus#

En ota heti alkuun tavoitteeksi koko ansi c kielen tarkastusta vaan olen päätynyt seuraavaan osajoukkoon:
 * tietotyypit int ja float
 * muuttujien määrittely ja alustus. esim. "int i" ja "int j = 0"
 * laskulausekkeet ja sijoitus matemaattisille perusoperaatioille + - * / esim. "i = 10*(a+b)"
 * while ja for rakenteetn
 * if lause
 * c:n kommentit. sekä yhden rivin että usean rivin muodossa.

#leksikaalinen analyysi#
leksikaalinen analyysi tuottaa 5 laisia tokeneita:
 * operaattorit: (, ), +, -, *, /, =, {, }, ==, !=, >, <, >=, <=
 * varatut sanat: "int", "float", "while" "for", "if"
 * muuttujat: [a-z][a-z,0-9,_-]*
 * literaaleja: kokonaisluvut [0-9]* ja liukuluvut [0-9]*.[0-9]*
 * virhetokeneita: kaikki merkkijonot mitkä eivät ole yllä mainittuja tokeneita
   tuottavat virhetokenin.

#skannaaminen#
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

