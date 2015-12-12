#autoexec#

Laitoin tähän hakemistoon "skriptin" autoexec. Tämä kääntää .c -> .k15 ja .k15 -> .b15
sekä suorittaa .b15 tiedoston ttk-15 suoritusympäristössä. Näin ohjelman testaaminen on 
helpompaa. Jokatapauksessa tämä "scripti" ei ota ollenkaan huomioon mitään virhetilanteita
eli se toimii vain ohjelman testauksen helpottamiseksi ja on tarkoitettu siihen, että
joku haluaa kokeilla kääntäjää.

Käyttö tapahtuu siis seuraavasti: Tee .c kooditiedosto jonka kääntäjä tunnistaa 
(tästä lisää doc/ hakemistossa) ja suorita ./autoexec tiedosto.c

##HUOM## 
Binäärit ovat x86_64 linux binäärejä joten toimii vian ko. ympäristössä.
