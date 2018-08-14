
# viikkoraportti 5

Tällä viikolla toteutin parseriin syntaksipuun generoinnin. Nyt ohjelma generoi puuta syntaksitarkistuksen yhteydessä. Lisäksi toteutin semanttisen tarkistuksen.
Siihen kuuluu tarkistus, että määrittelemättömiä muuttujia ei voi käyttää. Lisäksi muuttujia määriteltäesstä tarkistetaan että sen nimistä muuttujaa
ei ole jo aiemmin määritelty. Koska tarkoituksenani on toteuttaa myös koodin generointi ttk-15 alustalle eikä kyseisessä arkkitehtuurissa ole konekäskyä
kokonaisluvun muuntamiseksi liukulukuun tai päinvastoin, tein sellaisen ratkaisun että kieli ei tue eri tietotyyppien sekoitusta lausekkeissa. Toisinsanoen
lauseke a+b on virheellinen jos a ja b ovat eri tietotyyppejä. Semanttinen tarkistus varmistaa siis ettei määrittelemättömiä muuttujia voida käyttää, ettei
muuttujia voida määritellä useampaan kertaan sekä sen ettei eri tietotyyppejä voida laskea yhteen.

Parserille tein lisää testejä ja se onkin nyt melko kattavasti testattu. Jaoin iw terminaalin erillisiksi if ja while terminaaleiksi ja tein niille testit.

Kieleen tuli myös yksi nonterminaali (comparation) lisää. Tämä oli välttämätöntä semantiikan tarkastamisen kannalta.

Dokumentaatiota on myös päivtetty asianmukaisesti. Minkäänlaista testausdokumentaatiota en kuitenkaan tehnyt, sillä se ei aiheessani ole kovin
mielekästä + syntaksipuun ja semantiikan implementointiin meni huomattavasti aikaa.

Nyt koodi alkaakin olla kovasti refaktoroinnin tarpeessa joten ensi viikolla koitan siistiä koodia ja perehtyä koodin generointiin.
Refaktoroinnin yhteydessä myös kommentoin koodia huolellisemmin. Toisaalta koko projektissa on vain 3 pienen pientä non-static funktiota
ja ne on kommentoitu kuten pitää.
