------------------------------------------------------
-- IDS uloha 2,3, SQL  skript
-- 14.3.2018
-- Filip Kocica, Igor Ignac
------------------------------------------------------

------------------------------------------------------
-- Vymazani tabulek
------------------------------------------------------
DROP TABLE Osetrovatel CASCADE CONSTRAINT;
DROP TABLE Zvire CASCADE CONSTRAINT;
DROP TABLE ZaznamCisteni CASCADE CONSTRAINT;
DROP TABLE ZaznamKrmeni CASCADE CONSTRAINT;
DROP TABLE Vybeh CASCADE CONSTRAINT;

DROP TABLE VykonalCistenie CASCADE CONSTRAINT;
DROP TABLE VykonalKrmenie CASCADE CONSTRAINT;

DROP TABLE VedouciZOO CASCADE CONSTRAINT;


------------------------------------------------------
-- Vytvoreni tabulek
------------------------------------------------------
CREATE TABLE Osetrovatel (
rodne_cislo_osetrovatel INTEGER NOT NULL,
jmeno VARCHAR(20) NOT NULL,
prijmeni VARCHAR(20) NOT NULL,
datum_narozeni VARCHAR(20) NOT NULL,
titul VARCHAR(20),
specializace VARCHAR(20),
prac_doba VARCHAR(20) NOT NULL
);

-------------------------------------------------------------------------------------------------------
-- Vztah generalizace/specializace mezi vedoucim zoo a osetrovatelem (VedouciZOO dedi z Osetrovatele)
-------------------------------------------------------------------------------------------------------
CREATE TABLE VedouciZOO (
rodne_cislo_vedouci INTEGER NOT NULL,
jmeno VARCHAR(20) NOT NULL,
prijmeni VARCHAR(20) NOT NULL,
datum_narozeni VARCHAR(20) NOT NULL,
titul VARCHAR(20) NOT NULL,
specializace VARCHAR(20),
prac_doba VARCHAR(20) NOT NULL,
prax VARCHAR(20) NOT NULL,
CONSTRAINT CHK_datum_veduci CHECK (datum_narozeni not like '[1-2][0-9][0-9][0-9]-[0-1][0-9]-[0-3][0-9]')
);

CREATE TABLE Zvire (
id_zvire INTEGER NOT NULL,
krajina_puvodu VARCHAR(20) NOT NULL,
datum_narozeni DATE NOT NULL,
datum_umrti DATE,
oblast_vyskytu VARCHAR(20) NOT NULL,
id_vybehu INTEGER NOT NULL,
id_rodic INTEGER
);

CREATE TABLE Vybeh (
id_vybehu INTEGER NOT NULL,
typ_vybehu VARCHAR(20) NOT NULL,
kapacita_vybehu INTEGER NOT NULL,
cistota VARCHAR(20) NOT NULL
);

CREATE TABLE ZaznamCisteni (
id_cisteni INTEGER NOT NULL,
delka_cisteni INTEGER NOT NULL,
cas DATE NOT NULL,
pomucky VARCHAR(20) NOT NULL,
id_vybehu INTEGER NOT NULL,
rodne_cislo_vedouci INTEGER NOT NULL
);

CREATE TABLE ZaznamKrmeni (
id_krmeni INTEGER NOT NULL,
delka_krmeni INTEGER NOT NULL,
cas DATE NOT NULL,
mnozstvi_krmeni INTEGER NOT NULL,
id_zvire INTEGER NOT NULL,
rodne_cislo_vedouci INTEGER NOT NULL
);

----------------
-- m:n tabulky
----------------
CREATE TABLE VykonalKrmenie (
  rodne_cislo_osetrovatel NUMBER NOT NULL,
  id_krmeni NUMBER NOT NULL
);

CREATE TABLE VykonalCistenie (
  rodne_cislo_osetrovatel NUMBER NOT NULL,
  id_cisteni NUMBER NOT NULL
);

