# toteutusdokumentti

## ohjelman rakenne 
Ohjelma koostuu kolmesta osasta: Leksikaalisesta, syntaktisesta ja semanttisesta analyysistä.
Kaikki yksiköt ovat omissa .c tiedostoissa ja niillä on oma header tiedosto. Semanttsen
analyysin yhteydessä suoritetaan koodin generointi, joten tälle ei ole omaa tiedostoa.

Julkisia funktioita on varsinaisesti vain kolme. Yksi jokaiselle .c tiedostolle.
Lisäksi memory.c sisältää julkisia funktioita muistin käsitelyyn liittyen. Muilta osin
funktiot ovat saattisia.

### skanneri
Leksikaalinen analyysi eli skanneri tuottaa yhteen suuntaan linkitetyn listan ns.
tokeneita, jotka vastaavat kontekstittoman kielen terminaaleja. Tämä tapahtuu hieman
samaan tapaan kuin säännöllisten lausekkeiden tunnistaminen. Poikkeuksena kuitenkin se,
että skanneri etsii aina pisimmän mahdollisen tokenin. Näin ollen pelkästään
säännöllisiä lausekkeita käyttämällä skannaus olisi hitaampaa.

Skanneri siis tuottaa yhteen suuntaan linkitetyn listan. (määritelty tiedostossa
src/ tokens.h) Tämä prosessi ei käytännössä voi mennä mitenkään pieleen. Jos syötetiedosto
on tyhjä luodaan silti yhden pituinen lista joka sisältää alkion TOKEN_EOF.
Jos syöte on muuten virheellinen laitetaan listaan jokaista virheellistä sanaa kohti
TOKEN_ERROR.

### parseri
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

### semanttinen analyysi ja koodin generointi
Skannauksen ja parseamisen jälkeen suoritetaan vielä semanttinen analyysi.
(kunhan edelliset vaiheet eivät aiheuttaneet virhettä.) semanttinen analyysi varmistaa,
että ohjelma on mahdollista muuttaa konekieliseksi koodiksi. Tämä vaihe aiheuttaa
virheen jos käyttäjä yrittää käytää määrittelemätöntä muuttujaa, tai jos se koittaa
määritellä saman muuttujan useaan kertaan. Virhe aiheutuu myös jos yritetään operoida
yhteen eri tyyppisiä muuttujia ja/tai literaaleja.

#### tyyppitarkastuksen suoritus
Tyyppitarkastus suoritetaan globaalin
(ja staattisen) muuttujan global_type avulla. Operaatio on melko helppo sillä tiedämme
että tyyppi voi lähtökohtaisesti olla mitä tahansa (global_type = UNDEF). Tietyt
alipuut ovat kuitenkin sellaisia, että koko alipuussa on oltava vain yhden tyyppisiä
lukuja. Tällaiseen alipuuhun (esim. assignment ja expression) mentäessä muutetaan
muuttujan global_type arvo samaksi kuin ensimmäisenä vastaan tulevan muuttujan/literaalin.
Tästä eteenpäin aina uusi muuttuja/literaali kohdattaessa tarkistetaan että sen tyyppi
on sama kuin globaalin muuttujan. Jos tyyppi eroaa niin palautetaan virhe ja suoritus
keskeytetään. Edellämainitun kaltaisesta puusta palattaessa global_type arvoksi muutetaan
taas UNDEF, jolloin seuraava muuttuja seuraavassa tarkastettavassa alipuussa muuttaa
sen arvon taas oletetuksi.

#### kaksinkertainen määrittely ja määrittämättömyys
Määrittämätöntä muuttujaa ei voi käyttää eikä muuttujaa saa määritellä kahdesti. Tämä
on tarkastettu pitämällä semanttisen analyysin edetessä kirjaa (linkitetty lista) kaikista
vastaan tulleista muuttujista. Kun muuttuja tulee vastaan (eikä kyseessä ole muuttujan
määrittelly) tarkistetaan että se löytyy listalta. Jos ei löydy niin suoritus keskeytetään
ja virhe palautetaan. Samalla tavalla jokaisessa muuttujan määrittelyssä tarkastetaan
että listalla ei ole kyseistä muuttujaa.

