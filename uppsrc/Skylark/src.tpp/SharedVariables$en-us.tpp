topic "Shared variables";
[ $$0,0#00000000000000000000000000000000:Default]
[H6;0 $$1,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$2,0#37138531426314131252341829483370:item]
[l288;2 $$3,3#27521748481378242620020725143825:desc]
[i448;a25;kKO9;2 $$4,0#37138531426314131252341829483380:class]
[{_}%EN-US 
[s0;2%- &]
[ {{10000@(113.42.0) [s0; [*@7;4 Shared variables]]}}&]
[s1;%- &]
[s2;:ServeStaticPage`(Http`&`):%- &]
[s0; Shared variables created during client side http requests can 
come from both GET or POST, or a combination of both &]
[s0; Variable names can contain almost any character.&]
[s0; Any variable name which ends with `'[%-*@3 `[`]]`' characters 
is stored by Skylark as ValueArray on server side; otherwise 
it`'s a normal Value.&]
[s0; Array variables are useful during file uploads, for example, 
when multiple files are selected at once.&]
[s0; &]
[s0; Using this http form :&]
[s0; &]
[s0; [2 <form action`=`'upload`' method`=`'POST`' id`=`'myForm`' enctype`=`'multipart/f
orm`-data`'>]&]
[s0; [2 -|<input type`=`'file`' name`=`'][%-*@3;2 FilesToUpload`[`]][2 `' 
multiple`=`'`'>]&]
[s0; [2 </form>]&]
[s0; &]
[s0; On server side you`'ll find following ValueArray variables:&]
[s0; &]
[s0; http`[`"[%-*@3 FilesToUpload`[`]]`"`]-|-|-|-|with transferred file 
contents &]
[s0; http`[`"[%-*@3 FilesToUpload.content`_type`[`]]`"`]-|-|with transferred 
file content types &]
[s0; http`[`"[%-*@3 FilesToUpload.filename`[`]]`"`]-|-|-|with transferred 
file names &]
[s0; &]
[s0; Beware that the `'`[`]`' characters are parts of the variable 
name, to be able to distinguish them from normal Value variables 
without need to test them.]]