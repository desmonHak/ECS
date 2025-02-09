# ECS

----

# Actualizacion/instalacion de submodulos

antes de hacer nada es necesario instalar/descargar los submodulos con el comando:
```
git submodule update --init --recursive
```

estando situado en la raiz del proyecto.

----

## Compilacion a libreria estatica `.a`

La compilaciom a libreria estatica se hace mediante la utilidad `make`/`mingw32-make`. La compilacion se efectua de la siguiente manera:

En Linux:
```bash
make -f linux.mk generate_lib
```

En Windows:
```bash
mingw32-make -f windows.mk generate_lib
```

### Alternativas a `generate_lib`
Los archivos `Makefile` estan configurados para generar tambien codigo de ejemplo y versiones 'debug' de las librerias y codigo:

- `generate_lib_debug`: permite compilar la libreria y sus sub-libreria en modo debug, viendo el trazado de llamadas con sus argumentos recibidos.
- `examples`: permite compilar los ejemplos de la carpeta `./examples`.
- `examples_debug`: permite compilar los ejemplos de la carpeta `./examples` en version debug.
- `all`: permite compilar todo en modo 'no-debug'.
- `cleanobj`: elimina todo los archivos objetos generados.
- `cleanall`: invoca a `cleanobj`, elimina los archivos `.a` y la version release.

----

documentacion [aqui en HTML.](./doc/html/index.html)

----