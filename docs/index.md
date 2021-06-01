## Willkommen beim Blog zu meinem Projekt <br/> "Climb it"
### Projektidee
Mein Projekt soll eine Art "jump ’n’ run" werden. Bei dem Spielprinzip orientiere ich mich an dem Spiel "Jump King". In diesem Spiel ist es die Aufgabe, einen Turm zu erklimmen. Dies gestaltet sich durch die einzigartige Steuerung aber schwieriger als es zunächst klingt. Die Spielfigur springt, sobald die Sprungtaste losgelassen wird. Je länger diese Taste vorher gedrückt gehalten wird, desto höher springt die Figur. Durch Kollision mit Objekten prallt die Figur von diesen ab. Der Abprall nach der Kollision soll mit der vorher aufgeladenen Sprungkraft verechnet werden und somit variabel sein. 
<br/>Dieses Spielkonzept lässt mir sehr viele Möglichkeiten zur Erweiterung, wie zum Beispiel:
- Verschiedene Untergründe (nur optisch oder auch mit effekten auf die Spielfigur)
- Bewegliche Plattformen
- Unterschiedliche Figuren, die sich eventuell anders Fortbewegen
- Highscores, Anzeigen zur aktuellen Höhe, Zeit, etc.

Die vermutlich größten Probleme bei der Umsetzung dieser Idee vermute ich bei der Spielinternen Physik. Ich kann dabei bislang schlecht einschätzen, wie aufwändig diese sein wird. 

### 21.04.2021 Blog erstellt
Dieser Blog wird mithilfe von Github Pages und Jekyll geführt.


### 22.05.2021 Grundlagen von SDL2 verstehen
Der letzte Monat wurde hauptsächlich damit verbracht, die Grundlagen von SDL2 zu verstehen und mit dessen Nutzung ein wenig warm zu werden. Aus den anfänglich geplanten zwei Wochen wurden krankheitsbedingt nun leider vier. 
Die folgenden Funktionen wurden aus dieser Testphase erst einmal für "Climb it" übernommen. 
#### Die Game Klasse
Die Game Klasse beinhaltet die Grundfunktionen des Programms.  
##### Funktion init()
Diese Funktion ist dafür zuständig, das Spielfenster zu erstellen. Dazu werden Titel, Auflösung, Position und Modus des Fensters übergeben. Über die SDL_CreateWindow Funktion wird das Fenster dann erstellt und mithilfe von SDL_CreateRenderer ein 2D Renderer für das Fenster erstellt.
##### Funktion eventHandler()
Das verarbeiten von Events wird mithilfe dieser Funktion durchgeführt. Dazu wird ein SDL_Event erstellt und der entsprechende Fall dann ausgeführt. 
##### Funktion render():
Das rendern von allen Texturen wird in dieser Funktion ausgeführt. Dazu müssen drei SDL Funktionen ausgeführt werden. Der bei der Initialisierung erstellte Renderer wird zuerst geleert. Dann wird diesem die zu verarbeitende Textur übergeben und zuletzt wird mit SDL_RenderPresent ein Update des Fensters mit der neu geladenen Textur forciert.
##### Funktion clean():
Da das Programm in C++ programmiert wird, muss die sogenannte "garbage collection" manuell durchführt werden. Deshalb wird mit den SDL Funktionen SDL_DestroyWindow und SDL_DestroyRenderer zuerst Fenster und Renderer zerstört um ungewollte Belastung des Arbeitsspeichers nach dem Beenden vorzubeugen. Anschließend wird das Programm mit SDL_Quit beendet.
#### Die TextureLoader Klasse
Texturen werden in dem Spiel häufiger geladen. Diese Klasse soll diese Prozedur vereinfachen, da das Laden immer derselben Struktur folgt. Die Funktion benötigt den Pfad der Textur und den Renderer. Die Textur wird jetzt in ein SDL_Surface geladen. Dies ist ein aus dem Bild geladenes Feld mit zugriff auf jedes einzelne Pixel. Danach kann der Renderer die Textur mit SDL_CreateTextureFromSurface laden. Abschließend wird das Feld zerstört und die gerenderte Textur zurückgegeben.
<br/>
```cpp
SDL_Texture* TextureLoader::LoadTexture(const char* texture, SDL_Renderer* ren){  
	SDL_Surface* tempSur = IMG_Load(texture);  
	SDL_Texture* tex = SDL_CreateTextureFromSurface(ren, tempSur);    
	SDL_FreeSurface(tempSur);  
	return tex;  
}
```
#### Der Spielcharakter
In den letzten Wochen habe ich außerdem eine erste Idee für das Modell der Spielfigur entworfen. Diese ist noch nicht final, aber reicht vorerst für einige Tests aus. 
<br/>

<img src="https://raw.githubusercontent.com/mpeters4/GP_Peters/gh-pages/docs/img/Player.png"  width="100" height="100" />
<img src="https://raw.githubusercontent.com/mpeters4/GP_Peters/gh-pages/docs/img/Sprite%20Monkey%20figure%20GIF.gif"  width="100" height="100" />

#### Pläne und erste Ideen für die nächste Woche
Nächste Woche soll damit begonnen werden, die Spielfigur mit Tastatureingaben zu steuern. Außerdem möchte ich zusätzliche Objekte einfügen und versuchen, eine Kollision zu erzeugen. Dazu werde ich einen Weg finden müssen, die Objekte auch physisch zu platzieren. Eine erste Idee dazu wäre, eine Matrix aufzustellen und die Objekte darin zu platzieren. Die passende Textur könnte dann einfach an diese entsprechende Stelle gesetzt werden. 

