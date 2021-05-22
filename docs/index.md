## Willkommen beim Blog zu meinem Projekt "Climb it"
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
``cpp
SDL_Texture* TextureLoader::LoadTexture(const char* texture, SDL_Renderer* ren){
	SDL_Surface* tempSur = IMG_Load(texture);
	SDL_Texture* tex = SDL_CreateTextureFromSurface(ren, tempSur);
	SDL_FreeSurface(tempSur);
	return tex;
}
``
#### Der Spielcharakter
In den letzten Wochen habe ich außerdem eine erste Idee für das Modell der Spielfigur entworfen. Diese ist noch nicht final, aber reicht vorerst für einige Spieletests aus. Offensichtlich ist die Auflösung noch viel zu gering und ich werde die Texturen höher skalieren und somit neu zeichnen müssen.
<br/>
<img src="https://raw.githubusercontent.com/mpeters4/GP_Peters/gh-pages/docs/img/Player.png"  width="150" height="150" />
<img src="https://raw.githubusercontent.com/mpeters4/GP_Peters/gh-pages/docs/img/Sprite%20Monkey%20figure%20GIF.gif"  width="100" height="100" />

#### Pläne und erste Ideen für die nächste Woche
Nächste Woche soll damit begonnen werden, die Spielfigur mit Tastatureingaben zu steuern. Außerdem möchte ich zusätzliche Objekte einfügen und versuchen, eine Kollision zu erzeugen. Dazu werde ich einen Weg finden müssen, die Objekte auch physisch zu platzieren. Eine erste Idee dazu wäre, eine Matrix aufzustellen und die Objekte darin zu platzieren. Die passende Textur könnte dann einfach an diese entsprechende Stelle gesetzt werden. 
