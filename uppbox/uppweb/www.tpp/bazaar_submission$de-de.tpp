topic "Bazaar - Pakete einreichen und verwalten";
[ $$0,0#00000000000000000000000000000000:Default]
[a83;*R6 $$1,0#31310162474203024125188417583966:caption]
[{_}%DE-DE 
[s1; Bazaar Pakete einreichen und verwalten&]
[s0; [2 Du hast ein Paket, welches für die Upp Gemeinschaft interessant 
sein könnte. Du hast es so gut gemacht wie du nur konntest. 
Der Quellcode ist klar strukturiert und hält sich an den Upp 
Style.]&]
[s0;2 &]
[s0; [2 Darauf können wir nur eines antworten: Dankeschön!]&]
[s0;2 &]
[s0; [2 In diesem Kapitel wird dir erklärt wie du Pakete einreichst 
und sie verwaltest.]&]
[s0;2 &]
[s0;2 &]
[ {{10000f0;g0;^t/25b4/25 [s0;# [*+117 1. Subversion Installation]]}}&]
[s0;2 &]
[s0; [2 Die Pakete des Upp Projekts, und auch von Bazaar, werden über 
das Upp`-Hosting`-System bereitgestellt und die neuesten Releases 
sind über ][^http`:`/`/subversion`.tigris`.org`/^2 Subversion][2  
verfügbar. Dies ist ein quelloffenes Versions`-Kontroll`-System.]&]
[s0;2 &]
[s0; [2 Also musst du zuallerest einen Subversion (Svn) Client wie 
][^http`:`/`/tortoisesvn`.net`/downloads`.html^2 TortoiseSVN][2 , 
][^http`:`/`/www`.syntevo`.com`/smartsvn`/^2 SmartSVN][2 , ][^http`:`/`/www`.sliksvn`.com`/en`/download`/^2 S
likSVN][2 , ][^http`:`/`/kdesvn`.alwins`-world`.de`/^2 kdesvn][2  installieren 
oder, in Linux, einfach nur]&]
[s0;2 &]
[s0;l256; [C$7;2 -|apt`-get install subversion-|]&]
[s0;C$7;2 &]
[s0; [2 in deine Kommandozeile eingeben. In Windows musst du sicherstellen, 
dass der svn`-Client in der PATH Variable enthalten ist, dies 
wird normalerweise durch den Installer des SVN Clients gewährleistest.]&]
[s0;2 &]
[s0;2 &]
[ {{10000f0;g0;^t/25b4/25 [s0;# [*+117 2. Upload Berechtigungen]]}}&]
[s0;2 &]
[s0; [2 So! Du hast svn installiert und kannst jetzt die neuesten Quellen 
vom ][^http`:`/`/code`.google`.com`/p`/upp`-mirror`/^2 Google Code 
mirror][2  mit diesem Befehl herunterladen:]&]
[s0;2 &]
[s0;l256; [C$7;2 -|svn checkout http://upp`-mirror.googlecode.com/svn/trunk/my`-upp`-mirr
or]&]
[s0;2 &]
[s0;2 &]
[s0; [2 Um die Berechtigung zum Uploaden zu erhalten, musst du im Forum/Bazaar 
eine Nachricht schreiben, darin über dein Projekt so viele Details 
wie möglich beschreiben und um die Berechtigung, es in Bazaar 
upzuloaden, bitten.]&]
[s0;2 &]
[s0; [2 Wenn das Projekt akzeptiert wird (was i.d.R. der Fall ist), 
wird ein ][^http`:`/`/www`.ultimatepp`.org`/forum`/index`.php`?t`=usrinfo`&id`=3`&^2 U
pp Administrator][2  dich kontaktieren und dir die Adresse geben, 
auf die du dein neues Paket uploaden kannst. (Außerdem den benötigten 
Benutzernamen (dein Upp Forumsbenutzername) und das Passwort.)]&]
[s0;2 &]
[s0; [2 Wenn die svn Basisadresse z.b.]&]
[s0;l256; [C$7;2 -|svn://192.168.0.1-|]&]
[s0; [2 lautet, musst du dein Paket zu]&]
[s0;l256; [C$7;2 -|svn://192.168.0.1/upp/trunk/bazaar/Mein`_tolles`_Paket-|]&]
[s0; [2 uploaden.]&]
[s0;> [1 (Du solltest idealerweise ][*1 englische Namen][1  verwenden um 
die Einheitlichkeit zu gewährleisten!)]&]
[s0;2 &]
[s0; [2 Bitte vermeide Leerzeichen in deinem Paketnamen. Du musst aufpassen, 
andere Pakete nicht zu verändern (du hast möglicherweise auch 
auf diese Schreibzugriff).]&]
[s0;2 &]
[s0; [2 Wenn du die Änderungen commitest (sie also auf den svn Server 
lädst), vergiss nicht eine Beschreibung in diesem Format beizufügen:]&]
[s0;2 &]
[s0;l256; [C$7;2 -|`"Mein`_tolles`_Paket: First release`"]&]
[s0;2 &]
[s0; [2 oder]&]
[s0;2 &]
[s0;l256; [C$7;2 -|`"Mein`_tolles`_Paket: Added X an Y. Fixed bug Z`"]&]
[s0;2 &]
[s0;2 &]
[s0; [2 Der Google Mirror (][^http`:`/`/code`.google`.com`/p`/upp`-mirror`/source`/list^2 h
ttp://code.google.com/p/upp`-mirror/source/list][2 ) wird deine 
Änderung innerhalt einer Stunde übernommen haben. Dann siehst 
du deinen neuen Release in Bazaar `- Gratulation, dein erster 
Release ist über die Bühne!]&]
[s0;2 &]
[s0; [2 Alle Bazaar Verzeichnisse sind offen, passe also auf, nicht 
direkt die Inhalte der Verzeichnisse zu verändern, die du nicht 
verwaltest. Wenn du etwas verändern willst, folge den ][^topic`:`/`/uppweb`/www`/bazaar`_support`$de`-de^2 B
azaar Support][2  Anweisungen.]&]
[s0;2 &]
[s0; [2 Und quetsche nicht zu viele verschiedene Features in ein einzelnes 
Commit. Wenn sie verschiedene Dinge tun, ist es besser sie zu 
trennen.]&]
[s0;2 &]
[s0;2 &]
[ {{10000f0;g0;^t/25b4/25 [s0;# [*+117 3. Webveröffnetlichung des Pakets]]}}&]
[s0;2 &]
[s0; [*2 3.1 Einführung]&]
[s0;2 &]
[s0; [2 Du bist stolz auf dein neues Paket, hast alle im Forum/Bazaar 
informiert und es auf den svn geuploaded, so dass es jeder nutzen 
kann.]&]
[s0;2 &]
[s0; [2 Jetzt ist es an der Zeit deine eigene Webseite zu erstellen, 
die jedem zeigt wofür dein Paket gut ist, welche Technik verwendet 
wurde, Beispiele und eine Referenz bietet.]&]
[s0;2 &]
[s0; [2 Das ist sehr einfach. Folge einfach den folgenen Anweisungen.]&]
[s0;2 &]
[s0;2 &]
[s0; [*2 3.2 Upp Web Bearbeitung]&]
[s0;2 &]
[s0;i150;O0; [2 Hol dir zuerst die neuesten svn Quellen. Das ist wichtig 
da du die Webseite bearbeiten wirst.]&]
[s0;2 &]
[s0;i150;O0; [2 Öffne das uppbox/uppweb Paket, wähle www.tpp und 
das bazaar`&en`-us Dokument.]&]
[s0;2 &]
[s0;i150;O0; [2 Bearbeite hier einfach die `"Maintained Packages List`" 
indem du dein Projekt hinzufügst oder die Daten deines bestehenden 
Projekts bearbeitest. Benutze exakt den gleichen Paketnamen unter 
Beachtung der Gross`- und Kleinschreibung. Achte darauf die Spalten 
`"Last Release`" und `"Size`" leer zu lassen. Hierdurch werden 
diese Daten durch die Web Infrastruktur aktualisiert.]&]
[s0;2 &]
[s0;i150;O0; [2 Zum Schluss lädst du die Änderungen hoch indem du 
Projekt/SVN/Synchronize ../upp/uppbox/uppweb auswählst.]&]
[s0;2 &]
[s0;2 &]
[s0; [2 Mach das vorsichtig und schnell. Da dies die Index`-Seite ist, 
könnte es sein, dass jemand anderes sie zur selben Zeit bearbeitet 
und ein paar der Änderungen in Konflikt zueinander stehen.]&]
[s0;2 &]
[s0; [2 Bitte bearbeite die Inhalte anderer nicht direkt. Es ist besser 
zuerst die Änderungen vorzuschlagen und um Erlaubnis zu fragen.]&]
[s0;2 &]
[s0; [2 Danach wirst du am darauffolgenden Tag sehen, dass die Webseite 
aktualisiert wurde. Das Geheimnis dahinter ist, dass das Upp`-Hosting`-System 
das uppweb Paket ausführt und das Index Paket und das T`+`+ 
Hilfesystem in .html Webseiten umwandelt.]&]
[s0;2 &]
[s0;2 &]
[s0; [*2 3.3 Bazaar Projekte `- Grundsätzliche Daten]&]
[s0;2 &]
[s0; [2 Die Daten, die in die `"Maintained Packages List`" einzutragen 
sind, sind:]&]
[s0;2 &]
[s0;i150;O0; [2 Package name: Ganz wie du willst `- versuche aber nicht 
Leerzeichen einzufügen. Binde die T`+`+ Hauptseite deines Projekts 
als Hyperlink mit ein. Dies wird die erste Seite, auf die von 
der ][^topic`:`/`/uppweb`/www`/bazaar`$en`-us^2 Bazaar`-Einführungsseite][2  
verlinkt wird.]&]
[s0;i150;O0; [2 Basic Description `- Die Beschreibung des Pakets]&]
[s0;i150;O0; [2 Authors `- Du und evtl. noch andere Autoren]&]
[s0;i150;O0; [2 Status `- Status]&]
[s0;l160;i150;O0; [2 Launch]&]
[s0;l160;i150;O0; [2 Alpha]&]
[s0;l160;i150;O0; [2 Beta]&]
[s0;l160;i150;O0; [2 Production]&]
[s0;l160;i150;O0; [2 Not Maintained]&]
[s0;i150;O0; [2 External Dependencies: Gib hier die, von Upp und Bazaar, 
externen Abhängigkeiten (Bibliotheken oder Programme) an, die 
benötigt werden]&]
[s0;i150;O0; [2 Supported OS `- Unterstützte Betriebssysteme]&]
[s0;i150;O0; [2 Image: Hier kannst du ein Bild einfügen]&]
[s0;2 &]
[s0;2 &]
[s0; [*2 3.4 Deine Webseite]&]
[s0;2 &]
[s0; [2 Nun hast du die Möglichkeit jedem zu zeigen, was dein Paket 
macht. Es ist einfach.]&]
[s0;2 &]
[s0; [2 Erstelle in T`+`+ deine Hilfedateien wie sonst auch: Referenz, 
Beispiele, Implementation, Zusammenfassung.]&]
[s0;2 &]
[s0; [2 Achte darauf, dass alle Seiten untereinander und mit der T`+`+ 
Hilfe`-Hauptseite verlinkt sind. Wenn eine Seite unverlinkt bleibt, 
taucht sie nicht im Web auf.]&]
[s0;2 &]
[s0; [2 Eine gute Hauptseite kann die Seite mit der Zusammenfassung 
sein. Du musst darauf achten, dass diese Seite von der ][^topic`:`/`/uppweb`/www`/bazaar`$en`-us^2 B
azaar Einführungsseite][2  über den Paketnamen verlinkt wird.]&]
[s0;2 &]
[s0; [2 Danach wirst du am darauffolgenden Tag sehen, dass die Webseite 
aktualisiert wurde. Das Geheimnis dahinter ist, dass das Upp`-Hosting`-System 
das uppweb Paket ausführt und das Index Paket und das T`+`+ 
Hilfesystem in .html Webseiten umwandelt.]&]
[s0;2 &]
[s0;2 &]
[s0; [*2 3.5 Wenn überhaupt nichts funktioniert]&]
[s0;2 &]
[s0; [2 Wenn gar nichts funktioniert oder du hiermit nicht zurecht 
kommst, kannst du im ][^http`:`/`/www`.ultimatepp`.org`/forum`/^2 Forum][2  
um Hilfe fragen oder einfach ][^http`:`/`/www`.ultimatepp`.org`/forum`/index`.php`?t`=usrinfo`&id`=648`&^2 m
ir][2  eine Nachricht schicken. Ich mach das dann für dich.]&]
[s0;2 &]
[s0;2 &]
[ {{10000f0;g0;^t/25b4/25 [s0;# [*+117 4. Dokumentation und Beispiele]]}}&]
[s0;2 &]
[s0; [2 Normalerweise denken wir, dass wir gute Programmierer sind 
und unser Quellcode einfach so kristallklar ist, dass ein kurzes 
überfliegen der .h und der .c/.cpp Dateien ausreicht, damit 
jeder Benutzer weiß, wie er unserer wertvollen Bibliotheken 
zu nutzen hat.]&]
[s0;2 &]
[s0; [2 Jedoch benötigen Programmierer in der realen Welt, welche 
potentiell die Nutzer unserer Bibliotheken sind, einen gewissen 
Aufwand um einfache und klare Beispiele und Dokumentationen zu 
erstellen.]&]
[s0;2 &]
[s0; [2 Das ist den Aufwand wert. Die Beispiele ermöglichen es den 
Nutzern, unseren Quellcode schnell auszuprobieren und den grunsätzlichen 
Umgang damit zu verstehen. Auf der anderen Seite hilft eine Klassen`- 
und Funktionsreferenz den Nutzern viel unseres Könnens abzuschauen.]&]
[s0;2 &]
[s0; [2 Also sei nicht zaghaft. Denk an deine Nutzer und investiere 
in die T`+`+ Hilfe! Denk daran, dass sie für jeden auf der Webseite 
verfügbar sein wird.]&]
[s0;2 &]
[s0;2 &]
[ {{10000f0;g0;^t/25b4/25 [s0;# [*+117 5. Paket `- Releases]]}}&]
[s0;2 &]
[s0; [2 Warte nicht zu lange neue Releases herauszubringen. Warte nicht 
bis zu wichtige Wegpunkte erreicht hast um von Version 2.0 auf 
3.0 umzusteigen... Folge den Hinweisen im Buch ][^http`:`/`/de`.wikipedia`.org`/wiki`/Die`_Kathedrale`_und`_der`_Basar^2 `"
Die Kathedrale und der Basar`"][2 :]&]
[s0;2 &]
[ {{10000<576;>768;f0;g0;^t/25b4/25@(229) [s0;= [/2 `"Veröffentliche früh. Veröffentliche häufig. Und höre 
auf die Benutzer`"]]}}&]
[s0;l288;/$7;2 &]
[s0;# [2 Und den daran, dass wenn dein Paket nicht innerhalb eines 
Jahres geupdated worden ist, es als `"Nicht gewartet`" (`"Not 
Maintained`") behandelt wird.]&]
[s0;#2 &]
[s0;2 &]
[ {{10000f0;g0;^t/25b4/25 [s0;# [*+117 6. Paket Lizenzen]]}}&]
[s0;2 &]
[s0; [2 Alle Pakete, Dokumente und jeglicher Quellcode in Bazaar steht 
unter der ][^topic`:`/`/ide`/app`/About`$de`-de^2 New BSD license][2 .]&]
[s0;2 &]
[s0;# [2 Jede Software, jedes Dokument oder anderes Medium, welches 
zum Paket gehört, wie Plugins, sollte die gleiche oder eine 
eine gleichartige Lizenz haben.]&]
[s0;#2 &]
[s0;# [2 Zum Beispiel hat das Tcc Paket die BSD Lizenz aber der zugehörige 
libtcc Quellcode und die Bibliothek haben die LGPL Lizenz.]&]
[s0;# [2 Da Tcc ein libtcc Wrapper ist, muss es die LGPL Lizenz besitzen, 
um libtcc nutzen zu können. Doch denke daran, das Tcc Paket 
hat die BSD Lizenz.]&]
[s0;#2 &]
[s0;# [2 Vergiss nicht, dich an die Lizenzbestimmungen zu halten. Open 
Source und Creative Commons Lizenzen sind sehr einfach zu erfüllen 
aber du musst es tun.]&]
[s0;2 &]
[s0;2 &]
[ {{10000f0;g0;^t/25b4/25 [s0;# [*+117 7. Denke daran]]}}&]
[s0;2 &]
[s0; [2 Ein gutes Paket muss verfügbar sein und gewartet werden, also 
wenn:]&]
[s0;i150;O0; [2 du die Paketziele nicht in absehbarer Zeit erfüllen 
kannst]&]
[s0;i150;O0; [2 du einfach kaputt bist]&]
[s0;i150;O0; [2 du beschäftigt bist]&]
[s0;2 &]
[s0; [2 ...dann frage einfach um Hilfe. Schreib es im Forum/Bazaar, 
füge bei deinem Paket bei `"Author`" ein `"Help Required`" hinzu 
oder setze `"Status`" auf `"Not Maintained`". Dies gibt den Leuten 
die Möglichkeit dir zu helfen oder dir etwas abzunehmen.]&]
[s0;2 &]
[s0; [2 Und die Gemeinschaft wird es dir danken.]&]
[s0;%% ]