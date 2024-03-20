Popa Bianca

Client Web. Comunicatie cu REST API.

Detalii implementare:

Laboratorul de HTTP a fost folosit ca schelet de cod la implementarea proiectului.
Astfel, fisierele buffer.cpp, buffer.h, helpers.cpp, helpers.h, requests.cpp
si requests.h au ramas, in mare parte, neschimbate. In plus fata de laborator,
s-a adaugat functia compute_delete_request, care construieste o cerere de tip
DELETE, iar pentru toate cele 3 functii din requests a fost adaugat un parametru
pentru tokenul JWT. Acesta este adaugat in headerul Authorization si este 
prefixat de cuvantul Bearer.

Restul temei este implementata in fisierul client.cpp, unde se citesc comenzile
de la tastatura astfel:

1. autentificarea (login)

Dupa citirea comenzii, se seteaza ruta de acces si tipul de payload, apoi se
citesc de la tastatura username-ul si parola, care sunt adaugate intr-un json.
Se deschide un socket pentru comunicarea cu serverul, cu hostul 34.254.242.81 si
portul 8080. Se formeaza cererea de tip POST si se trimite la server. Dupa ce se
primeste raspunsul, se trateaza cazul de eroare (credentialele nu se potrivesc).
Daca nu exista erori, se afiseaza mesajul "200 - OK" si se parseaza raspunsul
pentru a salva cookie-ul de sesiune, de care este nevoie la urmatoarele comenzi.

2. inregistrarea unui cont (register)

Dupa citirea comenzii, se seteaza ruta de acces si tipul de payload, apoi se
citesc de la tastatura username-ul si parola, care sunt adaugate intr-un json.
Se deschide un socket pentru comunicarea cu serverul, cu hostul 34.254.242.81 si
portul 8080. Se formeaza cererea de tip POST si se trimite la server. Dupa ce se
primeste raspunsul, se trateaza cazul de eroare (username-ul este deja folosit de
catre cineva). Daca nu exista erori, se afiseaza mesajul "200 - OK".

3. cerere de acces în biblioteca (enter_library)

Dupa citirea comenzii, se seteaza ruta de acces. Se deschide un socket pentru 
comunicarea cu serverul, cu hostul 34.254.242.81 si portul 8080. Se formeaza
cererea de tip GET si se trimite la server. Dupa ce se primeste raspunsul, se
trateaza cazul de eroare (utilizatorul nu este autentificat). Daca nu exista
erori, se afiseaza mesajul "200 - OK" si se parseaza raspunsul pentru a salva
tokenul JWT, de care este nevoie la urmatoarele comenzi.

4. vizualizarea informatiilor sumare despre toate cartile (get_books)

Dupa citirea comenzii, se seteaza ruta de acces. Se deschide un socket pentru
comunicarea cu serverul, cu hostul 34.254.242.81 si portul 8080. Se formeaza
cererea de tip GET si se trimite la server. Dupa ce se primeste raspunsul, se
trateaza cazul de eroare (utilizatorul nu are acces la biblioteca). Daca nu exista
erori, se parseaza raspunsul pentru a obtine json-ul cu informatiile despre carti.

5. vizualizarea detaliilor despre o carte (get_book)

Dupa citirea comenzii, se citeste id-ul cartii. Se verifica daca acesta este valid
(contine numai cifre) si se afiseaza mesajul de eroare "Invalid id" in caz contrar.
Se seteaza ruta de acces si se deschide un socket pentru comunicarea cu serverul, 
cu hostul 34.254.242.81 si portul 8080. Se formeaza cererea de tip GET si se trimite 
la server. Dupa ce se primeste raspunsul, se trateaza cazul de eroare (utilizatorul
nu are acces la biblioteca). Daca nu exista erori, se parseaza raspunsul pentru a
obtine json-ul cu informatiile despre cartea ceruta.

6. adaugarea unei carti (add_book)

Dupa citirea comenzii, se citesc de la tastatura detaliile cartii. Se verifica daca
page_count-ul este valid (contine numai cifre) si se afiseaza mesajul de eroare
"Invalid page count" in caz contrar. Se seteaza ruta de acces si tipul de payload 
si se adauga intr-un json detaliile cartii. Se deschide un socket pentru comunicarea
cu serverul, cu hostul 34.254.242.81 si portul 8080. Se formeaza cererea de tip
POST si se trimite la server. Dupa ce se primeste raspunsul, se trateaza cazul de
eroare (utilizatorul nu are acces la biblioteca). Daca nu exista erori, se afiseaza
mesajul "200 - OK".

7. stergerea unei carti (delete_book)

Dupa citirea comenzii, se citeste id-ul cartii. Se verifica daca acesta este valid
(contine numai cifre) si se afiseaza mesajul de eroare "Invalid id" in caz contrar.
Se seteaza ruta de acces si se deschide un socket pentru comunicarea cu serverul,
cu hostul 34.254.242.81 si portul 8080. Se formeaza cererea de tip DELETE si se
trimite la server. Dupa ce se primeste raspunsul, se trateaza cazurile de eroare
(utilizatorul nu are acces la biblioteca; cartea cu id-ul specificat nu exista).
Daca nu exista erori, se afiseaza mesajul "200 - OK".

8. logout

Dupa citirea comenzii, se seteaza ruta de acces. Se deschide un socket pentru
comunicarea cu serverul, cu hostul 34.254.242.81 si portul 8080. Se formeaza
cererea de tip GET si se trimite la server. Dupa ce se primeste raspunsul, se
trateaza cazul de eroare (utilizatorul nu este autentificat). Daca nu exista
erori, se afiseaza mesajul "200 - OK" si se sterg datele din cookie-ul de sesiune
si tokenul JWT.

Pentru a parsa raspunsurile primite de la server, s-a folosit biblioteca nlohmann.
De ce nlohmann? Documentatia a fost usor de inteles si am preferat sa merg pe
recomandarea responsabililor de tema. :)

Functii folosite in tema:
dump() - conversia de la json la string,
dump(4) - conversia la string cu indentare,
parse(string) - conversia de la string la json