### 31.05.2021 Spielfigur bewegen und Map generieren
Folgende Fortschritte wurden in der letzten Woche erzielt:
- Laden der Texturen überarbeitet
- Erste Bewegung der Figur mit Tastatureingaben
- Spielfeld generieren (vorerst nur optisch)

#### Laden der Texturen mithilfe der Object Klasse
Für das platzieren von Texturen werden neben der Bilddatei selbst zwei Informationen benötigt. Zum einen wird die Quelle des Bildes (Source) gebraucht. Dazu wird mit SDL_Rect ein Rechteck mit dem gewünschten Bildausschnitt erzeugt. Es wird angegeben bei welchen Koordinaten innerhalb der Textur der Ausschnitt beginnt und wie hoch bzw. breit dieser ist. Zum anderen wird der Zielort(Destination) bestimmt. Hier wird ebenfalls ein Rechteck erzeugt. Es werden die Koordinaten des Zielorts und die Größe der Textur benötigt. Mit dem Destination Rechteck ist es zusätzlich möglich, die Textur zu skalieren. <br/>Um das Generieren und rendern von vielen Objekten zu erleichtern, habe ich die Object Klasse hinzugefügt. Diese enthält den Dateipfad und die beiden benötigten SDL_Rect Variablen.
#### Bewegen der Figur und erfassen von Tastatureingaben
Mithilfe von SDL_KEYDOWN bzw. SDL_KEYUP, lässt sich genau festhalten, wann die jeweilige Taste betätigt und wieder losgelassen wird. Dies ist sehr praktisch für meine Steuerung, da ich die Sprungkraft von der Dauer des Tastendruckes abhängig machen möchte. In dem EventHandler wird ermittelt, ob eine der getrackten Tasten verwendet wird, und wenn ja, welche. Wird eine Taste gedrückt, ermittelt der EventHandler in einer switch-case Anweisung, ob es eine relevante Taste ist. Ist dies der Fall, wird der zugehörige Case ausgeführt. Wird beispielsweise die "links"-Taste gedrückt, verringert sich die X-Koordinate der Spielfigur über das Destination Rechteck und die Figur bewegt sich somit nach links.
```cpp
switch (e.type){
	case SDL_KEYDOWN:
		if (e.key.keysym.sym == SDLK_LEFT) {
			std::cout << "LEFT down" << "\n" ;
			player.dest.x--;
		}
```

#### Generieren des Spielfeldes
Das Spielfeld wird in einer .map Datei gespeichert. In dieser wird in einer Matrix abgebildet, wie die Karte aufgebaut ist. Die ersten zwei Ziffern beschreiben die Größe der Matrix. Mit der dritten und vierten wird der Startpunkt festgelegt. Jedes weitere Feld beschreibt die Matrix selbst und steht für ein 32x32 Pixel großes Feld auf dem Spielfeld. Mithilfe von Nummern wird dann dargestellt, wie dieses aussehen soll. Die loadmap Funktion soll diese Matrix auslesen und für jedes Feld, dass nicht 0 ist, ein Objekt erzeugen. Diese werden dann in einem Vector gespeichert. Somit ist es möglich, eine Bilddatei mit allen Texturen für die Karte zu erstellen und die zugehörigen Ausschnitte entsprechend rauszuschneiden. Ich habe hier zu Testzwecken eine Datei mit drei unterschiedlichen Feldern erstellt. 
<br/>
Diese sieht wie folgt aus:
<br/>
<img src="https://raw.githubusercontent.com/mpeters4/GP_Peters/gh-pages/docs/img/tiles.png"/>
<br/>
Steht beispielsweise eine 1 in der Matrix, wird die Source für X und Y auf 0 gesetzt und das erste Feld wird verwendet. Bei einer 2 wird bei X=32 angefangen, aus der Textur zu schneiden usw.
<br/>
<img src="https://raw.githubusercontent.com/mpeters4/GP_Peters/gh-pages/docs/img/tiles_numbers.png"/>
```cpp
//INFO: TILE_SIZE = 32
in >> current;
if (current != 0) {
	Object tmp;
	tmp.setImg("model/tiles.png", renderer);
	tmp.setSrc(((current - 1)*TILE_SIZE), 0, TILE_SIZE, TILE_SIZE);
	tmp.setDest((j*TILE_SIZE)+ x, (i*TILE_SIZE) + y, TILE_SIZE, TILE_SIZE);
	map.push_back(tmp);
}
```
Mithilfe von drawmap wird dann das Feld aus dem Objectvektor erstellt. Zur Verdeutlichung habe ich eine kleine Matrix angelegt und daraus ein Bild generiert.
```
5 5		(5x5 Matrix)
0 32		(Beginn bei X=0, Y=32)
00 00 03 00 00 
00 01 01 01 00 
00 02 01 02 00 
00 03 01 01 00 
00 00 02 00 00 
```
Das daraus entstandene Spielfeld sieht wie folgt aus:
<br/>
<img src="https://raw.githubusercontent.com/mpeters4/GP_Peters/gh-pages/docs/img/doc_map_screen.PNG" width="407" height="292"/>