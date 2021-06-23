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
Für das platzieren von Texturen werden neben der Bilddatei selbst zwei Informationen benötigt. Zum einen wird die Quelle des Bildes (Source) gebraucht. Dazu wird mit SDL_Rect ein Rechteck mit dem gewünschten Bildausschnitt erzeugt. Es wird angegeben bei welchen Koordinaten innerhalb der Textur der Ausschnitt beginnt und wie hoch bzw. breit dieser ist. Zum anderen wird der Zielort(Destination) bestimmt. Hier wird ebenfalls ein Rechteck erzeugt. Es werden die Koordinaten des Zielorts und die Größe der Textur benötigt. Mit dem Destination Rechteck ist es zusätzlich möglich, die Textur zu skalieren. <br/>Um das Generieren und rendern von vielen Objekten zu erleichtern, habe ich die Object Klasse hinzugefügt. Diese enthält den Dateipfad und die beiden benötigten SDL_Rect Variablen. Somit wird die zuvor erstellte TextureLoader Klasse vorerst hinfällig und wird eventuell verworfen.
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
Allerdings habe ich bislang noch keinen Weg gefunden, wie ich die Dauer eines Tastendrucks ermitteln kann. Da der EventHandler in einer Schleife jedes mal neu ausgeführt wird, habe ich noch keinen akzeptablen Weg gefunden dieses Problem zu lösen. Damit muss ich mich noch einmal in der kommenden Woche auseinandersetzen.

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
<img src="https://raw.githubusercontent.com/mpeters4/GP_Peters/gh-pages/docs/img/doc_map_screen.PNG" width="326" height="234"/>

### 16.06.2021 Sprünge und Kollision
Der wichtigste Teil des Spiels ist die Steuerung. Wenn diese nicht absolut fehlerfrei funktioniert, geht das gesamte Spielkonzept nicht mehr auf. Die letzten 14 Tage habe ich damit verbracht, diese so zu gestalten, wie ich sie gerne hätte. Leider nicht ganz erfolgreich. Deshalb ist dieser Eintrag auch eher ein Update zum aktuellen Stand als eine finale Beschreibung der Funktionalität.
Folgende Fortschritte wurden in der letzten Woche erzielt:
-Bewegen von Objekten
-Sprung Funktion (teilweise)
-Kollisionsberechnung

#### move Funktion für die Bewegung von Objekten (überarbeitet)
Um vor allem den Spieler zu bewegen, besitzt die Object Klasse jetzt die Move Funktion. Dieser werden die Bewegungsrichtung und die dazugehörige Geschwindigkeit übergeben.
Geschwindigkeit und Richtung werden nun multipliziert und und der Position auf X- bzw. Y-Achse addiert. Anschließend wir die Figur auf die entsprechende Position gesetzt.
Die Variablen sind vom Datentyp float gewählt, da ich für die Sprungkurve teilweise Dezimalzahlen erhalte. Die Position des Objectes wird von SDL allerdings als Integer gespeichert.
Wenn ich diese Dezimalzahlen nicht exta in den Variablen posX und posY speichere, wird die Position nicht exakt gespeichert und es kommt zu Fehlern aufgrund der Ungenauigkeit.

```cpp
void Object::move(float mX, float mY, float velX, float velY) {

	posX += velX * mX;
	posY += velY * mY;
	setPos(posX, posY);
}
```

#### Kollisionsberechnung 
Sobald der Spieler ein Objekt berührt, soll dieser selbstverständlich nicht durchlaufen können, sondern davor stehen bleiben. Dafür muss erfasst werden, ob der Spieler mit einem Objekt kollidiert. Sobald eine Bewegung stattfindet, wird die Position des Spielers mit allen erstellten Objekten verglichen. Wird eine Kollision erfasst, wird die Bewegung noch zurückgesetzt, bevor sie visuell dargestellt wird.
Die Kollision wird berechnet, indem die Koordinaten der Ecken des Rechtecks miteinander verglichen werden. Dazu werden natürlich nur die Seiten verglichen, die sich überhaupt berühren können. 
<br/>Um auch Objekte in den Hintergrund zu setzen, habe ich einfach ein Boolean "solid" gesetzt, der aussagt, ob das Objekt in der Kollisionsberechnung berücksichtigt wird oder nicht. 
```cpp
player.move(mX, mY, velX, velY);
for (int i = 0; i < map.size(); i++) {
	if (map[i].getSolid()) {
	if (checkCollision(player, map[i]) == 1&& map[i].getSolid()) {
			colision = true;
		}
	}
}
	if (colision) {
	player.move(-mX, -mY, velX, velY);
	colision = false;
}
```
Wie genau nach einer Kollision gehandelt wird, hängt von der Bewegungsrichtung ab. Außerdem ist es wichtig, ob der Spieler sich in der Luft oder auf dem Boden befindet.
Ist der Spieler in der Luft und kollidiert beispielsweise mit einer Wand prallt dieser ab und die Flugrichtung ändert sich. Berührt man den Boden, bleibt die Figur stehen.

