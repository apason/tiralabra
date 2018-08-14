# viikkoraportti 4

Kielen kuvausta päivitettiin vielä hieman (toivottavasti se nyt on lopullisessa muodossaan). Kielen kuvaukseen lisättiin puolipiste, joten nyt lauseet ovat
oikeaoppisia c:n lauseita. Lisäksi EOF token lisättiin indikoimaan syötteen päättymistä. Lisäksi vertailuoperaattori != lisättiin kieleen koska se oli aiemmin unohtunut.

Myös seuraavat päivitykset tehtiin kielen syntaksiin: Muuttujien deklaraatio tulee tehdä erillään alustuksesta eli esim "int i = 10;" ei kelpaa vaan se pitää kirjoittaa
kahdella lauseella muotoon "int i;" ja "i = 10". Lisäksi if, while ja for rakenteiden ehdoissa on VÄLTTÄMÄTTÄ käytettävä vertailuoperaattoria. Esim tutkittaessa onko
lausekkeen arvo nollasta poikkeava tulee koodi kirjoittaa "if(expression != 0)". c:n toinen tapa "if(expression)" ei siis kelpaa. Muilta osin koodi on täysin yhdenmukainen
c:n kanssa. Nämä poikkeukset tehtiin siksi, että en ainakaan tähän hätään keksinyt miten kielioppi toteutettaisiin muuten LL(1) yhteensopivaksi. 

Syntaksin tarkistus on nyt tehty kieliopin mukaisesti ja se toimii ajassa O(n). Syntaksipuuta ei vielä erikseen tallenneta mihinkään, mutta se tehdään mahdollisesti
seuraavalla viikolla. 

Testejä on päivitetty skannerissa sekä testiskripti ja muutama testi on luotu myös syntaksianalyysille.

Testausdokumenttia ja toteutusdokumenttia on myös kirjoitettu melko pitkälle.

Tällä hetkellä ohjelman suoritus tulostaa ensin token listin jonka jälkeen se tulostaa parserin stack tracen. Se hyväksyttiinkö ohjelma käy ilmi mainin paluuarvosta: Main
palauttaa 1 jos ohjelma oli syntaksin mukainen, muuten 0.
