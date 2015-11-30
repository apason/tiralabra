#viikkoraportti 6#

Tällä viikolla refaktoroin koodia melko paljon ja kommentoin sen niin hyvin kuin pystyin.
Nyt koodin pitäisi olla melko selkeää jopa ilman kommentteja. Hakemistorakenteen muuttaminen
olisi myös tärkeää sillä kooditiedostoja alkaa olla jo aika monta. Tätä en kuitenkaan tällä
viikolla jaksanut miettiä joten teen sen ehkä vasta ennen palautusta.

Toteutin lisälksi koodin generoinnin ttk-15 arkkitehtuurille. Nyt ohjelma alkaa siis olla
kuten mikä tahansa täysin toimiva kääntäjä. Minkäänlaista koodin optimointia se ei kuitenkaan
tue enkä sellaista edes yritä tehdä. Jos aikaa jää saatan lisätä kieleen vielä lohkorakentaan {..}

Testejä on nyt tehty myös semanttiselle analyysille ja kaikki näyttää olevan kuten pitää.
Onko assareilla mitään ehdotuksia koodin generoinnin testaamiseen? Miten tämän voisi tehdä
järkevästi vai kannattaako ylipäätään koittaa automatisoida tätä vaihetta? Yksi vaihtoehto
olisi tieteknin sisällyttää projektiin ttk-15 binäärit ja testata sillä jotenkin. Tämä on
tosin jo kaukana yksikkötestauksesta mutta voisi olla ihan ok? Olen tällä hetkellä testannut
käännettyä koodia vain manuaalisesti.

Käyttöohje on nyt kutakuinkin valmis. Ainakin niin pitkällä että tämän viikon vertaisarvioija
pystyy ohjelmaa testaamaan ja tutkimaan. Lisäksi toteutus- ja testausdokumenttejä on päivitetty
sisältämään asiat koodin generoinnin ja semanttisen analyysin osalta.