#### Sprünge
Mit der Leertaste wird der Spielcharakter zum Springen gebracht. Dabei ist relevant, wie lange die Taste gedrückt wird. Je länger diese Taste gedrückt gehalten wird, desto hört ist die Sprungkraft. Wenn die Leertaste über drei Sekunden lang gedrückt gehalten wird, ist die maximale Sprunghöhe erreicht und die Figur springt los. Dies wird mithilfe von SDL_GetTicks() gelöst. Damit bekomme ich einen Zeitstempel im Millisekundenbereich. In Kombination mit einer Hilfvariable, die den Startzeitpunkt festhält, kann die Dauer des Tastendrucks ermittelt werden. Sobald der Boden verlassen wird, ist es nicht mehr möglich, die Figur noch zu bewegen. 
<br/>Der Sprung und das Abprallen funktioniert noch nicht einwandfrei. Ich halte mich mit genauen Beschreibungen der Funktionsweise vorerst zurück, da diese sich noch ändern werden.
#### Fehler (behoben)
Der einzige wirklich ausschlaggebende Fehler ist das Festhängen an der Decke bei Kollision. Sobald der Spieler mit der Oberseite kollidiert, bleibt dieser daran hängen und fällt nicht. Das hat vermutlich damit zu tun, dass die errechnete Höhe noch nicht erreicht ist und somit weiter gesprungen wird, obwohl eine Richtungsänderung vorliegt. Dies bedeutet, dass ich an einem neuen Konzept für die Sprunghöhe arbeiten muss. Meine Idee wäre es, eine Sprungkraft zu definieren, die zeitbedingt abfällt, wenn man sich in der Luft befindet. 
<br/>Ein weiterer Punkt ist die Flugkurve. Diese läuft noch sehr linear und der Spieler spring in einer Dreiecksform. <br/>
<img src="https://raw.githubusercontent.com/mpeters4/GP_Peters/gh-pages/docs/img/Sprung_dreieck.png"/><br/>
Dies ist natürlich keine realistische Sprungkurve. Diese soll zukünftig wie im folgenden Bild aussehen. Dazu muss die Positionsberechnung pro Frame, als schwarze Balken dargestellt, anders erfolgen. Dies lässt sich allerdings auch mithilfe einer Zeitkomponente und SDL_GetTicks() lösen und sollte kein Problem darstellen. Ich möchte vorerst einen problemfreien Sprung erzeugen, bevor ich mich weiter mit der Sprungkurve befasse.
<br/><img src="https://raw.githubusercontent.com/mpeters4/GP_Peters/gh-pages/docs/img/Sprung.png"/><br/>
#### Zukunftsausblick
Sobald der Bewegungsteil erfolgt ist, werden Animationen und Texturen für das Level erstellt. Nachdem dies eingebunden ist, fehlt noch das Übergehen in eine neue Karte, um alle essenziellen Spielfunktionen einzubinden.


### 22.06.2021 Sprünge und Kollision final und Konstante Framerate
Die gesamte Bewegungssteuerung ist abgeschlossen und meines Wissens nach fehlerfrei. Dazu habe ich die gesamte Mechanik der Bewegung noch einmal überarbeitet. 

#### Überarbeitete move Funktion 
Das Grundkonzept der move Funktion ist identisch geblieben. Allerdings gebe ich nur noch jeweils einen Wert für X und Y Achse an. Die Bewegung selbst wird jetzt in der calcMovement Funktion der Game Klasse berechnet und die move Funktion führt nur noch aus. Die Geschwindigkeit des Spielers wird mit velX und velY für beide Achsen bestimmt. 

#### Bewegung
Die Bewegung wird durch Tastatureingaben im EventHandler gesetzt. Die Variablen velDX und velDY bestimmen, wohin sich der Spieler im nächsten Bild bewegt. Bei der Bewegung nach links und rechts wird sich einfach mit der Geschwindigkeit von velX bewegt. Bei einem Sprung wird die Sprungkraft mit der Dauer des Tastendrucks bestimmt. Diese Berechnung hat sich nicht verändert. Sobald der Sprung 2 Sekunden lang gehalten wird, springt die Figur mit voller Kraft ab:
```cpp
//Beispiel zum Sprung mit maximaler Höhe
if ((SDL_GetTicks() - jumpTimer) >= MAX_JUMPTIME && jumpCharge) {
	jumpCharge = false;
	velDY = -velY * (MAX_JUMPTIME * 0.0015);
	velDX = -velX;	
	}
```
Die Bewegung selbst wird von den Funktionen calcMovement und calcAir ausgeführt.

