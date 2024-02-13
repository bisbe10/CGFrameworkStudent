
Miquel Bisbe -> Nia:267518  correu: astrid.alins01@estudiant.upf.edu
Astrid Alins -> Nia:268621  correu: miquel.bisbe01@estudiant.upf.edu

Escribimos este readme para aclarar brevement algunos aspectos de la practica 2.

Para la representación de las entidades (animación) hemos querido combinar el dibujo de
rayas y puntos intermitentemente. Para ello, hemos creado una variable booleana para
triángulos y otra para puntos. A través de unas condiciones (en formato if) vamos llevando a
cabo renders de esas formas respectivas. Hemos incluido “sleep” de 80 milisegundos entre sus
ejecuciones para que la animación no fuera tan agresiva visualmente. Le hemos añadido la
función “Rotate” para que gire mientras hace el cambio de formato. En nuestra animación
general tenemos una entidad que rota de forma simple, otra que se traslada y otra ultima que
cambia intermitentemente de rayas a puntos mientras rota.

Ahora explicaremos las dinámicas de nuestro mouse enlazado con el movimiento de los puntos
“eye” y “center”. El “eye” de nuestro programa se dirige mediante un ‘click’ derecho con 1
dedo, que se mantiene pulsado durante el traslado de nuestra cámara gracias a la función
“orbit”. Cuando dejamos de pulsar, dejamos de editar las coordenadas de “eye”. En contraste,
nuestro cambio de “center” se activa con 2 dedos pulsados a la vez (derecho), y decidimos
cómo movernos desplazando el mouse (sin mantener pulsado). Para salir de la edición de la
posición del center, hacemos ‘click’ con 1 dedo.

Para la gestión de “far” y “near” nos hemos dado cuenta de que si están configurados muy
cerca, no nos hace los cortes escalonadamente. Por tanto, hemos configurado las posiciones
de near en 0.001 y far en 2. Habrá que pulsar los símbolos +/- repetidas veces para ver los
cortes.