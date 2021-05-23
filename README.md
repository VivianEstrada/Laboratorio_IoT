# Laboratorio III 
## Transferencia de datos usando MQTT ESP8266

Este repositorio contiene la codificación, usando el IDE Arduino, para realizar la conexión a un broker **MQTT** usando la plataforma [ioticos.com](https://ioticos.org/mqtt/), el ESP8266 evia datos de temperatura, estado de una puerta y control de los mismos. El sistema de control de temperatura, permite modificar un límite máximo, que al ser superado, acciona un ventilador, simulando la refrigeración del ambiente, en caso de exceder el *límite*, se envía un correo electrónico, informando a un usuario, también se activa esta función, en caso de que se abra o cierre una puerta (servomotor).
