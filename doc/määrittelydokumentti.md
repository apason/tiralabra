#Määrittelydokumentti#

##ohjelman tarkoitus##
Tarkoitus toteuttaa ohjelma joka suorittaa syntaksitarkastuksen eräälle c-kielen osajoukolle. Tämän kielen
määritelmä löytyy tiedostosta "kielen_kuvaus.md". Jos aikaa jää kurssin lopussa, saatetaan kielen kuvausta
hieman laajentaa.

Syntaksin tarkastus muodostuu kahdesta osasta: Leksikaalisesta ja syntaktisesta analyysistä. Leksikaalinen 
analyysi toteutetaan säännöllisten lausekkeiden avulla jakamalla syöte pienempiin ohjelmointikielen 
tunnistamiin yksiköihin. Tämän jälkeen syntaktinen analyysi tarkistaa syntaksin oikeellisuuden ja rakentaa 
abstraktin syntaksipuun josta edelleen saadaan generoitua ohjelmakoodia (jota ei tällä kurssilla 
todennäköisesti tehdä)

Keskeisiä tietorakenteita ovat ainakin linkitetyt listat sekä abstrakti syntaksipuu. Tavoitteena on käsitellä 
tietorakenteita niin nopeasti kuin se teoreettisesti on mahdollista. Muistin suhteen tavoite on maltillisempi: 
Ohjelma toteutetaan todennäköisesti niin, että yhden käännösyksikön ns. token stream (eli leksikaalisen 
analyysin tulos) ja syntaksipuu ovat kokonaisuudessaan kerralla muistissa.

Lopullinen ohjelma siis saa syötteekseen jonkin kooditiedoston. Tulosteena ohjelma kertoo hyväksyttiinkö 
kyseinen koodi eli oliko syntaksin mukainen. Jos oli, tulostetaan mahdollisesti vielä syntaksipuu näytölle.
Jos aikaa jää saatetaan käännös suorittaa ttk-15 arkkitehtuurille.




