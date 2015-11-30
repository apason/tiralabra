Ensimmäisellä viikolla olen luonut githubiin repositorion sekä miettinyt aihemäärittelyn loppuun. Runsaasti 
aikaa on mennyt toteutuksen selvittämiseen eli yleiseen perehtymiseen kääntäjien ohjelmoimisesta ja 
kontekstittomien kielten (CFL) sekä kontekstittomien kielioppien (CFG) opiskeluun. Tämän saralla olenkin 
oppinut paljon, sillä viikon alussa en tiennyt vielä kääntäjien ohjelmoinnista mitään. olen myös tutkinut 
ohjelmia LEX ja YACC sekä koittanut perehtyä siihen miten ko. ohjelmat hoitavat kontekstittomien kielten 
syntaksitarkastukset. Tehtävänannon mukaisesti rekisteröidyin myös labtooliin.

Varsinaista ohjelmointia ei ole vielä aloitettu vaan se aloitetaan seuraavalla viikolla. Varsinainen 
ohjelmointi aloitetaan todennäköisesti leksikaalisesta analyysistä sekä listan implementoinnista, jonka jälkeen 
siirryn toteuttamaan itse parseria.

Epäselväksi minullue jäi vielä aikavaativuus jolla syntaksin tarkastus voidaan tehdä. Tiedetään, 
että jos on olemassa syöte w ja kontekstiton kielioppi G, niin kysymys päteekö: w € L(G) voidaan ratkaista 
CYK-algoritmilla ajassa O(|w|^3). Tämä on kuitenkin ilmeisen hidasta eikä oikeat kääntäjät käytä kyseistä 
menetelmää. Tarkoitus olisi vielä selvittää miten tämä syntaksin tarkistus tehdään tehokkaasti.

Täältä löytyy mahdollisesti projektiini liittyvää lisäinfoa, jonka avulla saan syntaksipuun toteutettua 
tehokkaasti:
http://www.compilers.iecc.com/crenshaw/tutor1.txt