------------------------------------------------------
-- Primarni klice
------------------------------------------------------
ALTER TABLE Osetrovatel ADD CONSTRAINT PK_Osetrovatel PRIMARY KEY (rodne_cislo_osetrovatel);
ALTER TABLE Zvire ADD CONSTRAINT PK_Zvire PRIMARY KEY (id_zvire);
ALTER TABLE Vybeh ADD CONSTRAINT PK_Vybeh PRIMARY KEY (id_vybehu);
ALTER TABLE ZaznamCisteni ADD CONSTRAINT PK_ZaznamCisteni PRIMARY KEY (id_cisteni);
ALTER TABLE ZaznamKrmeni ADD CONSTRAINT PK_ZaznamKrmeni PRIMARY KEY (id_krmeni);

ALTER TABLE VedouciZOO ADD CONSTRAINT PK_VedouciZOO PRIMARY KEY (rodne_cislo_vedouci);

ALTER TABLE VykonalKrmenie ADD CONSTRAINT PK_VykonalKrmenie PRIMARY KEY (id_krmeni, rodne_cislo_osetrovatel);
ALTER TABLE VykonalCistenie ADD CONSTRAINT PK_VykonalCistenie PRIMARY KEY (id_cisteni, rodne_cislo_osetrovatel);

------------------------------------------------------
-- Cizi klice
------------------------------------------------------
ALTER TABLE VykonalKrmenie ADD CONSTRAINT FK_JeKrmene FOREIGN KEY (id_krmeni) REFERENCES ZaznamKrmeni ON DELETE CASCADE;
ALTER TABLE VykonalKrmenie ADD CONSTRAINT FK_OsetrovatelKrmi FOREIGN KEY (rodne_cislo_osetrovatel) REFERENCES Osetrovatel ON DELETE CASCADE;
ALTER TABLE VykonalCistenie ADD CONSTRAINT FK_JeCistene FOREIGN KEY (id_cisteni) REFERENCES ZaznamCisteni ON DELETE CASCADE;
ALTER TABLE VykonalCistenie ADD CONSTRAINT FK_OsetrovatelCisti FOREIGN KEY (rodne_cislo_osetrovatel) REFERENCES Osetrovatel ON DELETE CASCADE;

------------------
-- 1:n references
------------------
ALTER TABLE ZaznamCisteni ADD CONSTRAINT FK_CisteniVybehu FOREIGN KEY (id_vybehu) REFERENCES Vybeh ON DELETE CASCADE;
ALTER TABLE ZaznamCisteni ADD CONSTRAINT FK_UpravitZaznamCisteni FOREIGN KEY (rodne_cislo_vedouci) REFERENCES VedouciZOO ON DELETE CASCADE;
ALTER TABLE ZaznamKrmeni ADD CONSTRAINT FK_KrmeniZvirete FOREIGN KEY (id_zvire) REFERENCES Zvire ON DELETE CASCADE;
ALTER TABLE ZaznamKrmeni ADD CONSTRAINT FK_UpravitZaznamKrmeni FOREIGN KEY (rodne_cislo_vedouci) REFERENCES VedouciZOO ON DELETE CASCADE;
ALTER TABLE Zvire ADD CONSTRAINT FK_Obyva FOREIGN KEY (id_vybehu) REFERENCES Vybeh ON DELETE CASCADE;
ALTER TABLE Zvire ADD CONSTRAINT FK_JeRodic FOREIGN KEY (id_zvire) REFERENCES Zvire ON DELETE CASCADE;

