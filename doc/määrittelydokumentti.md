#Määrittelydokumentti#

##ohjelman tarkoitus##
Tarkoitus toteuttaa ohjelma joka suorittaa syntaksitarkastuksen eräälle c-kielen osajoukolle. Tämän kielen
määritelmä löytyy tiedostosta "kielen_kuvaus.md". Jos aikaa jää kurssin lopussa, saatetaan kielen kuvausta
hieman laajentaa. (edit. kielen kuvausta muutettu muutamaan kertaan kurssin edetessä)

Syntaksin tarkastus muodostuu kahdesta osasta: Leksikaalisesta ja syntaktisesta analyysistä. Leksikaalinen 
analyysi toteutetaan säännöllisten lausekkeiden avulla jakamalla syöte pienempiin ohjelmointikielen 
tunnistamiin yksiköihin. Tämän jälkeen syntaktinen analyysi tarkistaa syntaksin oikeellisuuden ja rakentaa 
abstraktin syntaksipuun josta edelleen saadaan generoitua ohjelmakoodia (jota ei tällä kurssilla 
todennäköisesti tehdä edit. tehtiin kuitenkin)

Keskeisiä tietorakenteita ovat ainakin linkitetyt listat sekä abstrakti syntaksipuu. Tavoitteena on käsitellä 
tietorakenteita niin nopeasti kuin se teoreettisesti on mahdollista. Muistin suhteen tavoite on maltillisempi: 
Ohjelma toteutetaan todennäköisesti niin, että yhden käännösyksikön ns. token stream (eli leksikaalisen 
analyysin tulos) ja syntaksipuu ovat kokonaisuudessaan kerralla muistissa. Näin ohjelmakoodi pysyy kompaktina
ja selkeämpänä suuremman muistinkulutuksen kustannuksella.

Lopullinen ohjelma siis saa syötteekseen jonkin kooditiedoston. Tulosteena ohjelma kertoo hyväksyttiinkö 
kyseinen koodi eli oliko syntaksin mukainen. Jos oli, tulostetaan mahdollisesti vielä syntaksipuu näytölle.
Jos aikaa jää saatetaan käännös suorittaa ttk-15 arkkitehtuurille (edit. koodin generointi tehty).


##päivitys##
Koska syntaksitarkastus oli käytännössä valmis jo neljännellä viikolla, otin tavoitteeksi myös syntaksipuun
rakentamisen eksplisiittisesti. Lisäksi suoritin semanttisen analyysin sekä koodin generoinnin.

Semanttisessa analyysissä on tarkoitus tarkistaa kielen semantiikkaan liittyvät ongelmat. Vaikka syöte olisi
syntaktisesti laillinen, siinä saattaa silti olla osia jotka eivät käy semanttisesti järkeen. Tällaisia
on c-kielessä mm. alustamattomien muuttujien käyttäminen sekä muuttujien määritteleminen useampaan kertaan.

Toisin kuin c:ssä ei tämän projektin ohjelmointikieli tue minkäänlaisia tyypinmuunnoksia (koska ttk-15
arkkitehtuurissa ei ole konekäskyä tähän tarkoitukseen enkä jaksanut sitä konekielellä toteuttamaa).
Tämän takia semanttinen analyysi sisältää myös tyyppitarkistukset. Kokonaislukuja ja liukulukuja ei
siis voi laskea millään tavalla yhteen. Myöskään vertailulauseet ei onnistu keskenään erityyppisillä
luvuilla (siis esim 5 > 1.5). 

Koodin generointi suoritetaan ttk-15 arkkitehtuurille. Tämä tapahtuu syntaksipuuta läpikäyden samalla
kerralla kuin semantiikka tarkistetaan.

Koska ohjelma sisältää syntaksin ja semantiikan tarkastuksen lisäksi myös koodin generoinnin, on
projekti oikeastaan kokonainen ohjelmointikieli ja sille tehty kääntäjä.