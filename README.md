# OopBoidsSimuleringThreepp av Fillip Grüner
Kandidatnummer: 10020

## UML Diagram
UML-diagrammet (img_2.png) viser hvordan klassene og funksjonsheaders er koblet sammen. 
main.cpp er ikke med i diagrammet, ettersom den bruker alle .hpp-filene, men eier ingen av dem.

Diagrammet beskriver viktige egenskaper og funksjoner for hver klasse. For UtilityFunctions og ThreeppFunctions, 
som ikke er klasser, beskrives alle funksjonene i diagrammet.

## Brukte tredjepartsbiblioteker
Dette prosjektet benytter seg av følgende tredjepartsbiblioteker:

- [threepp](https://github.com/markaren/threepp)
- [Catch2](https://github.com/catchorg/Catch2)
- [ImGui](https://github.com/ocornut/imgui)

## Om Prosjektet
Prosjektet er en simulering av forskjellige flokker med boids (fuglelignende objekter) som beveger seg i et 
definert område. Simuleringen inkluderer også predators (rovfugler) som kan fjernes dynamisk fra simuleringen.

Simulasjonen kan brukes til å studere effekten av flokking for å avlede eller forvirre potensielle trusler. 
Under kjøring kan brukeren justere ulike parametre for boids og flokkens adferd, samt aktivere eller deaktivere rovdyr.

## Hovedfunksjoner
Simulering av boids: Diverse boids-bevegelser og flokkformasjon.

Predatorer: Predators kan toggles for å observere boidene sin respons på trusselen.

Parametertilpasning: Endre verdier for boids sin hastighet, søkeavstand og flokk-krefter.

Full-screen visning: Programmet kan kjøres i fullskjermmodus for bedre visualisering.

Orbit controls: Lar brukeren bevege seg rundt arenaen og observere simuleringen fra ulike vinkler.


## Bruk:
1. Start programmet og trykk på "Run" for å starte simuleringen.
2. Bytt til fullskjermmodus for best visning.
3. Juster boids- og predatorparametre ved hjelp av sliders.
4. Bruk orbit controls for å bevege kameraet rundt simuleringen.

## Kompilering og Kjøring
Krav:
C++20 eller nyere.
CMake 3.21 eller nyere.
Installere nødvendige tredjepartsbiblioteker (threepp, Catch2, ImGui).

## Bygging:
Clone prosjektet.
Kjør cmake for å konfigurere prosjektet.
Bygg prosjektet med make eller ønsket byggesystem.

## Kjøring:
Etter bygging, kjør ./BoidSimulation for å starte programmet.
Etter bygging, kjør ./tests for å teste programmet.

## Unittests:
For å kjøre ulike tester på programmet, kan man bytte run-konfigurasjonen om til tests.
dette vil kjøre testene i tests.cpp ved hjelp av Catch2, disse har beskrivende overskrifter som
viser hva som testes.

## Lisens
Dette prosjektet er lisensiert under MIT-lisensen – se LICENSE for detaljer.