-- Osetrovatel
INSERT INTO Osetrovatel(rodne_cislo_osetrovatel, jmeno, prijmeni, datum_narozeni, titul, specializace, prac_doba) VALUES('9606053258', 'Igor', 'Ignac', TO_DATE('1996-06-05', 'YYYY-MM-DD'), 'MBA', 'Lev', '8:00-16:00');
INSERT INTO Osetrovatel(rodne_cislo_osetrovatel, jmeno, prijmeni, datum_narozeni, titul, specializace, prac_doba) VALUES('9704293256', 'Filip', 'Kocica', TO_DATE('1997-04-29', 'YYYY-MM-DD'), 'Ing. Mgr. RNDr. MBA', 'Hroch', '14:00-14:30');
INSERT INTO Osetrovatel(rodne_cislo_osetrovatel, jmeno, prijmeni, datum_narozeni, titul, specializace, prac_doba) VALUES('8512123356', 'Ales', 'Novak', TO_DATE('1985-12-12', 'YYYY-MM-DD'), 'Ph D.', 'Pavian', '5:50-23:30');
INSERT INTO Osetrovatel(rodne_cislo_osetrovatel, jmeno, prijmeni, datum_narozeni, titul, specializace, prac_doba) VALUES('9805023365', 'Pavel', 'Ony', TO_DATE('1998-05-02', 'YYYY-MM-DD'), NULL, 'Slepice', '9:00-15:00');
INSERT INTO Osetrovatel(rodne_cislo_osetrovatel, jmeno, prijmeni, datum_narozeni, titul, specializace, prac_doba) VALUES('9202052565', 'Karel', 'Svoboda', TO_DATE('1992-02-05', 'YYYY-MM-DD'), 'Bc.', 'Krokodyl', '13:00-16:00');

-- VedouciZOO
INSERT INTO VedouciZOO(rodne_cislo_vedouci, jmeno, prijmeni, datum_narozeni, titul, specializace, prac_doba, prax) VALUES('8602123532', 'Igor', 'Kocica', TO_DATE('1986-02-12', 'YYYY-MM-DD'), 'Bc.', NULL, '14:00-15:30', '10 let osetrovatel');

-- Vybeh
INSERT INTO Vybeh (id_vybehu, typ_vybehu, kapacita_vybehu, cistota) VALUES ('1', 'savana', '10', 'cisty');
INSERT INTO Vybeh (id_vybehu, typ_vybehu, kapacita_vybehu, cistota) VALUES ('2', 'mala ohrada', '5', 'mirne spinavy');
INSERT INTO Vybeh (id_vybehu, typ_vybehu, kapacita_vybehu, cistota) VALUES ('3', 'akvarium', '10', 'mirne spinavy');
INSERT INTO Vybeh (id_vybehu, typ_vybehu, kapacita_vybehu, cistota) VALUES ('4', 'nadrz s pevninou', '4', 'spinavy');
INSERT INTO Vybeh (id_vybehu, typ_vybehu, kapacita_vybehu, cistota) VALUES ('5', 'velka ohrada', '15', 'cisty');

-- Zvire
INSERT INTO Zvire(id_zvire, krajina_puvodu, datum_narozeni, datum_umrti, oblast_vyskytu, id_vybehu, id_rodic) VALUES('1', 'Afrika', TO_DATE('2014-11-11', 'YYYY-MM-DD'), NULL, 'Afrika', '1', NULL);
INSERT INTO Zvire(id_zvire, krajina_puvodu, datum_narozeni, datum_umrti, oblast_vyskytu, id_vybehu, id_rodic) VALUES('2', 'Evropa', TO_DATE('2012-05-15', 'YYYY-MM-DD'), NULL, 'Evropa', '2', '1');
INSERT INTO Zvire(id_zvire, krajina_puvodu, datum_narozeni, datum_umrti, oblast_vyskytu, id_vybehu, id_rodic) VALUES('3', 'Asie', TO_DATE('2014-08-13', 'YYYY-MM-DD'), NULL, 'Afrika', '3', '2');
INSERT INTO Zvire(id_zvire, krajina_puvodu, datum_narozeni, datum_umrti, oblast_vyskytu, id_vybehu, id_rodic) VALUES('4', 'Amerika', TO_DATE('2016-02-03', 'YYYY-MM-DD'), TO_DATE('2018-03-14', 'YYYY-MM-DD'), 'Amerika; Afrika', '4', '3');
INSERT INTO Zvire(id_zvire, krajina_puvodu, datum_narozeni, datum_umrti, oblast_vyskytu, id_vybehu, id_rodic) VALUES('5', 'Amerika', TO_DATE('2015-12-23', 'YYYY-MM-DD'), NULL, 'Amerika', '5', '4');

