topic "Slicing HelloWorld U++ example";
[2 $$0,0#00000000000000000000000000000000:Default]
[l288;i1120;a17;O9;~~~.1408;2 $$1,0#10431211400427159095818037425705:param]
[a83;*R6 $$2,5#31310162474203024125188417583966:caption]
[b83;*4 $$3,5#07864147445237544204411237157677:title]
[i288;O9;C2 $$4,6#40027414424643823182269349404212:item]
[b42;a42;2 $$5,5#45413000475342174754091244180557:text]
[l288;b17;a17;2 $$6,6#27521748481378242620020725143825:desc]
[l321;t246;C@5;1 $$7,7#20902679421464641399138805415013:code]
[b2503;2 $$8,0#65142375456100023862071332075487:separator]
[*@(0.0.255)2 $$9,0#83433469410354161042741608181528:base]
[t4167;C2 $$10,0#37138531426314131251341829483380:class]
[l288;a17;*1 $$11,11#70004532496200323422659154056402:requirement]
[i417;b42;a42;O9;~~~.416;2 $$12,12#10566046415157235020018451313112:tparam]
[b167;C2 $$13,13#92430459443460461911108080531343:item1]
[i288;a42;O9;C2 $$14,14#77422149456609303542238260500223:item2]
[*@2$(0.128.128)2 $$15,15#34511555403152284025741354420178:NewsDate]
[l321;*C$7;2 $$16,16#03451589433145915344929335295360:result]
[l321;b83;a83;*C$7;2 $$17,17#07531550463529505371228428965313:result`-line]
[l160;t4167;*C+117 $$18,5#88603949442205825958800053222425:package`-title]
[2 $$19,0#53580023442335529039900623488521:gap]
[t4167;C2 $$20,20#70211524482531209251820423858195:class`-nested]
[b50;2 $$21,21#03324558446220344731010354752573:Par]
[{_}%RU-RU 
[s0; &]
[s0; Исторически почти каждый язык или 
среда программирования имеют различного 
вида примеры типа `"Hello world!`", и Ultimate`+`+ 
не является исключением :). В этом коротком 
уроке я постараюсь объяснить Вам 
основы U`+`+ приложения с помощью HelloWorld 
U`+`+ примера.&]
[s0; Когда вы запустите приложение HelloWorld 
и выберете пункт меню `"File`->About`" вы 
увидите что`-то вроде этого:&]
[s0; &]
[s0;= 
@@image:2162&1572
(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)
&]
[s0; &]
[s0; Давайте изучим исходный код HelloWorld 
U`+`+ приложения:&]
[s0; &]
[s7; #include <CtrlLib/CtrlLib.h>&]
[s7; &]
[s7; class HelloWorld : public TopWindow `{&]
[s7; -|MenuBar menu;&]
[s7; -|StatusBar status;&]
[s7; -|&]
[s7; -|void FileMenu(Bar`& bar);&]
[s7; -|void About();&]
[s7; &]
[s7; public:&]
[s7; -|typedef HelloWorld CLASSNAME;&]
[s7; &]
[s7; -|HelloWorld();&]
[s7; `};&]
[s7; &]
[s7; void HelloWorld`::About()&]
[s7; `{&]
[s7; -|PromptOK(`"`{`{1`@5 `[`@9`= This is the`]`::`@2 `[A5`@0 Ultimate```+```+ 
Hello world sample`}`}`");&]
[s7; `}&]
[s7; &]
[s7; void HelloWorld`::FileMenu(Bar`& bar)&]
[s7; `{&]
[s7; -|bar.Add(`"About..`", THISBACK(About));&]
[s7; -|bar.Separator();&]
[s7; -|bar.Add(`"Exit`", THISBACK(Close));&]
[s7; `}&]
[s7; &]
[s7; HelloWorld`::HelloWorld()&]
[s7; `{&]
[s7; -|AddFrame(menu);&]
[s7; -|AddFrame(status);&]
[s7; -|menu.Add(`"File`", THISBACK(FileMenu));&]
[s7; -|status `= `"Welcome to the Ultimate`+`+ !`";&]
[s7; `}&]
[s7; &]
[s7; GUI`_APP`_MAIN&]
[s7; `{&]
[s7; -|SetLanguage(LNG`_ENGLISH);&]
[s7; -|HelloWorld().Run();&]
[s7; `}&]
[s0;3 &]
[s0; Много еще неизвестных Вам инструментов, 
но давайте не будем торопиться, разберем 
код построчно.&]
[s0; &]
[s0; В первую очередь, мы подключаем `"CtrlLib/CtrlLib.h`";
 этот заголовок включает в себя большую 
часть U`+`+ виджетов. Все U`+`+ виджеты 
порождены от базового класса [^topic`:`/`/CtrlCore`/src`/Ctrl`$en`-us`#`:`:Ctrl`:`:class^ C
trl], поэтому их также называют `"Ctrls`" 
(контролы).&]
[s0; &]
[s0; Следующий шаг `- наследуем HelloWorld от 
класса [^topic`:`/`/CtrlCore`/src`/TopWindow`$en`-us`#`:`:TopWindow`:`:class^ T
opWindow], который представляет из себя 
базовое окно, интерфейс работы с которым 
не зависит от особенностей операционной 
системы. Класс содержит множество 
инструментов, таких как, например, 
заголовок, кнопки закрытия/свертывания 
окна и т. п. Иначе говоря, это наше 
стандартное окно, ;).&]
[s0; &]
[s0; Окно HelloWorld имеет главное меню и строку 
статуса, которые в классе описаны 
как два private члена.&]
[s0; &]
[s7; MenuBar menu;&]
[s7; StatusBar status; &]
[s7; &]
[s0; &]
[s0; В конструкторе HelloWorld с помощью [^topic`:`/`/CtrlCore`/src`/Ctrl`$en`-us`#`:`:Ctrl`:`:AddFrame`(`:`:CtrlFrame`&`)^ A
ddFrame] функции добавлены главное меню 
и строка статуса. AddFrame добавляет данные 
элементы как [^topic`:`/`/CtrlCore`/srcdoc`/AboutFrames`$en`-us^ Frames] 
(рамки), помещая их на границу окна 
и уменьшая видимую область окна. И 
главное меню, и строка статуса знают 
где должно быть место их размещения, 
когда они используются в качестве 
рамок (главное меню вверху, строка 
статуса внизу).&]
[s0; &]
[s0; Следующая строка в конструкторе 
очень интересна:&]
[s0; &]
[s7; menu.Add(`"File`", THISBACK(FileMenu));&]
[s0; &]
[s0; и заслуживает более подробного объяснения. 
U`+`+ использует [^topic`:`/`/Core`/src`/Callbacks`$en`-us^ callbacks] 
`- callbacks могут быть описаны как весьма 
обобщенная форма указателей на функции. 
Каждый Callback представляет из себя 
некоторое действие (обычно вызов 
некоторой определенной функции или 
же метода класса), которое может быть 
вызвано в любой момент. Сейчас, &]
[s0; &]
[s7; THISBACK(x)&]
[s0; &]
[s0;  это макрос, который разворачивается 
в&]
[s0; &]
[s7; callback(this, `&CLASSNAME`::[@3 x][@0 )]&]
[s0;*C@3 &]
[s0; где x `- это название метода, который 
мы хотим вызывать этим действием. 
Функция [^topic`:`/`/Core`/src`/Callbacks`$en`-us`#`:`:callback`(OBJECT`*`,void`(METHOD`:`:`*`)`(P1 p1`)`)^ c
allback] `- шаблонная функция, для ее работы 
необходимо определить переменную 
CLASSNAME. Каждый U`+`+ класс, который использует 
[^topic`:`/`/Core`/src`/Callbacks`$en`-us`#`:`:THISBACK`(x`)^ THISBACK] 
макрос содержит следующую строку&]
[s0; &]
[s7; typedef HelloWorld CLASSNAME;&]
[s0;3 &]
[s0;3 &]
[s0; То есть&]
[s0;3 &]
[s7; menu.Add(`"File`", THISBACK(FileMenu));&]
[s0; &]
[s0; добавляет меню `"File`" в главное меню 
нашего приложения и, когда пользователь 
выберет это меню, U`+`+ вызовет соответствующу
ю функцию из нашего HelloWorld&]
[s0;3 &]
[s7; void HelloWorld`::FileMenu(Bar`& bar)&]
[s7; `{&]
[s7; -|bar.Add(`"About..`", THISBACK(About));&]
[s7; -|bar.Separator();&]
[s7; -|bar.Add(`"Exit`", THISBACK(Close));&]
[s7; `}&]
[s0;3 &]
[s0; Что произойдет теперь: после выбора 
`"File`" меню пользователем, U`+`+ создает 
раскрывающееся меню и затем вызывает 
FileMenu функцию, параметром которой 
является адрес только что созданного 
главного меню. После этого FileMenu добавляет 
`"About`" и `"Exit`" в меню, с разделителем 
между ними (Separator), и назначает пунктам 
вызовы функций из нашего HelloWorld объекта 
(более детальное обсуждение указателей 
на функции [^topic`:`/`/CtrlLib`/srcdoc`/MenuCb`$en`-us^ здесь]).&]
[s0;@5;3 &]
[s0; Последняя строка конструктора&]
[s0;3 &]
[s7; status `= `"Welcome to the Ultimate`+`+ !`";&]
[s0; &]
[s0; Класс StatusBar имеет перегруженный оператор 
`=, который устанавливает текст нашей 
строки статуса. Красиво и чисто.&]
[s0; &]
[s0; Последняя функция HelloWorld класса&]
[s0;3 &]
[s7; void HelloWorld`::About()&]
[s7; `{&]
[s7; -|PromptOK(`"`{`{1`@5 `[`@9`= This is the`]`::`@2 `[A5`@0 Ultimate```+```+ 
Hello world sample`}`}`");&]
[s7; `}&]
[s0;3 &]
[s0; Очевидно, когда пользователь кликает 
по `"File`"`->`"About`" в меню вызывается именно 
эта функция. PromtOK `- удобная глобальная 
функция, описанная в `"CtrlLib`\RichText.h`", 
отображает простой диалог со значком 
информации и кнопкой OK. Также, этот 
диалог содержит информационное сообщение 
для пользователя, и этот текст является 
параметром PromtOK функции. Как вы заметили, 
есть странные символы в нашем информационно
м сообщении, это [^topic`:`/`/RichText`/srcdoc`/QTF`$en`-us^ QTF 
коды] (QTF `- родной формат U`+`+ форматированных 
текстов).&]
[s0; &]
[s0; И в конце мы имеем &]
[s0; &]
[s7; GUI`_APP`_MAIN&]
[s7; `{&]
[s7; -|SetLanguage(LNG`_ENGLISH);&]
[s7; -|HelloWorld().Run();&]
[s7; `}&]
[s0; &]
[s0; Как вы догадываетесь, каждая программа 
должна иметь точку входа, где начинается 
ее выполнение. U`+`+ код начинается 
с макроса GUI`_APP`_MAIN. Этот макрос инициализируе
т U`+`+ GUI приложение для соответствующей 
ОС платформы, а также завершает работу 
U`+`+ среды.&]
[s0; Каждая хорошая среда разработки 
U`+`+ имеет хорошую [^topic`:`/`/Core`/srcdoc`/i18n`$en`-us^ подде
ржку] мультиязычных программ, но для 
примера основным языком мы установили 
Английский.&]
[s0; Далее мы создаем один экземпляр (объект) 
класса HelloWorld и вызываем [^topic`:`/`/CtrlCore`/src`/TopWindow`$en`-us`#`:`:TopWindow`:`:Run`(bool`)^ R
un] функцию, определенную в TopWindow классе, 
которая запустит наше приложение[@5 .]&]
[s0;3 &]
[s0; ]]