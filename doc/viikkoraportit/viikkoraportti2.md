#viikkoraportti2#

##koodin valmius##
Leksikaalinen analyysi on nyt toteutettu jollain tasolla. Ohjelma toimii ja
palauttaa listan tokeneista. Error tokeneita ei kuitenkaan vielä konstruoida, vaan
tuntemattomat patternit ignorataan kuten kommentit. Tällä hetkellä toteuttamatta
on vielä seuraavat tokenit:
 * (
 * )
 * {
 * }

Syntaktista analyysiä ei ole vielä aloitettu toteuttamaan

##koodin laatu ja kommentointi##
Koodi on koitettu kommentoida mahdollisimman hyvin. Etenkin "rajapinta" eli mainista
ja muualta ohjelmasta kutsuttavissa olevat funktiot on kommentoitu niin ettei
parametrien tai paluuarvon merkitys jää epäselväksi. Myös staattisia funktioita on
kommentoitu jonkin verran.

Koodi ei ole mitenkään erityisen huonoa mutta silti on mahdollista että käsin
"kovakoodatut" säännöllisten lausekkeiden lukeminen korvataan jollakin muulla
"kirjastomaisemmalla" ratkaisulla.

##makefile##
Projektilla on nyt makefile joka huolehtii käännöstä ja testeistä

projektin juuressa oleva makefile toimii kutsumalla toisissa hakemistoissa olevia
makefilejä. Tällä hetkellä projektissa on kaksi makefileä: koodin kääntämiseen
tarkoitettu tiedosto hakemistossa src/ ja testaukseen tarkoitettu hakemistossa tests/

Näihin ei kuitenkaan tarvitse koskea vaan juurihakemiston makefilellä voidaan kääntää
ja testata seuraavasti.

"make" tai "make project" kääntää projektin. Binääri löytyy hakemistosta target/

"make tests" suorittaa testit jotka löytyvät hakemistosta tests/

##testit##
Testejä on tety jonkun verran. Esimerkiksi rajatapaukset joissa lähdekoodi koostuu
vain yhdestä tokenista on otettu huomioon. Myös koodi joka sisältää molemman
tyyppisiä kommentteja toimivuus on testattu. Testit kattavat myös rajatapaukset
jokaisesta token tyypistä, niin että on testattu että kaikki tokenit voivat olla
sekä ensimmäisiä, että viimeisiä osia lähdekoorissa. Lisäksi yksi hieman "isompi"
lähdekooditiedosto on testattu.

Toistaiseksi kaikki testit menivät läpi.

Lisätietoja testiskriptin toiminnasta voi käydä lukemassa skriptin (tests/lex/test.sh)
kommenteista.

##virhetilanteita/ongelmia##
Jos ohjelma ajetaan ilman syötettä tai tiedostolla jonka koko on 0, se kaatuu.
Tämä ongelma korjaantunee viikon sisällä.

##muuta##
Projektin dokumentointihakemistosta (doc/) löytyy nyt kuvaus tavoitteena olevasta
kielestä. Kuvaus löytyy tiedostosta "kielen_kuvaus.md"

Myös aihemäärittelyä on hieman päivitetty.
