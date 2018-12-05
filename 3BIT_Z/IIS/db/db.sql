-- Smazat tabulky (pokud existuji)
DROP TABLE IF EXISTS Zvire CASCADE;
DROP TABLE IF EXISTS ZaznamCisteni CASCADE;
DROP TABLE IF EXISTS ZaznamKrmeni CASCADE;
DROP TABLE IF EXISTS Vybeh CASCADE;
DROP TABLE IF EXISTS VedouciZOO CASCADE;
DROP TABLE IF EXISTS Osetrovatel CASCADE;
DROP TABLE IF EXISTS Rasa CASCADE;

-- Tabulky
CREATE TABLE Osetrovatel (
login VARCHAR(256) ,
password VARCHAR(256) ,
jmeno VARCHAR(256) ,
prijmeni VARCHAR(256) ,
datum_narozeni VARCHAR(256) ,
titul VARCHAR(256),
specializace VARCHAR(256)
);

CREATE TABLE VedouciZOO (
login VARCHAR(256) ,
password VARCHAR(256) ,
jmeno VARCHAR(256) ,
prijmeni VARCHAR(256) ,
datum_narozeni VARCHAR(256) ,
titul VARCHAR(256) ,
specializace VARCHAR(256)
);

CREATE TABLE Zvire (
id_zvire INTEGER ,
rasa VARCHAR(256) ,
jmeno VARCHAR(256) ,
krajina_puvodu VARCHAR(256) ,
datum_narozeni DATE ,
datum_umrti DATE,
oblast_vyskytu VARCHAR(256) ,
id_vybehu INTEGER ,
id_rodic INTEGER
);

CREATE TABLE Rasa (
jmeno VARCHAR(256) ,
popis VARCHAR(256)
);

CREATE TABLE Vybeh (
id_vybehu INTEGER ,
typ_vybehu VARCHAR(256) ,
kapacita_vybehu INTEGER ,
cistota VARCHAR(256),
id_zvire1 INTEGER,
id_zvire2 INTEGER,
id_zvire3 INTEGER
);

CREATE TABLE ZaznamCisteni (
id_cisteni INTEGER ,
delka_cisteni INTEGER ,
cas DATE ,
pomucky VARCHAR(256) ,
id_vybehu INTEGER ,
login VARCHAR(256)
);

CREATE TABLE ZaznamKrmeni (
id_krmeni INTEGER ,
delka_krmeni INTEGER ,
cas DATE ,
mnozstvi_krmeni INTEGER ,
id_zvire INTEGER ,
login VARCHAR(256)
);

-- Primarni Klice
ALTER TABLE Osetrovatel ADD CONSTRAINT PK_Osetrovatel PRIMARY KEY (login);
ALTER TABLE Zvire ADD CONSTRAINT PK_Zvire PRIMARY KEY (id_zvire);
ALTER TABLE Vybeh ADD CONSTRAINT PK_Vybeh PRIMARY KEY (id_vybehu);
ALTER TABLE ZaznamCisteni ADD CONSTRAINT PK_ZaznamCisteni PRIMARY KEY (id_cisteni);
ALTER TABLE ZaznamKrmeni ADD CONSTRAINT PK_ZaznamKrmeni PRIMARY KEY (id_krmeni);
ALTER TABLE VedouciZOO ADD CONSTRAINT PK_VedouciZOO PRIMARY KEY (login);
-- Cizi klice
ALTER TABLE ZaznamCisteni ADD CONSTRAINT FK_VykonalCisteni FOREIGN KEY (login) REFERENCES Osetrovatel (login);
ALTER TABLE ZaznamKrmeni ADD CONSTRAINT FK_VykonalKrmeni FOREIGN KEY (login) REFERENCES Osetrovatel (login);



-- Osetrovatel
INSERT INTO Osetrovatel(login, password, jmeno, prijmeni, datum_narozeni, titul, specializace) VALUES ('osetrovatelA', 'osetrovatel123', 'Filip', 'Kočica', '1997-04-29', 'Ing.', 'Savci');
INSERT INTO Osetrovatel(login, password, jmeno, prijmeni, datum_narozeni, titul, specializace) VALUES ('osetrovatelB', 'osetrovatel123', 'Igor', 'Ignác', '1970-01-01', 'Mgr. Ph.D.', 'Plazi');
-- VedouciZOO
INSERT INTO VedouciZOO(login, password, jmeno, prijmeni, datum_narozeni, titul, specializace) VALUES ('admin', 'admin123', 'Palo', 'Habera', '1970-12-24', 'Bc.', 'Savci');
-- Vybeh
INSERT INTO Vybeh (id_vybehu, typ_vybehu, kapacita_vybehu, cistota, id_zvire1, id_zvire2, id_zvire3) VALUES ('1', 'Jižní výběh', '3', 'Čistý', '1', '2', '0');
INSERT INTO Vybeh (id_vybehu, typ_vybehu, kapacita_vybehu, cistota, id_zvire1, id_zvire2, id_zvire3) VALUES ('2', 'Severní výběh', '3', 'Špinavý', '3', '0', '0');
-- Zvire
INSERT INTO Zvire(id_zvire, rasa, jmeno, krajina_puvodu, datum_narozeni, datum_umrti, oblast_vyskytu, id_vybehu, id_rodic) VALUES ('1', 'Žirafa', 'Pepik', 'Afrika', '2014-11-11', NULL, 'Afrika; Evropa; Amerika', '1', '2');
INSERT INTO Zvire(id_zvire, rasa, jmeno, krajina_puvodu, datum_narozeni, datum_umrti, oblast_vyskytu, id_vybehu, id_rodic) VALUES ('2', 'Žirafa', 'Jiřinka', 'Evropa', '2011-04-03', NULL, 'Afrika; Evropa; Amerika', '1', NULL);
INSERT INTO Zvire(id_zvire, rasa, jmeno, krajina_puvodu, datum_narozeni, datum_umrti, oblast_vyskytu, id_vybehu, id_rodic) VALUES ('3', 'Zebra', 'Pavlík', 'Asie', '1998-12-13', NULL, 'Asie', '2', NULL);
-- Rasa
INSERT INTO Rasa(jmeno, popis) VALUES ('Žirafa', 'Zvířátko s dlouhým krkem. Doporučená denní dávka potravy je 20kg trávy.');
INSERT INTO Rasa(jmeno, popis) VALUES ('Zebra', 'Doporučená denní dávka potravy je 10kg zeleniny.');
-- Krmeni
INSERT INTO ZaznamKrmeni(id_krmeni, delka_krmeni, cas, mnozstvi_krmeni, id_zvire, login) VALUES ('1', '20', '2014-11-12', '25', '1', 'osetrovatelA');
INSERT INTO ZaznamKrmeni(id_krmeni, delka_krmeni, cas, mnozstvi_krmeni, id_zvire, login) VALUES ('2', '42', '2015-10-09', '33', '3', 'osetrovatelB');
-- Cisteni
INSERT INTO ZaznamCisteni (id_cisteni, delka_cisteni, cas, pomucky, id_vybehu, login) VALUES ('1', '60', '2012-06-07', 'Hrábě, hadice, lopata', '1', 'osetrovatelB');
INSERT INTO ZaznamCisteni (id_cisteni, delka_cisteni, cas, pomucky, id_vybehu, login) VALUES ('2', '45', '2008-08-13', 'Pytel, hadice', '2', 'osetrovatelA');