# viikkoraportti 3

## kielen kuvauksen määritelmä
Koska kielen halutaan olevan LL(1) yhteensopiva, tulee sen kuvausta hieman muuttaa.
Seuraavat muutokset suoritettiin kielen kuvaukseen:
 * Lisätty token tyypit addOp ja mulOp. Näiden avulla lausekkeiden laskeminen oikeassa
 laskujärjestyssopimuksessa mahdollistuu LL(1) parserilla.
 * Kielestä toistaiseksi poistettu yhdistettu lause eli { ja } rakenne.
 * ( ja ) tulevat kumpikin omiksi tokeneikseen
 * vertailuoperaattorit (==, !=, <, >, >=, <=) erotettu omaksi ryhmäksi
 * sijoitusoperaattori = omaksi tokeniksei

tiedetään että kielioppi on LL(1) yhteensopiva jos ja vain jos

predict(A->ßi) leikkaus predict(A->ßj) = {} kaikilla i != j

kun

first(ß)  = { x | ß =>* xʒ, z € T}
follow(B) = { x | S =>* ðBxʒ, x € T U {EOF}}
predict(B->ß) = first(ß) U (if ß =>* eps then follow(B) else {})

Näiltä näkymin kielioppi (tiedostossa kielen_kuvaus.md) on LL(1) yhteensopiva.

tällä viikolla siis:
 * päivitettiin kieltä
 * ohjelmoitiin päivitykset
 * päivitettiin testit
 * päivitettiin yleistä dokumentaatiota
