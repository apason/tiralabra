#käyttöohje#
Koska kääntäjä kääntää projektissa määritellyn ohjelmointikielen lähdekoodit ttk-15 arkkitehtuurille
ei kääntäjällä oikeastaan voi tehdä mitään ilman kyseisen prosessorin emulaattoria. Toki koodin voi
kääntää mutta sitä ei pääse suorittamaan.

Binääritiedosto löytyy oletusarvoisesti paikasta target/tira

##muutama sana ttk-15:a##
ttk-15 on Auvo Häkkisen kehittämän prosessoriarkkitehtuurin ttk-91:n, jota käytetään tietokoneen toiminta
-kurssilla, innoittamana kehitetty arkkitehtuuri. Se on taaksepäin yhteensopiva ttk-91:n kanssa mutta
siinä on useita päivityksiä ja uusia ominaisuuksia. ttk-15 on ladattavissa osoitteesta
https://github.com/apason/ttk-15 sen voi joko kääntää lähdekoodeista tai etsiä hakemistosta target/itselleen
sopivan version. Suositeltavaa olisi käyttää version 0.3 valmiiksi käännettyjä binäärejä. (jos käyttöjärjestelmä
on dpkg tai rpm pohjainen, voi ladata suoraan .deb tai .rpm tiedostot ja asentaa ne paketinhallinnan kautta).
Windows versio toimii toistaiseksi vain cygwinin alaisuudessa joten suositeltavaa on käyttää vain linux versioita.

##ohjelman suorittaminen##
Ohjelman suoritus on erittäin helppo toimenpide. Tarvitaan vain yksi lähdetiedosto sekä ohjelma.
Lähdetiedosto annetaan ohjelmalle parametrinä ja jos koodi on syntaktisesti ja semanttisesti korrekti,
syntyy tuloksena konekielinen tiedosto "result.k15". Tämä tiedosto on siis ttk-15 arkkitehtuurin tunnistama
ohjelma (symbolisella konekielellä).

Jos käännettyä ohjelmaa haluaa vielä kokeilla suorittaa, tulee ttk-15 emulaattori ladata. Tämän jälkeen .k15
tiedosto (joka siis on symbolista konekieltä) tulee vielä kääntää binääriksi. Tämä tapahtuu yksinkertaisimmillaan
ajamalla ttk-15 projektin kääntäjä "ttc" parametrinaan tämän kurssin kääntäjän luoma result.k15 riedosto.
Tämä luo objektimoduulin result.o15 joka pitää vielä linkittää (vaikka olisikin ainoa objektimoduuli),
jotta saadaan suorituskelpoinen ttk-15 binääri. Linkitys tapahtuu yksinkertaisimmillaan antamalla ttk-15 projektin
linkkerille parametrina saatu .o15 tiedosto: linker result.o15
Nyt linkerin antama tiedosto a.out.b15 voidaan suorittaa ttk-15 emulaattorilla. Suoritus tapahtuu
yksinkertaisimmillaan antamalla binääritiedosto parametriksi ohjelmalle ttk-15: ttk-15 a.out.b15

Tarkemmat ohjeet ttk-15 käyttämiseen löytyy osoitteesta https://github.com/apason/ttk-15/tree/master/doc

Jos syntaksissa tai semantiikassa on virhe, tulostuu syytä kuvaava virheviesti ja suoritus keskeytetään.

HUOM! Suosittelen lukemaan tiedoston doc/toteutusdokumentti.md kohdan "puutteet ja parannusehdotukset" -> tulostus.
