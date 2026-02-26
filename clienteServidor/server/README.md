## como probar:

hacemos make tanto en cliente como en servidor
ejecutamos servidor con ./server
ejecutamos los clientes usando ./test_clients.sh


1) instalar conan
cd clienteServidor
python3 -m venv .venv

Esto crea:

clienteServidor/
├── .venv/
├── client/
└── server/

📌 .venv NO se versiona (va al .gitignore).

1.3 Activar el venv
source .venv/bin/activate

Vas a ver algo como:

(.venv) francomamani@pop-os:~/clienteServidor$

👉 Todo lo que instales ahora queda aislado.

1.4 Instalar Conan DENTRO del venv
pip install conan

Verificá:

conan --version



para compilar:
pararse en la raiz:

mkdir -p build
cmake -S . -B build
cmake --build build

ejecutables: 
build/client/client
build/server/server