#### koodin generointi
Semanttisen analyysin yhteydessä suoritetaan myös koodin generointi. Jos semantiikassa
on virhe myös generointi lopetetaan ja jo generoitu kooditiedosto tuhotaan. Koodi
generoidaan käyttämällä tietynlaista templatea tietynlaiselle lauseelle. Esimerkiksi
vertailu (1 == 2) suorittaa ensin koodin generoinnin expressioneille 1 ja 2.
(expression puolestaan kutsuu muita funktioita kunnes päästään nonterminaaliin "term"
ja term_tail. käytetään seuraavaa templatea:
 * load r3, =<value=1>
 * push sp, r3
 * load r3, =<value=2
 * push sp, r3

Nyt molempien expressioiden arvot on pinossa tulevaa laskentaa vartenä
Tämän jälkeen vertailufunktio kutsuu templatea vertailun tarkistamiseen:
 * load r0, =0
 * pop sp, r2
 * pop sp, r1
 * comp r1, r2
 * jequ =_comp0_true
 * jump =_comp0_end
 * _comp0_true load r0, =1
 * _comp0_end nop

Se siis lataa argumentit pinosta ja suorittaa comp konekäskyn argumenteille. Tämän jälkeen
käytetään jequ konekäskyä (koska ==). Jos vertailu tuotti halutun tuloksen (eli on true)
laitetaan rekisterin r0 arvoksi 1, jos se oli väärä sinne jää arvo 0. Koska comparation
funktio kutsutaan aina jonkun ehtolauseen (if, while, for) sisällä voi tarvitsee kyseisen
ehtolauseen sisään vain templaten joka tarkistaa rekisterin r0 arvon ja suorittaa koodin
jos ko. rekisterin arvo on 1. Tällä tavalla kooia muodostetaan rekursiiviseti puuta
läpi käydessä. Selitys ei ehkä ollut paras mahdollinen vaikka toiminta onkin melko
suoraviivaista. Paremmin toimnnan kuitenkin ymmärtää lukemalla tiedoston src/semantics.c

### syntaksipuu
Syntaksipuu on rakenne, jossa jokaisella solmulla on linkit **kaikkiin mahdollisiin**
lehtiin ja alipuihin mitä kyseisestä solmusta voi lähteä. Toisin sanoen solmun linkit
vastaavat nonterminaalin predict joukkoa. Jokainen linkki on alustettu NULL pointteriksi
ja syntaksipuuta rakennettaessa linkit täytetään. On kuitenkin huomattavaa, että kaikki
linkit eivät muutu vaan puuhun jää NULL viitteitä. Nämä viitteet kuitenkin jätetään
huomioimatta semantisessa analyysissä ja koodin generoinnissa.

Syntaksipuun rakenne löytyy tiedostosta src/tree.h

## aika- ja tilavaativuudet
Ohjelman aika ja tilavaativuus on lineaarinen suhteessa syötteen kokoon. Tässä on
huomioitavaa että syötteen koolla tarkoitetaan nimenomaan lähdetiedostossa olevien
tokenien eli kielen terminaalien määrää, ei tiedoston kokoa. Käytännössä nämä kaksi
kuitenkin korreloivat keskimäärin hyvin vahvasti keskenään.

### tila
Koska parseaminen tapahtuu rekursiivisesti, ja muistissa on kerrallaan vain yksi haara
juuresta lehteen, toimii parseri oikeastaan logaritmisesti muistivaativuuden suhteen.
Tästä huolimatta pullonkaulana on token lista (ja viikosta 5 eteenpäin myös
abstrakti syntaksipuu), joka sisältää koko lähdetiedoston
tokeneiksi muutettuina (ja kommentit poistettuina). Koska token lista on
kokonaisuudessaan muistissa ja se tuhotaan vasta ohjelman loputtua, tulee muistin
vaatimukseksi O(n + log(n)) = O(n)

### aika
Kaikki vaiheet ovat luokassa O(n) sillä joka vaiheessa syöte luetaan vain kertaalleen läpi.
Tämä on varmasti itsestään selvää skannauksessa sekä semanttisessa analyysissä. Myös parseaminen
on O(n) sillä siinä ei ole minkäänlaista backtrack järjestelmää, eli se osaa sanoa aina
seuraavan terminaalin(tokenin) kohdalla voiko syöte mahdollisesti toteuttaa kielen syntaksin.

## muuta
Ennenkuin joku ihmettelee miksi puu käydään rekursiivisesti läpi kaksi kertaa:
Semanttisen analyysin olisi voinut suorittaa samalla rekursiokerralla syntaksin tarkastamisen
yhteydessä. Tämä olisi kuitenkin epäselkeyttänyt koodia huomattavasti, joten päätin
suorittaa läpikäynnin jokaisessa osassa erikseen. Tällöin funktiot pysyvät lyhyempinä
ja helppolukuisempina. Vaikka aikavaativuuden kerroin kasvaa, sen kokoluokka pysyy
silti samana, joten tämä ratkaisu ei merkittävästi hidasta ohjelman suoritusta.

Koodin generointi on kuitenkin tethty samaan yhteyteen semantiikan tarkistuksen kanssa.

## puutteet ja parannusehdotukset

### literaalialueen puuttuminen
Vaikka ttk-15 arkkitehtuuri käsittelee lukuja 32 bittisinä, ei c:kääntäjä kuitenkaan kykene kuin 16 bitin
tarkkuuteen. Luvut voivat kyllä kasvaa yli 16 bittisiksi, mutta muuttujia ei voi alustaa kuin 16 bitin
tarkkuudella. Myöskään literaalit jotka vievät yli 16 bittiä eivät ole sallittuja. Tämä johtuu siitä, että
kääntäjä ei luo minkäänlaista literaalialuetta, eikä muuttujia voida alustaa niiden esittelyn yhteydessä.

Esimerkiksi jos muuttuja x esitellään ja alustetaan arvoon 10 seuraavilla käskyillä: int x; x = 10;
on tuotettu konekoodi seuraavan lainen:
 * x dc 0
 * nop
 * load r3, =10
 * push sp, r3
 * pop sp, r3
 * store r3, =x

Tästä nähdään selvästi että sijoituslause x = 10; tuottama koodi käyttää lukua 10 suoraan konekäskyn
osoiteosassa. Koska osoiteosa on vain 16 bittiä, ei tätä suurempia lukuja ole mahdollista käyttää.
Tämän projektin kääntäjä ei kuitenkaan tarkista literaalien kokoja joten liian suurella/pienellä
luvulla operoitaessa ei aiheudu virheilmoitusta. Toisalta ttk-15 projektin kääntäjä kyllä valittaa
liian suurista osoitekentän arvoista.

Tämän ongelman voisi ratkaista tekemällä tarkistukset kaikille literaaleille ja binääriesityksen ollessa
yli 16 bittiä tallentamalla ko, literaalin erilliseen muistipaikkaan ja viittaamalla lukuun sitä kautta.

### tulostus ja syötteen luku
Koska kääntäjän yhteydessä ei ole minkäänlaista standardikirjastoa, ei pelkillä c-kielen ominaisuuksilla
ole mahdollista lukea syötettä tai tulostaa mitään. Tämän asian korjaamiseksi kieleen on lisätty
mahdollisuus inline assembly konekäskyille. $-merkillä alkavat rivit tulkitaan siis suoriksi konekäskyiksi
ja $ merkin jälkeinen osa siirretään suoraan tuotettuun koodiin.

Tämän ominaisuuden avulla on helppo lukea syötettä ja tulostaa tietoja käyttäjälle. esim määriteltyyn
muuttujaan x lukeminen näppäimistöltä saadaan seuraavalla koodilla:

$in r1, =KBD
$store r1, =x

Rajoitteena inline assemblyllä on kuitenkin se, että rivi ei saa olla yli 32 merkkiä pitkä ja jokainen
konekäsky tulee laittaa omalle rivilleen.

Vastaavasti tulostus tapahtuu load ja out konekäskyillä. ttk-15 konekäskyistä lisätietoa ko. projektin
dokumentaatiossa github.com/apason/ttk-15

### muuttujien esittely
Määriteltyyn ohjelmointikieleen jäi pienimuotoinen moka jonka tajusin vasta loppusuoralla, joten se saa
sinne toistaiseksi myös jäädä. Statement nonterminaali sisältää declaration nonterminaalin saman arvoisena
kuin muutkin nonterminaalit. Lisäksi if, while ja for rakenteet on määritelty niin, että ne saavat
suoritettavaksi koodikseen vain nonterminaalin statement. Näinollen on mahdollista kirjoittaa koodia,
jossa muuttujan esittely on ehdollinen. Koska ohjelmointikieli ei sisällä kuin yhden näkyvyysalueen, tulee
myös ehdollisesti määritelty muuttuja välttämättä kaiken muun koodin näkyviin, vaikka ehto ei edes toteutuisi.
Esimerkiksi if(5==4) int muuttuja; luo muuttujan "muuttuja" joka tapauksessa, ja siihen on myös mahdollisuus
viitata vaikkei ehto olekaan tosi. Tämä ei varsinaisesti haittaa mitään, kun sen tietää, mutta "ominaisuus"
on silti hieman hölmö ja epälooginen.

### kielen yleinen suppeus
Toteutettu kieli on lähtökotaisesti erittäin suppea. Lähtökohtaisesti laajentaminen olisi aina mukava
juttu, mutta erityisesti haluaisin mainita seuraavat seikat:

Kielessä ei ole ns. yhdistettyä lausetta
eli {...} rakennetta kuten c kielessä. Tämän vuoksi esim ehtolauseen if ja toistolauseiden while ja for
suoritettava koodirunko voi olla vain yhden lauseen pituinen! Tämä on erittäin suuri puute muuten
toimivaan ohjelmointikieleen. Onneksi tämän toiminnallisuuden toteuttaminen ei ole kovin suuri vaiva
ja teen sen varmaan viimeistään kurssin loputtua.

Ennenaikainen lopettaminen tulisi myös olla mahdollista. Tällä hetkellä ohjelman suoritus loppuu vasta
kun kontrolli päätyy ohjelmakoodin loppuun. Return lauseen toteutus ratkaisisi ongelman ja mahdollistaisi
ehdollisen suorituksen lopetuksen. Tämä on myös todella helppo toteuttaa.

Koska kieli ei tue funktioita - enkä niitä varmaankaan ikinä toteuta - on hyvä vaihtoehtoinen mahdollisuus
toteuttaa kieleen goto lause. Laskentaa voisi jo monipuolistaa huomattavan paljon.

Myös tyypinmuunnokset int->float ja float->int olisivat erittäin tärkeät. Nämä toteutetaankin todennäköisesti
tulevaisuudessa jos ttk-15 käskykanta on laajennettu sisältämään käskyt tyypinmuunnoksiin.

## lähteet
 * Eniten käytettynä lähteenä Michael L. Scott - Programming Language Pragmatics - Morgan Caufmann (2009)
 * Juha Vihavaisen kalvot kääntäjä kurssilta 2014 (näyttää olevan aika pitkälti referointia ylemmästä lähteestä)
 * Laskennan mallit kurssin pruju Jyrki Kivinen (2011) osa "kontekstittomat kielet"
 * Myös tällaista vilkaisin: Let's Build a Compiler - Jack Crenshaw tutoriaali osoitteessa http://www.compilers.iecc.com/crenshaw/
 * Luin myös hieman ohjelmien lex ja yacc dokumentointia
