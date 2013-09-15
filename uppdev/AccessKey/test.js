var s1 = new String("Hello !");
var s2 = new String("Hello !");
s1 == s2; // Is false, because they are two distinct objects.
s1.valueOf() == s2.valueOf(); // Is true.

//Automatic type coercion
f1(true  ==    2 ); // false... true  → 1 !== 2 ←  2
alert2(false ==    2 ); // false... false → 0 !== 2 ←  2
alert3(true  ==    1 ); // true.... true  → 1 === 1 ←  1
alert4(false ==    0 ); // true.... false → 0 === 0 ←  0
alert5(true  ==   "2"); // false... true  → 1 !== 2 ← "2"
alert6(false ==   "2"); // false... false → 0 !== 2 ← "2"
alert7(true  ==   "1"); // true.... true  → 1 === 1 ← "1"
alert8(false ==   "0"); // true.... false → 0 === 0 ← "0"
alert9(false ==   "" ); // true.... false → 0 === 0 ← ""
alert(false ==   NaN); // false... false → 0 !== NaN
 
//Type checked comparison (no conversion of types and values)
alert(true === 1);     // false... data types do not match
 
//Explicit type coercion
alert(true === !!2);   // true.... data types and values match
alert(true === !!0);   // false... data types match but values differ
alert( 1  ? true : false); // true.... only ±0 and NaN are “falsy” numbers
alert("0" ? true : false); // true.... only the empty string is “falsy”
alert(Boolean({}));    // true.... all objects are “truthy” except null

var d = new Date(2010, 2, 1, 14, 25, 30); // 2010-Mar-01 14:25:30
 
// Displays '2010-3-1 14:25:30':
alert(d.getFullYear() + '-' + (d.getMonth()+1) + '-' + d.getDate() + ' '
      + d.getHours() + ':' + d.getMinutes() + ':' + d.getSeconds());
 
// Built-in toString returns something like 'Mon Mar 01 2010 14:25:30 GMT-0500 (EST)':
alert(d);

s.length 

var myRe = /(\d{4}-\d{2}-\d{2}) (\d{2}:\d{2}:\d{2})/;
var results = myRe.exec("The date and time are 2009-09-08 09:37:08.");
if (results) {
    alert("Matched: " + results[0]); // Entire match
    var my_date = results[1]; // First group == "2009-09-08"
    var my_time = results[2]; // Second group == "09:37:08"
    alert("It is " + my_time + " on " + my_date);
} else alert("Did not find a valid date!");

try
