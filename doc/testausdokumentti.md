# testausdokumentti

## leksikaalinen analyysi
Leksikaalisen analyysin eli skannauksen testaaminen toteutettiin luomalla pienehköjä
testitiedostoja, jotka sisältävät halutun ohjelmakoodin. Ohjelmakoodi ajetaan skannerin
läpi, jolloin se tuottaa token listin. Tämä lista tulostetaan terminaaliin, jolloin
nähdään ohjelmakoodi muodossa

token_value token_id

token_value token_id

jne...

esimerkiksi

int 4

muuttuja 1

; 13

Koska tokenien id arvot ovat suoraan tiedossa (kovakoodattu tiedostoon src/lex.h) on
tästä tulosteesta nyt mahdollista tarkastaa että skanneri on tuottanut oikeanlaisen
sekvenssin.

Koska käsin testaaminen on työlästä sekä virhealtista, on testaukseen luotu pienehkö
bash-skripti joka hoitaa sen käyttäjän puolesta. hakemistossa test/lex on skripti sekä
kaikki sen toimintaan vaikuttavat tiedostot. Skripti toimii siten, että samassa
hakemistossa on konfiguraatiotiedosto test.cfg. Tässä tiedostossa joka rivillä on yksi
pari: (testattava tiedosto) (odotettu sekvenssi). (testattavia tiedostoja etsitään
oletusarvoisesti hakemistosta units/). Testiskripti suorittaa skannauksen units/
hakemistossa oleville testeille ja vertaa niiden tulostamaa token sekvenssiä odotettuun.
Skripti myös ilmoittaa mitä testataan ja menikö testi läpi. Jos ei mennyt niin odotettu
sekä saatu sekvenssi ilmoitetaan terminaaliin.

Testeissä on testattu seuraavat tapaukset:
 * tiedosto joka sisältää vian yhden tokenin (kaikki tokenit käyty läpi)
 * tiedosto joka sisältää 3 tokenia joista ensimmäinen ja viimeinen ovat samat. (testattu kaikille tokeneille)
 * muutama hieman isompi "random" tiedosto testattu.
 * Myös testit erityyppisille kommenteille

Tämän perusteella rajatapaukset ovat melko hyvin testattu. Testit on tehty nimenomaan
tokeneittain, mutta esim jokaista operaattoria ei ole erikseen toistaiseksi testattu.
(syntaksianalyysin testeissä on testattu myös jokainen operaattori ja mahdollisuus)
esim. TOKEN_COMPOP testi testaa vertailussa käytettäviä operaattoreita, mutta kaikkia
== != < <= > >= niitä ei ole testattu. Toisaalta kaikki operaattorit on testattu vähintään
kerran jossain testauksen vaiheessa.

Viikolla 4 lisättiin ohjelmaan TOKEN_EOF joka siis ilmoittaa token virran päättymisestä.
Jotta testejä ei olisi tarvinnut muuttaa, tämä token lisätään implisiittisesti jokaisen
testin odotettuun tokenarvoon.

## syntaksianalyysi
Syntaksianalyysin eli parserin testaaminen hoidetaan hieman samalla tavalla kuin
skannaamisenkin testaaminen. Hakemistossa tests/parser on skripti, joka suorittaa
units/hakemistossa oleville testitiedostoille syntaksitarkastuksen. Kuten skannerin
tapauksessa, myös tässä skriptissä on test.cfg tiedosto jossa löytyy odotettu tulos.

Koska parseri kertoo vain onko tiedosto syntaksin mukainen, se palauttaa
ympäristömuuttujalle $? arvon 0 tai 1. 0 tarkoittaa syntaksin olevan sopimaton
määritellyn kieliopin kanssa ja 1 tarkoittaa syntaksin olevan kunnossa. Kuten skannerin
testiskriptissä, myös tämä kertoo testin epäonnistuessa tiedoston nimen ja mitä arvoa
odotettiin.

Testejä on tehty käytännössä kaikkiin ei mahdollisuuksiin, niin että ne kattavat
myös kaikki eri vertailuoperaattorit sekä aritmeettiset operaattorit. Myös tyhjiä
lauseita on testattu erikseen sekä toisto ja ehtolauseiden kanssa.

## semanttinen analyysi
Semanttinen analyysi testataan käytännössä täysin samalla tavalla kuin syntaktinen analyysi.
Erona näiden kahden välillä on käytännössä ohjelman main funktio. Semantiikan tarkastuksen
main funktio ajaa vielä parserilta saadun syntaksipuun checkSemantics funktion läpi.
Muutoin kaikki toimii samalla tavalla eli tulos 0 tai 1 palautetaan suorituksen
ympäristömuuttujalle ja skripti lukee tästä arvoja ja vertaa odotettuun.

Testattu on kaikkia rajatapauksia, mm.
 * määrittämättömän muuttujan käyttö
 * oikein määritellyn muuttujan käyttö
 * eri tietotyyppien käyttö samassa lauseessa
 * eri tietotyyppien käyttö toisistaan erillään
 * muuttujan määrittely useampaan kertaan
 * eri tietotyyppien käyttö vertailulauseessa

Kaikkia edellä mainittuja tapauksia on testattu useammin kuin kerran.

## koodin generointi
Koodin generointia eli lopullista ohjelman tulostetta on testattu vain manuaalisesti.
En tiedä kannattaako tähän tehdä automaattisia testejä, koska se edellyttäisi
ttk-15 binäärien liittämistä projektiin. Manuaaliset testit ovat kuitenkin kaikki
tuottaneet odotetun lopputuloksen

## testien ajaminen

Kaikki testit voidaan ajaa suoraan testiskriptistä hakemistoissa tests/asd/test.sh
missä asd on joko "lex", "parser" tai "semantics". Testeille on kuitenkin tehty oma makefile
tiedosto, joka löytyy tests/ hakemiston juuresta. Tätä makefileä (tai test.sh tietostoja) ei kuitenkaan ole
suunniteltu suoritettavaksi sellaisenaan vaan oikea tapa suorittaa testit on suorittaa
projektin juuressa oleva makefile parametrillä "tests". Jos testis suorittaa jollakin
muulla tavalla, ei testien käyttämät binäärit välttämättä ole käännetty tai
suorituskelpoisia. Testit siis suoritetaan projektin juurihakemistossa komennolla

make tests

## muuta testeihin liittyvää

Testit käyttävät tällä hetkellä testitiedoston nimen parseamiseen grep komentoa.
Tämän takia testitiedostojen nimet eivät voi olla toistensa osamerkkijonoja!
Esim. grep testi.c palauttaisi testi.c rivin lisäksi eka_testi.c rivin, jos sellainen
mahdollisesti olisi.

Tähän tulee ehkä muutos jossain vaiheessa.

Testien toiminnan ymmärtämiseksi kannattaa myös lukea testiskriptit ja niiden kommentit.

Suoritin myös valgrindin mem-leak testin ohjelmalle. Tällä hetkellä onnistunut suoritus
ei aiheuta miknäänlaista mahdollisuutta muistivuotoihin.



