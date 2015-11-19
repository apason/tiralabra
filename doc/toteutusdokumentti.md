#toteutusdokumentti#

##ohjelman rakenne##
Ohjelma koostuu kahdesta osasta: Leksikaalisesta sekä syntaktisesta analyysistä.
Molemmat yksiköt ovat omissa .c tiedostoissa ja molemmilla on oma header tiedosto.

Julkisia funktioita on vain kaksi. Yksi leksikaaliselle analyysille ja toinen
syntaktiselle analyysille. Muilta osin funktiot ovat staattisia.

###skanneri###
Leksikaalinen analyysi eli skanneri tuottaa yhteen suuntaan linkitetyn listan ns.
tokeneita, jotka vastaavat kontekstittoman kielen terminaaleja. Tämä tapahtuu hieman
samaan tapaan kuin säännöllisten lausekkeiden tunnistaminen. Poikkeuksena kuitenkin se,
että skanneri etsii aina pisimmän mahdollisen tokenin. Näin ollen pelkästään
säännöllisiä lausekkeita käyttämällä skannaus olisi hitaampaa.

###parseri###
Syntaktinen analyysi eli parseaminen tapahtuu käymällä skannerin palauttama lista
läpi. Parseri on niin sanottu recursive descen parser eli se käy token listan läpi
muodostaen samalla (implisiittisesti) syntaksipuuta juuresta lehtiin, vasemmalta
oikealle. Lisäksi parseri on niin sanottu ennustava parseri. Tämän (ja kieliopin
LL(1) yhteensopivuuden) ansiosta token listaan ei tarvita minkäänlaista backtrack
systeemiä ja syntaksin tarkastus toimii lineaarisessa ajassa suhteessa syötteen kokoon.

Jokaisella kieliopin nonterminaalilla on siis parserissa oma funktio. Funktio tietää
vain seuraavaa tokenia katsomalla onko syntaksi mahdollisesti kieliopin mukainen.
Jos on, siirtyy kontrolli seuraavalle funktiolle kieliopin sääntöjen mukaan, jos ei
lopetetaan ohjelman suoritus ja palautetaan 0 pääohjelmalle.

##aika- ja tilavaativuudet##
Ohjelman aika ja tilavaativuus on lineaarinen suhteessa syötteen kokoon. Tässä on
huomioitavaa että syötteen koolla tarkoitetaan nimenomaan lähdetiedostossa olevien
tokenien eli kielen terminaalien määrää, ei tiedoston kokoa. Käytännössä nämä kaksi
kuitenkin korreloivat keskimäärin hyvin vahvasti keskenään.

####tila####
Koska parseaminen tapahtuu rekursiivisesti, ja muistissa on kerrallaan vain yksi haara
juuresta lehteen, toimii parseri oikeastaan logaritmisesti muistivaativuuden suhteen.
Tästä huolimatta pullonkaulana on token lista, joka sisältää koko lähdetiedoston
tokeneiksi muutettuina (ja kommentit poistettuina). Koska token lista on
kokonaisuudessaan muistissa ja se tuhotaan vasta ohjelman loputtua, tulee muistin
vaatimukseksi O(n + log(n)) = O(n)

####aika####
.....

##puutteet parannusehdotukset##
Tässä vaiheessa ohjelma käytännössä tekee sen mitä alun määrittelydokumentissa
sanottiin. Seuraavilla viikoilla joko laajennetaan kieltä tai toteutetaan semanttinen
tarkastus ja mahdollisesti koodin generointi.

Parserin koodi on mielestäni melko selkeää ja toimivaa eikä sisällä sen pahempia
puutteita. Skanneria puolestaan voisi heieman refaktoroida ja muuttaa ainakin
lukuoperaatiot joko binääriseen käsittelyyn tai merkkipohjaiseen käsittelyyn.
(tällä hetkellä skanneri käyttää sekaisinsekä freadia että fgetchiä, mikä on ehkä
hieman hassua.)

##lähteet##
