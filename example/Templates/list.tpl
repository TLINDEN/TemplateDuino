<% extend base.tpl %>

<% block title %>
PIN List
<% endblock title %>

<% block content %>
<p>PIN List:</p>
<ul>
<% for pin in PinList pins %>
  <li><% String pin.name %>: <% int pin.value %></li>
<% endfor %>
</ul>
<% endblock content %>
