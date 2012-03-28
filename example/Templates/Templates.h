P(list_content_2) = "<ul>";
P(list_content_5) = "</ul>";
P(list_content_1) = "<p>PIN List:</p>";
P(base_content_left) = 
  "<html>"
  "<body>";
P(base_content_right) = 
  "</body>"
  "</html>";
P(index_content_4) = "</ul>";
P(index_content_1) = "<ul>";

void tpl_list(WebServer &server, DATA_list data) {
  server.printP(base_content_left);

  server.printP(list_content_1);

  server.printP(list_content_2);

  for(int i = 0; i < (sizeof(data.pins) / sizeof(DATA_PinList)); i++) {
    server << "<li>" << data.pins[i].name << ": " << data.pins[i].value << "</li>" << endl;

  }
  server.printP(list_content_5);

  server.printP(base_content_right);

}


void tpl_index(WebServer &server, DATA_index data) {
  server.printP(base_content_left);

  server.printP(index_content_1);

  server << "<li>Temperatur: " << data.temp << "&deg; C</li>" << endl;

  server << "<li>Luftfeuchte: " << data.humidity << "%</li>" << endl;

  server.printP(index_content_4);

  server.printP(base_content_right);

}

