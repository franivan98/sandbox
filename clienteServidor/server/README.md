## Como probar:
1) ejecutar ./server.
2) en otra terminal, ejecutar: 
echo "hola servidor" | nc localhost 8080
nc es un programa que:

abre una conexión TCP

a un host y puerto

y envía lo que reciba por stdin

📌 nc = cliente TCP genérico

4️⃣ Todo junto (qué pasa realmente)

echo produce:

hola servidor\n

El pipe lo pasa a nc

nc:

se conecta a localhost:8080

envía ese texto por el socket

Tu servidor:

hace accept()

hace recv()

recibe "hola servidor"

👉 nc está haciendo de cliente.