#### Bewegungssteuerung mit calcMovement und calcAir
Eine Bewegung des Spielcharakters soll nur am Boden stattfinden. Während sich dieser in der Luft befindet, darf kein Einfluss mehr auf die Bewegung genommen werden können. Mithilfe von calcAir wird geprüft, ob der Spieler sich in der Luft oder auf dem Boden befindet. 

##### calcAir
Um zu bestimmen, ob der Spieler sich auf dem Boden befindet, wird mit move() eine Bewegung um einen Pixel nach unten ausgeführt und geprüft, ob eine Kollision entsteht. Bei Kollision befindet sich der Spieler logischerweise auf einem Objekt. Wenn nicht, dann ist dieser in der Luft. Dies wird einfach durch das Boolean "air" gesetzt. Wenn der Spieler aus der Luft das erste Mal auf ein Objekt fällt, wird die Bewegung aus der Luft (der Fall) auf 0 gesetzt und die Figur bleibt stehen.

##### calcMovement
Die calcMovement Funktion versucht die Bewegung für den Frame auszuführen. Dabei werden die Bewegungen in X und Y Achse nacheinander ausgeführt, da die Kollision verschieden behandelt wird. Die Bewegung auf der X-Achse wird ausgeführt und bei Kollision wieder zurückgesetzt und somit nicht ausgeführt. Bei Kollision in der Luft wird die Richtung auf der X-Achse zusätzlich geändert, damit die Figur von der Wand abprallt.
```cpp
player.move(velDX, 0);
	for (int i = 0; i < map.size(); i++) {
		if (checkCollision(player, map[i]) == 1 && map[i].getSolid()) {
			player.move(-velDX, 0);
			if (air) {
				velDX = velDX * -1;
			}
		}
	}
``` 
Die Bewegung auf der y-Achse funktioniert ähnlich. Zuerst wird die Figur bewegt und dann auf Kollision geprüft. Bei Kollision wieder zurückgesetzt. Bei Bewegung nach oben prallt die Figur ebenfalls von dem Objekt ab. Bewegt die Figur sich nach unten und kollidiert, bleibt diese direkt stehen. 
In der Luft wird velDY zusätzlich mit einer Schwerkraftsvariable verrechnet. Diese zieht die Figur nach und nach wieder nach unten. Somit wird auch die Sprungkurve realisiert. Die Berechnung rechnet für jedes Bild einen Faktor aus, multipliziert diesen mit der gravity Variable und addiert diesen zu velDY. Dadurch verringert sich die Bewegung nach oben immer weiter, bis sie schließlich die Richtung ändert und ein Fallen ermöglicht. Für die Berechnung wird eine konstante Framerate benötigt, da die Sprünge sonst inkonstant werden. 
```cpp
if (air) {
	//Berechnung des Zeitfaktors dt
	flPrevTime = flCurTime;
	flCurTime = SDL_GetTicks();
	dt = (flCurTime - flPrevTime) * 0.001;
	if (dt >= 0.007) {
		dt = 0.007;
	}
	velDY = velDY + gravity * dt;
}
``` 
Fällt die Figur auf ein Objekt, kann es vorkommen, dass die Bewegung nach unten höher ist als der Abstand zum Boden. Somit wird diese Bewegung zurückgesetzt und die Figur bleibt in der Luft stehen. Um dies zu verhindern, wird die Figur in diesem Fall die restliche Route bewegt und landet immer auf dem Boden.

Die Bewegung im Spiel ist im folgenden Video zu sehen:
{% include youtubePlayer.html id=-KlYuiTADXI %}

#### Konstante Framerate
Um eine konstante Sprungkurve zu ermöglichen, müssen auch die Frames konstant sein. Zusätzlich dazu vermeidet eine Limitierung der Framerate unnötigen Ressourcenverbrauch.
Diese Limitierung ist ziemlich simpel. Es wird zuerst errechnet, wie viel Verzögerung zwischen den Bildern benötigt wird. Wird dieser unterschritten, wird mithilfe von SDL_Delay die nächste Ausführung des Gameloops verzögert.

#### Animation und Texturen