-- Zaznam krmeni
INSERT INTO ZaznamKrmeni(id_krmeni, delka_krmeni, cas, mnozstvi_krmeni, id_zvire, rodne_cislo_vedouci) VALUES('1', '23', TO_DATE('2014-11-12', 'YYYY-MM-DD'), '25', '1', '8602123532');
INSERT INTO ZaznamKrmeni(id_krmeni, delka_krmeni, cas, mnozstvi_krmeni, id_zvire, rodne_cislo_vedouci) VALUES('2', '31', TO_DATE('2014-08-08', 'YYYY-MM-DD'), '44', '2', '8602123532');
INSERT INTO ZaznamKrmeni(id_krmeni, delka_krmeni, cas, mnozstvi_krmeni, id_zvire, rodne_cislo_vedouci) VALUES('3', '876', TO_DATE('2014-11-12', 'YYYY-MM-DD'), '123', '3', '8602123532');
INSERT INTO ZaznamKrmeni(id_krmeni, delka_krmeni, cas, mnozstvi_krmeni, id_zvire, rodne_cislo_vedouci) VALUES('4', '56', TO_DATE('2014-08-08', 'YYYY-MM-DD'), '32', '4', '8602123532');
INSERT INTO ZaznamKrmeni(id_krmeni, delka_krmeni, cas, mnozstvi_krmeni, id_zvire, rodne_cislo_vedouci) VALUES('5', '9', TO_DATE('2014-12-31', 'YYYY-MM-DD'), '65', '5', '8602123532');

-- Zaznam cisteni
INSERT INTO ZaznamCisteni (id_cisteni, delka_cisteni, cas, pomucky, id_vybehu, rodne_cislo_vedouci) VALUES ('1', '100', TO_DATE('2014-11-12','YYYY-MM-DD'), 'hrable', '1', '8602123532');
INSERT INTO ZaznamCisteni (id_cisteni, delka_cisteni, cas, pomucky, id_vybehu, rodne_cislo_vedouci) VALUES ('2', '85', TO_DATE('2014-06-25','YYYY-MM-DD'), 'koste', '2', '8602123532');
INSERT INTO ZaznamCisteni (id_cisteni, delka_cisteni, cas, pomucky, id_vybehu, rodne_cislo_vedouci) VALUES ('3', '60', TO_DATE('2014-07-03','YYYY-MM-DD'), 'sitka', '3', '8602123532');
INSERT INTO ZaznamCisteni (id_cisteni, delka_cisteni, cas, pomucky, id_vybehu, rodne_cislo_vedouci) VALUES ('4', '105', TO_DATE('2014-08-08','YYYY-MM-DD'), 'lopata', '4', '8602123532');
INSERT INTO ZaznamCisteni (id_cisteni, delka_cisteni, cas, pomucky, id_vybehu, rodne_cislo_vedouci) VALUES ('5', '40', TO_DATE('2014-07-10','YYYY-MM-DD'), 'koste', '5', '8602123532');

-- VykonalKrmenie
INSERT INTO VykonalKrmenie(id_krmeni, rodne_cislo_osetrovatel) VALUES ('1', '9606053258');
INSERT INTO VykonalKrmenie(id_krmeni, rodne_cislo_osetrovatel) VALUES ('2', '9805023365');
INSERT INTO VykonalKrmenie(id_krmeni, rodne_cislo_osetrovatel) VALUES ('3', '9606053258');
INSERT INTO VykonalKrmenie(id_krmeni, rodne_cislo_osetrovatel) VALUES ('4', '9606053258');
INSERT INTO VykonalKrmenie(id_krmeni, rodne_cislo_osetrovatel) VALUES ('5', '9805023365');

