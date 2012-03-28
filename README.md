### Introduction to TemplateDuino

This is a small perl script which generates a C++ header
file from HTML templates for use with Arduino sketches.
The generated header file contains functions which output
the HTML, one for each template.

The generated functions accept input parameters which
can be used in the templates (as with any other template
toolkit).

Raw HTML will be put into PROGMEM to save memory.

### Structure

Create a new directory for your templates inside your libraries
folder where your sketches reside. Put the templates into this
directory. Templates have to have the .tpl suffix.

### Template Syntax

You need basically two templates: a Base-template and a template
for page output (or more, if there are more pages). The base template
might look like this:

```
<html>
<body>
<% insert content %>
</body>
</html>
```

Lets name it base.tpl. As you can see the file contains plain HTML plus a special
tag. The <% insert content %> marks a block which can be filled in by
your page templates. This allows you to maintain ONE base template containing
design, stylseeht, menu etc and the page templates only the content.

The page template index.tpl will then look like this:

```
<% extend base.tpl %>

<% block content %>
<ul>
  <li>Temperatur: <% float temp %>&deg; C</li>
  <li>Luftfeuchte: <% float humidity %>%</li>
</ul>
<% endblock content %>

```


Here we define, that the index.tpl template is using base.tpl as its base.

And we insert the content with <% block content %> til <% endblock content %>. Everything
in between wil be put into the base.tpl's <% insert content %> tag. Very easy.

Also we are using some variables here, eg: <% float temp %>. The float keyword is
mandatory and tells the compiler the variable type. So use here whatever you are
using in your sketch (and the compiler accepts).


#### Loops

You can also generate loops to display list of data or to create a selection
in a form. In a template you write:

```
<% for pin in PinList pins %>
  <li><% String pin.name %>: <% int pin.value %></li>
<% endfor %>
```

And in your code you add the data structures required for this like so:

```
struct DATA_PinList {
  String name;
  int value;
  int pin;
};

struct DATA_list {
  DATA_PinList pins[3];
};

DATA_PinList button1;
DATA_PinList button2;
DATA_PinList button3;

DATA_list buttonlist;

void setup() {
  button1.name = "Left Button";
  button1.pin  = 10;
  button2.name = "Middle Button";
  button2.pin  = 11;
  button3.name = "Right Button";
  button3.pin  = 12;
  
  buttonlist.pins[0] = button1;
  buttonlist.pins[1] = button2;
  buttonlist.pins[2] = button3;
  
  for (int i=0; i<3; i++) {
     pinMode(buttonlist.pins[i].pin, INPUT);
  }
}
```

And in the webduino handler:

```
    for (int i=0; i<3; i++) {
      buttonlist.pins[i].value = digitalRead(buttonlist.pins[i].pin);
    }
    tpl_list(server, buttonlist);
```

The example sketch contains a working loop template and code.

### Code Generation

Next, execute the script with the directory containing the templates as a parameter:

```
% templateduino.pl Templates 

Created Templates/Templates.h
You should put the directory Templates into your 'libraries' folder
and add the following code to your sketch to use it:


/*
 * Add this code to the top of your sketch
 */
#include "Ethernet.h"
#include "WebServer.h"

struct DATA_index {
  float humidity;
  float temp;
};

char endl = '\n';

template<class T>
inline Print &operator <<(Print &obj, T arg) { obj.print(arg); return obj; }

#include "Templates.h"

/*
 * To call one of the generated template functions, do:
 */

WebServer webserver(PREFIX, 80);

  DATA_index index_vars;
  index_vars.humidity = ...; // fill with appropriate values
  index_vars.temp = ...; // fill with appropriate values

  tpl_index(webserver, index_vars);

```

As you can see, there's a lot of output. Just follow the printed
instructions and put the stuff into your sketch.

Important here is the generated struct variable. Its named and structured
based on your template (DATA_index in our example). You are entitled to
define it appropriately and fill with sencefull values before running
the template function.

The template function (tpl_index here) takes the WebServer object (ArduinoWeb)
and the mentioned struct as parameter.

### Example

Take a look to the example to learn more.

### FIXME

Add documentation and example about the loop feature (foreach), which
is implemented but not documented yet.

### Author

T.Linden

### Version

TemplateDuino Version 0.01

### License and Copyright

Licensed under the terms of the perl artistic license.

Copyright (c) 2012 T.Linden
