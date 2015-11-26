#toteutusdokumentti#

##ohjelman rakenne##
Ohjelma koostuu kolmesta osasta: Leksikaalisesta, syntaktisesta ja semanttisesta analyysistä.
Kaikki yksiköt ovat omissa .c tiedostoissa ja niillä on oma header tiedosto.

Julkisia funktioita on vain kolme. Yksi jokaiselle .c tiedostolle
Muilta osin funktiot ovat saattisia.

###skanneri###
Leksikaalinen analyysi eli skanneri tuottaa yhteen suuntaan linkitetyn listan ns.
tokeneita, jotka vastaavat kontekstittoman kielen terminaaleja. Tämä tapahtuu hieman
samaan tapaan kuin säännöllisten lausekkeiden tunnistaminen. Poikkeuksena kuitenkin se,
että skanneri etsii aina pisimmän mahdollisen tokenin. Näin ollen pelkästään
säännöllisiä lausekkeita käyttämällä skannaus olisi hitaampaa.

###parseri###
Syntaktinen analyysi eli parseaminen tapahtuu käymällä skannerin palauttama lista
läpi. Parseri on niin sanottu recursive descen parser eli se käy token listan läpi
muodostaen samalla syntaksipuuta juuresta lehtiin, vasemmalta
oikealle. Lisäksi parseri on niin sanottu ennustava parseri. Tämän (ja kieliopin
LL(1) yhteensopivuuden) ansiosta token listaan ei tarvita minkäänlaista backtrack
systeemiä ja syntaksin tarkastus toimii lineaarisessa ajassa suhteessa syötteen kokoon.

Jokaisella kieliopin nonterminaalilla on siis parserissa oma funktio. Funktio tietää
vain seuraavaa tokenia katsomalla onko syntaksi mahdollisesti kieliopin mukainen.
Jos on, siirtyy kontrolli seuraavalle funktiolle kieliopin sääntöjen mukaan, jos ei
lopetetaan ohjelman suoritus ja palautetaan NULL pääohjelmalle. Onnistuneessa
suorituksessa palautetaan osoite syntaksipuun juureen.

###semanttinen analyysi###
Skannauksen ja parseamisen jälkeen suoritetaan vielä semanttinen analyysi.
(kunhan edelliset vaiheet eivät aiheuttaneet virhettä.) semanttinen analyysi varmistaa,
että ohjelma on mahdollista muuttaa konekieliseksi koodiksi. Tämä vaihe aiheuttaa
virheen jos käyttäjä yrittää käytää määrittelemätöntä muuttujaa, tai jos se koittaa
määritellä saman muuttujan useaan kertaan. Virhe aiheutuu myös jos yritetään operoida
yhteen eri tyyppisiä muuttujia ja/tai literaaleja.

##aika- ja tilavaativuudet##
Ohjelman aika ja tilavaativuus on lineaarinen suhteessa syötteen kokoon. Tässä on
huomioitavaa että syötteen koolla tarkoitetaan nimenomaan lähdetiedostossa olevien
tokenien eli kielen terminaalien määrää, ei tiedoston kokoa. Käytännössä nämä kaksi
kuitenkin korreloivat keskimäärin hyvin vahvasti keskenään.

##muuta##
Ennenkuin joku ihmettelee miksi syntaksi käydään rekursiivisesti läpi kaksi kertaa:
Semanttisen analyysin olisi voinut suorittaa samalla rekursiokerralla syntaksin tarkastamisen
yhteydessä. Tämä olisi kuitenkin epäselkeyttänyt koodia huomattavasti, joten päätin
suorittaa läpikäynnin jokaisessa osassa erikseen. Tällöin funktiot pysyvät lyhyempinä
ja helppolukuisempina. Vaikka aikavaativuuden kerroin kasvaa, sen kokoluokka pysyy
silti samana, joten tämä ratkaisu ei merkittävästi hidasta ohjelman suoritusta.

Mahdollinen koodin generointi tehdään kuitenkin toden näköisesti samaan yhteyteen
semantiikan tarkistuksen kanssa. 

####tila####
Koska parseaminen tapahtuu rekursiivisesti, ja muistissa on kerrallaan vain yksi haara
juuresta lehteen, toimii parseri oikeastaan logaritmisesti muistivaativuuden suhteen.
Tästä huolimatta pullonkaulana on token lista (ja viikosta 5 eteenpäin myös
abstrakti syntaksipuu), joka sisältää koko lähdetiedoston
tokeneiksi muutettuina (ja kommentit poistettuina). Koska token lista on
kokonaisuudessaan muistissa ja se tuhotaan vasta ohjelman loputtua, tulee muistin
vaatimukseksi O(n + log(n)) = O(n)

####aika####
Kaikki vaiheet ovat luokassa O(n) sillä joka vaiheessa syöte luetaan vain kertaalleen läpi.
Tämä on varmasti itsestään selvää skannauksessa sekä semanttisessa analyysissä. Myös parseaminen
on O(n) sillä siinä ei ole minkäänlaista backtrack järjestelmää, eli se osaa sanoa aina
seuraavan terminaalin(tokenin) kohdalla voiko syöte mahdollisesti toteuttaa kielen syntaksin.

##puutteet parannusehdotukset##
Tässä vaiheessa ohjelma käytännössä tekee sen mitä alun määrittelydokumentissa
sanottiin. Lisäksi suoritetaan semanttinen tarkastus. Seuraavilla viikoilla joko
laajennetaan kieltä tai toteutetaan koodin generointi.

Parserin koodi on mielestäni melko selkeää ja toimivaa eikä sisällä sen pahempia
puutteita. Skanneria puolestaan voisi heieman refaktoroida ja muuttaa ainakin
lukuoperaatiot joko binääriseen käsittelyyn tai merkkipohjaiseen käsittelyyn.
(tällä hetkellä skanneri käyttää sekaisinsekä freadia että fgetchiä, mikä on ehkä
hieman hassua.)

##lähteet##
 * Eniten käytettynä lähteenä Michael L. Scott - Programming Language Pragmatics - Morgan Caufmann (2009)
 * Juha Vihavaisen kalvot kääntäjä kurssilta 2014 (näyttää olevan aika pitkälti referointia ylemmästä lähteestä)
 * Laskennan mallit kurssin pruju Jyrki Kivinen (2011)
 * Myös tällaista vilkaisin: Let's Build a Compiler - Jack Crenshaw tutoriaali osoitteessa http://www.compilers.iecc.com/crenshaw/
 * Luin myös hieman ohjelmien lex ja yacc dokumentointia