-- VykonalCistenie
INSERT INTO VykonalCistenie(id_cisteni, rodne_cislo_osetrovatel) VALUES ('1', '9202052565');
INSERT INTO VykonalCistenie(id_cisteni, rodne_cislo_osetrovatel) VALUES ('2', '8512123356');
INSERT INTO VykonalCistenie(id_cisteni, rodne_cislo_osetrovatel) VALUES ('3', '9606053258');
INSERT INTO VykonalCistenie(id_cisteni, rodne_cislo_osetrovatel) VALUES ('4', '8512123356');
INSERT INTO VykonalCistenie(id_cisteni, rodne_cislo_osetrovatel) VALUES ('5', '9202052565');

-------------------------------------------------------------
-- Uloha c. 3
-------------------------------------------------------------



-- Spojeni 2 tabulek
-- Tabulka jmen vybehu jez byly cisteny v rozmezi 1.7.2014 az 12.12.2014
SELECT typ_vybehu AS vybeh, cas
FROM Vybeh NATURAL JOIN ZaznamCisteni
WHERE cas BETWEEN TO_DATE('2014-07-01','YYYY-MM-DD') AND TO_DATE('2014-12-12','YYYY-MM-DD');



-- Spojeni 2 tabulek
-- Ktere vybehy obsahuji zvirata z ameriky
SELECT typ_vybehu as vybeh
FROM Vybeh NATURAL JOIN Zvire
WHERE krajina_puvodu='Amerika';



-- Spojeni 3 tabulek
-- Kteremu zvireti se kdy cistil vybeh (jsou pouze 2, protoze ostatni cisteni se provadely
--                                      drive nez se zvirata narodila)
SELECT z.id_zvire, zc.cas
FROM Vybeh v, Zvire z, ZaznamCisteni zc
WHERE z.id_vybehu = v.id_vybehu AND v.id_vybehu = zc.id_vybehu
    AND zc.cas >= z.datum_narozeni;


-- S klauzuli GROUP BY a agregacni funkci COUNT
-- Kolik cisteni vybehu provedl ktery osetrovatel za celou dobu prace
SELECT jmeno, prijmeni, COUNT(id_cisteni) AS pocet_cisteni
FROM Osetrovatel NATURAL JOIN VykonalCistenie
GROUP BY rodne_cislo_osetrovatel, jmeno, prijmeni
ORDER BY COUNT(id_cisteni) ASC;



-- S klauzuli GROUP BY a agregacni funkci AVG
-- Prumerne mnozstvi krmeni pouziteho za jednotlive dny serazeno vzestupne
SELECT cas, AVG(mnozstvi_krmeni) as max_mnozstvi_krmeni
FROM ZaznamKrmeni
GROUP BY cas
ORDER BY AVG(mnozstvi_krmeni) ASC;



-- Dotaz obsahujuci predikat EXISTS
-- Vsetky zaznamy krmenia upravene veducim ZOO v ktorych bolo mnozstvo krmiva mensie ako 60
SELECT *
FROM ZaznamKrmeni
WHERE EXISTS(SELECT * FROM VedouciZOO WHERE ZaznamKrmeni.rodne_cislo_vedouci = VedouciZOO.rodne_cislo_vedouci AND mnozstvi_krmeni < 60);



-- Dotaz obsahujici predikat IN
-- Vsechny zaznamy cisteni jez se provadely ve stejnou dobu jako nektere z krmeni
SELECT *
FROM ZaznamCisteni
WHERE cas IN (SELECT cas FROM ZaznamKrmeni);