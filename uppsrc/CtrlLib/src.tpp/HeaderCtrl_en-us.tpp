topic "HeaderCtrl";
[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;*@(64)2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[a83;*R6 $$3,0#31310162474203024125188417583966:caption]
[l288;i1121;b17;O9;~~~.1408;2 $$4,0#10431211400427159095818037425705:param]
[i448;a25;kKO9;*@(64)2 $$5,0#37138531426314131252341829483370:item]
[*+117 $$6,6#14700283458701402223321329925657:header]
[{_}%EN-US 
[s3;%- HeaderCtrl&]
[s0; &]
[s0; class_[* HeaderCtrl]&]
[s2; &]
[s2; 
@@image:1350&150
‚ƒØ€˜€€€ƒèÿ€€€€ø¯œí™ÁÂ°ÌŒÅ™˜½˜€úÀÜÁâ¤ŒÀŠÈİ¬ÎÔàÄîÒº›“şë‰Ãšıêşş—ƒÔñÁõœ€€üÙ
òŸÎ€ÔäğõÂ™——Ãêï“ÙÓğ„ğ†Úİ‘Ã§öìøÖ¸òèÊÍşŠÏÍáî¿¸åÛŠ—–Àî¦âûóÄ»’…·®ËºğğÓ«û­î¼œíŞÂ©
›÷å±Ğå‚öåáœää°Š––ÏĞ…¾ò¨È¬áˆ‹Ş•âÖˆä¢½ì†Ê¾¨èÇÔø÷´õ¬´Ÿ¤çÃ°‰ó¹ü¼Î¨»áóè¯·È‚ÖÈÅ»ä­
Á³Â”­«ÄÊõÿÃÁ”•îŠùóĞ¥‡ò¶¢‡¦«ç°º¤ÛÒÁ¼ÌŠÄú®æÎå¶ò¨â½“È¡â¹¬‰üú«íÌÉï™áŠ–•¢ƒøœÎöüËÜ
¼á”½Ä­ö¤á å˜¨Ú¡Ë›’¥Í§­æÔğÎÉı­ÏĞĞÄÓ•Ş¢Ü»ò¸ÀğÊ–¹—¦¥¼Ü÷Ñ¼ßöâÇæ…Ø‹¹üÜ¯€˜‡Áåğö™€°
¤Ã¸—Ñ¨ËÂ€ÑŠ¹ÄùÅ¨©‡Ÿ°œÙ€¸
&]
[s2; &]
[s0; [/ Derived from] Ctrl, [@(0.0.255) CtrlFrame]&]
[s0; &]
[s0; HeaderCtrl is used mainly for table headers. It can be set up 
to a certain column structure (in the code terminology, the individual 
header columns are called [/ tabs]) including the tabs`' visual 
characteristics (tab title names, icons etc.) and behavioral 
constraints (tab width limits, the ability to show / hide individual 
tabs).&]
[s0; &]
[s0; The individual tab widths are defined by three basic properties: 
the [/ logical tab width], its [/ minimum] and [/ maximum pixel width]. 
The minimum and maximum tab width are final pixel values defining 
constraints for physical (final) column widths. On the other 
hand, the logical tab widths are given in arbitrary units, which 
are multiplied by a certain scaling factor whenever the header 
control is laid out to fit the view size. Therefore normally 
their absolute magnitudes don`'t matter, just their ratios.&]
[s0; &]
[s0; The header control can operate in four different visual modes. 
Each mode effectively defines how the logical relative tab widths 
are fitted to the current view size:&]
[s0; &]
[s0;i150;O0; [* Proportional]: the tab widths are proportionally adjusted 
with respect to preset width constraints to fill the current 
view width;&]
[s0;i150;O0; [* ReduceLast]: every time a tab width is set and the total 
tab width exceeds the view size, the following tab widths (tabs 
to the right) are reduced in right`-to`-left order (beginning 
with the rightmost tab) to fit the view size.&]
[s0;i150;O0; [* ReduceNext]: every time a tab width is set and the total 
tab width exceeds the view size, the following tab widths (tabs 
to the right) are reduced in left`-to`-right order (beginning 
with the tab just after the tab being adjusted) to fit the view 
size.&]
[s0;i150;O0; [* Absolute]: the tab widths are never modified automatically, 
the logical header width can be smaller or greater than the actual 
view size. Whenever the total tab width exceeds the view size, 
the header can be scrolled to the left or right to pan its visible 
portion within the parent view.&]
[s0; &]
[s0; From the programmer`'s point of view, the HeaderCtrl takes complete 
control over the distribution of tabs and the user interface 
needed to adjust them. The host control can use the header control 
(typically during [/ Layout] or [/ Paint]) to obtain horizontal pixel 
positions of the individual tab breaks and use them to position 
and paint the column data.&]
[s0; &]
[s0; If Moving modifier is used, HeaderCtrl allows to reorder tabs 
using drag `& drop operation. Client code can identify the order 
using GetTabIndex method, which returns the `"original`" index 
for the tab.&]
[s0; &]
[s0; The HeaderCtrl implements the [/ frame interface]; when attached 
to a parent control using the [/ AddFrame] method, it positions 
itself automatically at the top of the control rectangle (above 
its view).&]
[s0; &]
[s0; In addition to the standard tab title functionality, the rectangular 
areas corresponding to the individual tabs can act as pseudo`-buttons 
invoking a certain functionality when clicked. Each header tab 
has a [/ WhenAction] callback; when set to a non`-null value, the 
header control starts to check for mouse clicks and executes 
the callback whenever the corresponding tab is clicked. When 
used to invoke popups, you can use the [/ GetTabRect] method to 
determine the actual location of a certain header tab and align 
the dropdown control with respect to the tab rectangle location.&]
[s0; &]
[s5;K%- [^`:`:Rect^ Rect]_[@0 GetTabRect]([@(0.0.255) int]_[@3 i])&]
[s2; Returns the bounding box of a certain tab (in coordinates relative 
to the HeaderCtrl view). This can be used by controls using the 
header (like ArrayCtrl) to determine column break locations or 
by host applications for positioning dropdown controls (activated 
upon clicking a tab) relative to the tab box within the header.&]
[s4; [*C@3 i]-|Tab index (zero based).&]
[s4; [*/ Return value]-|rectangular tab bounding box within the HeaderCtrl.&]
[s0;* &]
[s0; [* Hint:] to locate drop`-down controls (e.g. popup menus) relative 
to the tab rectangle, you have to transform the returned rectangle 
to screen coordinates. This can be done for example using the 
following code snippet:&]
[s0; &]
[s0; [C -|Rect tab2`_scr`_rect `= tab.GetTabRect(2) `+ tab.GetScreenView().TopLeft();]&]
[s0;3 &]
[s5;:`:`:HeaderCtrl`:`:Tab`(int`)const: [@(0.0.255) const]_[^`:`:Column^ Column]`&_[@0 Tab](
[@(0.0.255) int]_[@3 i][@0 )_const]&]
[s2; Returns the descriptive [* HeaderCtrl`::Column] structure of a 
given header tab. This constant version can be used to retrieve 
individual tab properties. For a thorough description of the 
[* Column] structure, see below.&]
[s4; [*C@3 i]-|Tab index within the header (zero based).&]
[s4; [*/ Return value]-|a (constant) reference to the [* Column] structure 
describing the tab.&]
[s0;3 &]
[s5;:`:`:HeaderCtrl`:`:Tab`(int`): [^`:`:Column^ Column][%- `&]_[@0 Tab]([@(0.0.255) int]_[@3 i
][@0 )]&]
[s2; Returns a (non`-constant) reference to the descriptive [* HeaderCtrl`::Column] 
structure of a given header tab. This can be used to set and 
retrieve individual tab properties (see below for a full description 
of the [* Column] structure).&]
[s4; [*C@3 i]-|Tab index (zero based).&]
[s4; [*/ Return value]-|a non`-constant reference to the [* Column] structure 
for the given tab.&]
[s0;3 &]
[s5;K%- [^`:`:HeaderCtrl`:`:Column^ Column]`&_[@0 Add]([@(0.0.255) const]_[@(0.0.255) char]_`*
[@3 text], [@(0.0.255) double]_[@3 ratio]_`=_[@3 0])&]
[s2; Adds a new tab after (to the right of) all existing header tabs. 
The function returns a (non`-constant) reference to the [* HeaderCtrl`::Column] 
structure which can be further used to set up additional tab 
properties.&]
[s4; [*C@3 text]-|Tab (title) name.&]
[s4; [*C@3 ratio]-|Logical tab width.&]
[s4; [*/ Return value]-|a non`-constant reference to the [* Column] structure 
describing the newly added tab.&]
[s0;3 &]
[s5;K%- [^`:`:HeaderCtrl`:`:Column^ Column]`&_[@0 Add]()&]
[s2; Adds a new tab after (to the right of) all existing header tabs. 
This is identical to the above version with the [/ text] argument 
set to an empty string and [/ ratio] set to 0.&]
[s4; [*/ Return value]-|a non`-constant reference to the [* Column] structure 
describing the newly added tab.&]
[s0;3 &]
[s5;K%- [@(0.0.255) const]_[^`:`:HeaderCtrl`:`:Column^ Column]`&_[@0 operator`[`]]([@(0.0.255) i
nt]_[@3 i])_[@(0.0.255) const]&]
[s2; Returns a (constant) reference to a given header tab. This is 
identical to the [/ const] version of the [* Tab] method.&]
[s4; [*C@3 i]-|Tab index (zero based).&]
[s4; [*/ Return value]-|a constant reference to the given header tab.&]
[s0;3 &]
[s5;K%- [@(0.0.255) int]_[@0 GetCount]()_[@(0.0.255) const]&]
[s2; Returns the current number of header tabs. Each [* Add] increases 
this value by one and [* Reset] sets it to 0 (the initial value).&]
[s4; [*/ Return value]-|Number of tabs.&]
[s0;3 &]
[s5;K%- [@(0.0.255) void]_[@0 Reset]()&]
[s2; Resets the header tab to its initial state (removes all tabs).&]
[s0;3 &]
[s5;K%- [@(0.0.255) void]_[@0 ShowTab]([@(0.0.255) int]_[@3 i], [@(0.0.255) bool]_[@3 show]_`=_tr
ue)&]
[s2; Shows or hides a certain header tab.&]
[s4; [*C@3 i]-|tab index (zero based)&]
[s4; [*C@3 show]-|flag indicating whether the tab should be shown ([* true]) 
or hidden ([* false]).&]
[s0;3 &]
[s5;K%- [@(0.0.255) void]_[@0 HideTab]([@(0.0.255) int]_[@3 i])&]
[s2; Hides a certain header tab. This is identical to [* ShowTab(i, 
false)].&]
[s4; [*C@3 i]-|tab index (zero based)&]
[s0;3 &]
[s5;K%- [@(0.0.255) bool]_[@0 IsTabVisible]([@(0.0.255) int]_[@3 i])&]
[s2; Checks for visibility of a certain tab.&]
[s4; [*C@3 i]-|tab index (zero based).&]
[s4; [*/ Return value]-|[* false] `= hidden, [* true] `= shown.&]
[s0;3 &]
[s5;K%- [@(0.0.255) void]_[@0 SetTabRatio]([@(0.0.255) int]_[@3 i], [@(0.0.255) double]_[@3 ratio
])&]
[s2; Sets the logical tab width (ratio) of a given tab.&]
[s4; [*C@3 i]-|tab index&]
[s4; [*C@3 ratio]-|logical tab width.&]
[s0;3 &]
[s5;K%- [@(0.0.255) double]_[@0 GetTabRatio]([@(0.0.255) int]_[@3 i])_[@(0.0.255) const]&]
[s2; Retrieves the current logical width of a certain tab. The logical 
tab width can be set using the [* SetTabRatio] method (or using 
the [* SetRatio] method for an individual [* HeaderCtrl`::Column] 
object) or by dragging the tab breaks using the mouse.&]
[s4; [*C@3 i]-|tab index (zero based)&]
[s4; [*/ Return value]-|current logical tab width&]
[s0;3 &]
[s5;K%- [@(0.0.255) void]_[@0 SetTabWidth]([@(0.0.255) int]_[@3 i], [@(0.0.255) int]_[@3 cx])&]
[s2; Sets the given header tab to a physical (pixel) size. This also 
modifies the logical (relative) tab width accordingly.&]
[s4; [*C@3 i]-|tab index (zero based)&]
[s4; [*C@3 cx]-|tab pixel size&]
[s0;3 &]
[s5;K%- [@(0.0.255) int]_[@0 GetTabWidth]([@(0.0.255) int]_[@3 i])&]
[s2; Returns the physical (pixel) width of a given header tab. Remember 
that (unless the HeaderCtrl is in the [* Scroll] mode) the physical 
tab widths can change with the parent view size. Algorithms for 
long`-term manipulation of tab sizes (typically serialization) 
should always count on the logical tab widths.&]
[s4; [*C@3 i]-|tab index (zero based)&]
[s4; [*/ Return value]-|physical (pixel) tab width. Note that this function 
isn`'t [/ const], because the header control uses a lazy algorithm 
to lay out the header control, and during the call to [* GetTabWidth] 
it may show that the tabs need to be newly laid out.&]
[s0;3 &]
[s5;K:`:`:HeaderCtrl`:`:SwapTabs`(int`,int`):%- [@(0.0.255) void]_[@0 SwapTabs]([@(0.0.255) i
nt]_[@3 first], [@(0.0.255) int]_[@3 second])&]
[s2; Swaps tabs [%-*@3 first] and [%-*@3 second].&]
[s0; &]
[s5;K:`:`:HeaderCtrl`:`:MoveTab`(int`,int`):%- [@(0.0.255) void]_[@0 MoveTab]([@(0.0.255) i
nt]_[@3 from], [@(0.0.255) int]_[@3 to])&]
[s2; Moves tab to another position.&]
[s0; &]
[s5;K:`:`:HeaderCtrl`:`:GetTabIndex`(int`):%- [@(0.0.255) int]_[@0 GetTabIndex]([@(0.0.255) i
nt]_[@3 i])&]
[s2; Returns `"original`" index of Tab.&]
[s0; &]
[s5;K:`:`:HeaderCtrl`:`:FindIndexTab`(int`):%- [@(0.0.255) int]_[@0 FindIndexTab]([@(0.0.255) i
nt]_[@3 ndx])&]
[s2; Finds a current position of tab with original index [%-*@3 ndx].&]
[s0; &]
[s5;K%- [@(0.0.255) void]_[@0 StartSplitDrag]([@(0.0.255) int]_[@3 s])&]
[s2; Starts the header tab drag `& drop for a given tab break.&]
[s4; [*C@3 s]-|break index (zero `= left side of tab 0).&]
[s0;3 &]
[s5;K%- [@(0.0.255) int]_[@0 GetSplit]([@(0.0.255) int]_[@3 x])&]
[s2; Locates the header tab break corresponding to a given [/ x] coordinate 
(relative to the header control). This can be used by the parent 
controls to check whether the mouse cursor hovers above a tab 
break (e.g. [* ArrayCtrl] uses this function to check whether to 
display the horizontal resizing cursor).&]
[s4; [*C@3 x]-|horizontal pixel coordinate (relative to the header control 
bounding box).&]
[s4; [*/ Return value]-|Index of tab break (in the range `[0..GetCount()`]) 
or `-1 when there is no tab break in the vicinity of a given 
point.&]
[s0;3 &]
[s5;K%- [@(0.0.255) int]_[@0 GetScroll]()_[@(0.0.255) const]&]
[s2; [* Absolute] mode only: returns pixel offset defining the current 
origin of the (potentially scrolled) header. For non`-scrolling 
visual modes, this function always returns 0 (in non`-scrolling 
modes the left side of first tab always aligns with the left 
side of parent view).&]
[s4; [*/ Return value]-|Number of pixels by which the header control 
is currently scrolled in the horizontal direction. The physical 
[/ x] coordinate of a tab edge (relative to parent view) can be 
calculated by subtracting this value from its logical coordinate 
(relative to leftmost header edge).&]
[s0;3 &]
[s5;K%- [@(0.0.255) bool]_[@0 IsScroll]()_[@(0.0.255) const]&]
[s2; Checks whether the header control is currently in scrolling 
mode (i.e., whether it is in the [* Absolute] visual mode and its 
total tab width exceeds current view size). When [* IsScroll] returns 
[* true], the [* GetScroll] method can be used to retrieve the current 
horizontal scrollbar position.&]
[s4; [*/ Return value]-|[* true] `= scrolling header, [* false ]`= non`-scrolling 
header.&]
[s0;3 &]
[s5;K%- [@(0.0.255) int]_[@0 GetHeight]()_[@(0.0.255) const]&]
[s2; Determines the pixel height of the header control. This is equal 
to the maximum of all the individual tab heights (depending on 
the tab title font and icon sizes).&]
[s4; [*/ Return value]-|HeaderCtrl pixel height.&]
[s0;3 &]
[s5;K%- [^`:`:HeaderCtrl^ HeaderCtrl]`&_[@0 Invisible]([@(0.0.255) bool]_[@3 inv])&]
[s2; When called with a [* true] argument, makes the header zero height 
in frame mode (the tab placement routines still work but the 
header itself remains hidden).&]
[s4; [*C@3 inv]-|[* true] `= hide the header, [* false] `= show it.&]
[s4; [*/ Return value]-|[* `*this]&]
[s0;3 &]
[s5;K%- [^`:`:HeaderCtrl^ HeaderCtrl]`&_[@0 Track]([@(0.0.255) bool]_[@3 `_track]_`=_true)&]
[s2; Sets whether the HeaderCtrl and its parent view contents should 
be `'animated`' during repositioning the tab breaks using mouse 
drag `& drop. When [* `_track] is set to [* true], the header and 
its parents repaint is requested every time the mouse moves during 
the drag `& drop. When set to [* false], the header and the parent 
control get repainted only after the drag `& drop is finished. 
It is wise to set this value depending on the complexity of the 
data shown `'beneath`' the header (usually in an ArrayCtrl) and 
on the expected target hardware because the track mode is much 
more demanding with respect to computational time spent during 
the necessary multiple repaints.&]
[s4; [*C@3 `_track]-|[* true] `= regenerate header and its parent dynamically 
during drag `& drop, [* false] `= only after it`'s finished.&]
[s4; [*/ Return value]-|[* `*this]&]
[s0;3 &]
[s5;K%- [^`:`:HeaderCtrl^ HeaderCtrl]`&_[@0 NoTrack]()&]
[s2; Turns off track mode. This is identical to [* Track(false)].&]
[s4; [*/ Return value]-|[* `*this]&]
[s0;3 &]
[s5;K%- [^`:`:HeaderCtrl^ HeaderCtrl]`&_[@0 Proportional]()&]
[s2; Switches the HeaderCtrl to the [/ proportional] mode. In proportional 
mode, the individual logical tab widths are always recalculated 
to physical (pixel) widths by scaling them using a common multiplication 
factor in order to fit the whole header into the view width.&]
[s4; [*/ Return value]-|[* `*this]&]
[s0;3 &]
[s5;K%- [^`:`:HeaderCtrl^ HeaderCtrl]`&_[@0 ReduceNext]()&]
[s2; Switches the HeaderCtrl to the [/ reduce next] mode. Every time 
a tab width is set in this mode and the total tab width exceeds 
the view size, the following tab widths (tabs to the right) are 
reduced in left`-to`-right order (beginning with the tab just 
after the tab being adjusted) to fit the view size.&]
[s4; [*/ Return value]-|[* `*this]&]
[s0;3 &]
[s5;K%- [^`:`:HeaderCtrl^ HeaderCtrl]`&_[@0 ReduceLast]()&]
[s2; Switches the HeaderCtrl to the [/ reduce last] mode. Every time 
a tab width is set in this mode and the total tab width exceeds 
the view size, the following tab widths (tabs to the right) are 
reduced in right`-to`-left order (beginning with the rightmost 
tab) to fit the view size.&]
[s4; [*/ Return value]-|[* `*this]&]
[s0;3 &]
[s5;K%- [^`:`:HeaderCtrl^ HeaderCtrl]`&_[@0 Absolute]()&]
[s2; Switches the HeaderCtrl to the [/ absolute] (scroll) mode. In 
this mode the tab widths are never modified automatically, the 
logical header width can be smaller or greater than the actual 
view size. Whenever the total tab width exceeds the view size, 
the header can be scrolled to the left or right to pan its visible 
portion within the parent view.&]
[s4; [*/ Return value]-|[* `*this]&]
[s0;3 &]
[s5;K:`:`:HeaderCtrl`:`:Moving`(bool`):%- [^`:`:HeaderCtrl^ HeaderCtrl]`&_[@0 Moving]([@(0.0.255) b
ool]_[@3 b]_`=_true)&]
[s2; Activates mode when tabs can be rearranged by dragging them. 
Default is inactive.&]
[s0;3 &]
[s0;3 &]
[s5;K%- static static_[@(0.0.255) int]_[@0 GetStdHeight]()&]
[s2; Returns the standard header control height (when there are no 
icons and the standard font is used in all tabs).&]
[s4; [*/ Return value]-|&]
[s0;3 &]
[s5;K%- [^`:`:Callback^ Callback]_[@0 WhenLayout]&]
[s2; This callback is executed whenever the header control gets laid 
out (whenever the tab positions and/or widths change).&]
[s0;3 &]
[s5;K%- [^`:`:Callback^ Callback]_[@0 WhenScroll]&]
[s2; This callback is executed whenever the header control gets scrolled 
([/ absolute] mode only).&]
[s0;3 &]
[s5;K%- [^`:`:Callback^ Callback]_[@0 WhenScrollVisibility]&]
[s2; This callback is executes whenever its scrollbar appears or 
disappears (whenever the return value of the [* IsScroll] method 
changes).&]
[s0; &]
[s0; &]
[s0; [*+117 HeaderCtrl`::Column (nested class)]&]
[s2; &]
[s0;%- [%%/ Derived from][%%  ]LabelBase&]
[s0; &]
[s0; The [* HeaderCtrl`::Column] structure represents an individual 
header tab. HeaderCtrl methods for tab insertion and editation 
typically return a reference to this [/ Column] class which can 
be used afterwards to program the various tab properties.&]
[s0; &]
[s0; The set methods return a reference to [* `*this] ; this allows 
multiple tab properties to be set using a single C`+`+ statement 
with repeated use of the dot operator, e.g.:&]
[s0; &]
[s0; [C -|headerctrl.Add().Min(10).Max(20).Margin(3);]&]
[s0;3 &]
[s5;K%- [^`:`:HeaderCtrl`:`:Column^ Column]`&_[@0 Min]([@(0.0.255) int]_[@3 `_min])&]
[s2; Sets minimum tab pixel size. The default value is 0 (the tab 
can be shrunk arbitrarily and can disappear altogether at a certain 
moment).&]
[s4; [%-*C@3 `_min]-|Minimum tab width in pixels.&]
[s4; [*/ Return value]-|[* `*this]&]
[s0;3 &]
[s5;K%- [^`:`:HeaderCtrl`:`:Column^ Column]`&_[@0 Max]([@(0.0.255) int]_[@3 `_max])&]
[s2; Sets maximum tab pixel size. The default value is [/ INT`_MAX] 
(unlimited).&]
[s4; [%-*C@3 `_max]-|Maximum tab width in pixels.&]
[s4; [*/ Return value]-|[* `*this]&]
[s0;3 &]
[s5;K%- [^`:`:HeaderCtrl`:`:Column^ Column]`&_[@0 MinMax]([@(0.0.255) int]_[@3 m], 
[@(0.0.255) int]_[@3 n])&]
[s2; Sets both minimum and maximum tab pixel size at the same time. 
This has the same effect as [* .Min(m).Max(n)].&]
[s4; [%-*C@3 m]-|Minimum tab width in pixels.&]
[s4; [%-*C@3 n]-|Maximum tab width in pixels.&]
[s4; [*/ Return value]-|[* `*this]&]
[s0;3 &]
[s5;K%- [^`:`:HeaderCtrl`:`:Column^ Column]`&_[@0 Fixed]([@(0.0.255) int]_[@3 f])&]
[s2; Sets both minimum and maximum tab pixel size to the same value, 
effectively making the tab width constant. Equal to [* .MinMax(f, 
f)].&]
[s4; [%-*C@3 f]-|Fixed tab width in pixels.&]
[s4; [*/ Return value]-|[* `*this]&]
[s0;3 &]
[s5;K%- [^`:`:HeaderCtrl`:`:Column^ Column]`&_[@0 SetRatio]([@(0.0.255) double]_[@3 ratio])&]
[s2; Sets logical (relative) tab width. The logical tab width (together 
with logical widths of other tabs and the minimum / maximum width 
constraints) is used to calculate the final pixel size of each 
tab according to the current header visual mode.&]
[s4; [%-*C@3 ratio]-|relative tab width (a generic floating`-point value)&]
[s4; [*/ Return value]-|[* `*this]&]
[s0;3 &]
[s5;K%- [^`:`:HeaderCtrl`:`:Column^ Column]`&_[@0 SetMargin]([@(0.0.255) int]_[@3 m])&]
[s2; Sets horizontal tab margin size, the number of pixels to deflate 
the tab rectangle horizontally to obtain the final `"column data`" 
rectangle. In [/ ArrayCtrl], the tab margin areas are used to paint 
the grid lines and the inverted selection rectangle outside of 
the table cells.&]
[s4; [%-*C@3 m]-|Horizontal tab margin size in pixels.&]
[s4; [*/ Return value]-|[* `*this]&]
[s0;3 &]
[s5;K%- [@(0.0.255) int]_[@0 GetMargin]()_[@(0.0.255) const]&]
[s2; Returns current tab margin width in pixels.&]
[s4; [*/ Return value]-|[* `*this]&]
[s0;3 &]
[s5;K%- [^`:`:Callback^ Callback]_[@0 WhenAction]&]
[s2; The tab action callback. When set to a non`-null value, the 
header control starts to check for mouse clicks within this tab 
and calls this callback whenever the corresponding tab is clicked.&]
[s0; &]
[s5;K:`:`:HeaderCtrl`:`:Column`:`:WhenBar:%- [^`:`:Callback1^ Callback1]<Bar`&>_[@0 WhenB
ar]&]
[s2; Provides am optional context menu for header tab.  &]
[s0; ]