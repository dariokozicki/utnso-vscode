# Sistemas Operativos: TP en VSCode
## o 'Cómo aprendí a dejar de preocuparme y AMAR Sisop!'

<a href="https://imgur.com/YSLFDs3"><img src="https://i.imgur.com/YSLFDs3.png" title="source: imgur.com" /></a>

## Esta guía explica cómo hacer un proyecto en VSCode paso a paso.
### *El resultado se puede descargar del mismo repositorio.*

## Requisitos:
- Tener instalado:
- - El editor [VSCode](https://code.visualstudio.com/). Viene en la VM pero se recomienda actualizar a [la ultima version son soporte a 32bit](https://code.visualstudio.com/updates/v1_35)
- - La extensión [C/C++](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools) de VSC
- - La [commons library de la cátedra](https://github.com/sisoputnfrba/so-commons-library)
- - (Altamente recomendado) [CUnit](https://mysnippets443.wordpress.com/2015/03/07/ubuntu-install-cunit/) para no desvelarse retesteando a mano el dia anterior a la entrega :smile:
- - GNOME Terminal (__sudo apt-get install gnome-terminal__)
- Hacerse un buen cafecito antes de empezar

## Importante:

#### Proceso 1, 2, y 3 representan N procesos, y todos se setean de la misma forma. Se recomienda seguir los pasos para un solo proceso, copiar y pegar en los N procesos necesarios, y luego hacer un find and replace de los nombres. Shared sin embargo es distinto y debe hacerse siguiendo los pasos.

#### Error conocido: Si copiaste y pegaste el makefile de acá, y te dice: 
    makefile:35: *** missing separator.  Stop.
Es porque Markdown alteró los tabs. Necesitas sacar el espacio de las acciones, poner tab en cada uno, y probar nuevamente. 

## Parte 1: Carpetas y makefiles

Primero creamos una carpeta para cada uno de los procesos que nos piden, más una llamada 'shared' donde va a estar el código compartido. (Vamos a hacer una trampita: no es realmente una biblioteca compartida.)
#### Todas tienen que estar en la misma carpeta padre.

>proceso1 proceso2 proceso3 shared

Vamos a abrir VSCode e ir a Archivo->Abrir Carpeta, y seleccionamos todas estas carpetas, con shift. Luego hacemos **archivo->guardar área de trabajo como...** y se nos va a crear un archivo .code-workspace en la ruta del TP.

La proxima vez que lo queramos abrir, vamos a hacer **Archivo->Abrir Área de Trabajo**, seleccionando ese .code-workspace, y nos va a abrir todas las carpetas de una con sus configuraciones.

En cada una de estas carpetas, creamos las subcarpetas:
>obj src include cfg .vscode


En __src__ van a estar nuestros __.c__, en __include__ nuestros __.h__, en __cfg__ nuestras __configuraciones y logs__, y en __obj__ van a estar los __.o__ (no vamos a interactuar con éstos). **Shared no necesita cfg**. 

*.vscode* es una carpeta de VSC que va a ser más útil en breve. 
 

A continuación, vamos a copiar el siguiente makefile __en cada uno de los procesos excepto shared__, reemplazando el valor de PROCESS_NAME con el de nuestro proceso

    PROCESS_NAME=proceso1
    IDIR =./include
    SHARED_IDIR = ../shared/include
    CC=gcc
    CFLAGS=-I$(IDIR) -I$(SHARED_IDIR) -g -Wall

    ODIR=./obj
    SRCDIR =./src
    LIBS=-lcommons -lpthread -lreadline -lcunit -lrt

    SHARED_SRCDIR = ../shared/src
    SHARED_ODIR = ../shared/obj

    DEPS = $(wildcard $(IDIR)/*.h)

    SRC = $(wildcard $(SRCDIR)/*.c)
    OBJ = $(patsubst $(SRCDIR)/%.c,$(ODIR)/%.o,$(SRC))

    $(ODIR)/%.o: $(SRCDIR)/%.c $(DEPS) $(SHARED_DEPS)
        $(CC) -c -o $@ $< $(CFLAGS)

    $(SHARED_ODIR)/%.o: $(SHARED_SRCDIR)/%.c $(SHARED_DEPS)
        $(CC) -c -o $@ $< $(CFLAGS)

    SHARED_DEPS = $(wildcard $(SHARED_IDIR)/*.h)

    SHARED_SRC = $(wildcard $(SHARED_SRCDIR)/*.c)
    SHARED_OBJ = $(patsubst $(SHARED_SRCDIR)/%.c,$(SHARED_ODIR)/%.o,$(SHARED_SRC))

    $(PROCESS_NAME): $(OBJ) $(SHARED_OBJ)
        $(CC) -o $@ $^ $(CFLAGS) $(LIBS)

    .PHONY: clean

    clean:
        rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ $(PROCESS_NAME) $(SHARED_ODIR)/*.o 

### Explicación:
Los makefiles constan de macros y reglas.
Las macros tienen el formato:

    MACRO=valor 
 
 y podemos poner paths, comandos, nombres, includes y mucho más, evitando repetirlo después y referenciándolo con el formato $(MACRO).
A su vez, los makefiles tienen *reglas*, que tienen el formato:
    
    target: dependencias
        acción

Donde acción debe tener un __TAB__ sí o sí. __Target__ es el nombre de la regla que podemos usar en la terminal (make clean, make install, etc), __Dependencias__ son las que necesito para ejecutar la acción, como los .o que necesito para armar el ejecutable. __Acción__ es el comando que quiero ejecutar, como gcc 
con los argumentos y las dependencias. Este makefile lo hice [siguiendo esta guía de la prehistoria](https://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/) y si querés modificarlo a tu gusto te invito a leer [la documentación](https://www.gnu.org/software/make/manual/make.html).    

__LIBS__ es un macro donde van las librerías que vamos a usar. __-lcommons__ es la de la cátedra, __-cunit__ es para los tests unitarios (optativo), y los demás los necesité en su momento. Agregá o sacá de acuerdo a tus necesidades.

__CFLAGS__ es un macro donde van los flags al compilar. __-g es necesario para ejecutar en VSCode en modo debug__. __-Wall__ es para mostrar todos los warnings. Te recomiendo no sacar ninguno.

Este makefile nos va a permitir que el procesoN compile utilizando los .c y .h del proceso más los que están en la shared. Acá está la trampita; no es una biblioteca compartida sino un lugar donde los procesos saben que tienen que ir a buscar el resto de los archivos.
#### Desventajas:
- Vamos a estar agregando archivos extra al compilado cuando quizá no las necesitemos, haciendo que 'make' haga de más.
- Actualmente no permite subcarpetas (Ej. ./src/misubcarp/foo.c no va a funcionar)
#### Ventajas:
- [Principalmente ésta.](https://www.youtube.com/watch?v=1JfeuNlQJQY)
- Ya no nos tenemos que preocupar por la configuración rompiendo el repo de nuestros compañeros, porque usamos paths relativos para todo.
- Vamos a estar usando el ejecutable para correr en VSCode, así que __si anda en VSC anda en la terminal__.

## Parte 2: Crear los .h y .c básicos

En /src/ vamos a crear *nombreproceso*.c y en /include/ *nombreproceso*.h.

##### nombreproceso.c:
    #include "proceso1.h"

    int main(){
        t_log* logger = log_create("./cfg/proceso1.log", "PROCESO1", true, LOG_LEVEL_INFO);
        log_info(logger, "Soy el proceso 1! %s", mi_funcion_compartida());
        log_destroy(logger);
    }
##### nombreproceso.h:
    #ifndef PROCESO1_H
    #define PROCESO1_H

    #include <stdio.h>
    #include <commons/log.h>
    #include <stdbool.h>
    #include "shared_utils.h"

    #endif

Podes ver la [docu si no entendés el código](https://github.com/sisoputnfrba/so-commons-library/blob/master/src/commons/log.h). Igualmente lo vas a ver en el TP0.

Vamos a agregar dos archivos más en la carpeta **shared**
##### shared/include/shared_utils.h
    #ifndef SHARED_UTILS_H
    #define SHARED_UTILS_H

    #include <stdio.h>
    #include <commons/log.h>
    #include <stdbool.h>

    char* mi_funcion_compartida();

    #endif

##### shared/src/shared_utils.c
    #include "shared_utils.h"

    char* mi_funcion_compartida(){
        return "Hice uso de la shared!";
    }

Si hacemos _make_ vamos a ver que se nos creó el ejecutable. __Hurra!__

    >make && ./proceso1
    make: 'proceso1' is up to date.
    [INFO] 17:14:17:572 PROCESO1/(16977:16977): Soy el proceso 1! Hice uso de la shared!

## Parte 3: Scripts de uso común

<a href="https://imgur.com/IPSXWAw"><img src="https://i.imgur.com/IPSXWAw.png" title="source: imgur.com" /></a>

Seguro te está desmotivando tener que hacer *make* y *./miproyecto* cada vez que querés probar tu programa. Y el botoncito de Eclipse para correr con valgrind (o de manera normal!) ni está. 

>*'Me están cagando'*. Dijo, mientras perdía la esperanza.

Vamos a crear dos shell scripts en cada proceso, cambiando el valor de FILE por el nombre de nuestro proceso: 

##### exec
    #!/bin/bash
    FILE=proceso1
    make $FILE
    if test -f "./$FILE"; then
        ./$FILE
    fi
##### vexec
    #!/bin/bash
    FILE=proceso1
    make $FILE
    if test -f "./$FILE"; then
        valgrind --tool=memcheck --leak-check=yes --show-possibly-lost=no --show-reachable=no --num-callers=20 ./$FILE
    fi

A partir de esto, en la terminal podemos escribir __./exec__ o __./vexec__ para Compilar y Ejecutar el proceso en un solo paso, ya sea normalmente o con valgrind. Optativamente podemos agregar un script para helgrind:

#### hexec
    #!/bin/bash
    FILE=proceso1
    make $FILE
    if test -f "./$FILE"; then
        valgrind --tool=helgrind ./$FILE
    fi

__Si queremos que esté más ordenado, podemos agregar --log-file=a.out antes de ./$FILE y así lo importante va a irse a un archivo aparte__ 

__Para ejecutar estos scripts, pero vamos a tener que darnos permiso de ejecución. Para esto vamos a escribir__ 

    chmod 777 exec

y así con vexec y hexec. Pero en VSC todavía nos faltan algunas cosas. **Nota: Ni se les ocurra hacer esto fuera de la VM o del TP**

## Parte 4.1: Configurando VSCode (Intellisense)

Habrás observado que, si bien compila y todo, VSCode te subraya en verde algunos imports. Esto es porque, si bien vos, yo y el makefile sabemos dónde están los headers (.h), VSCode no tiene ni idea de cómo organizamos el proyecto.

Dentro de /.vscode/, creamos el archivo **c_cpp_properties.json** con el siguiente contenido:

    {
        "configurations": [
            {
                "name": "Linux",
                "includePath": [
                    "${workspaceFolder}",
                    "/usr/include/c++/5",
                    "/usr/include/i386-linux-gnu/c++/5",
                    "/usr/include/c++/5/backward",
                    "/usr/lib/gcc/i686-linux-gnu/5/include",
                    "/usr/local/include",
                    "/usr/lib/gcc/i686-linux-gnu/5/include-fixed",
                    "/usr/include/i386-linux-gnu",
                    "/usr/include",
                    "${workspaceFolder}/include",
                    "../shared/include"
                ],
                "defines": ["_GNU_SOURCE"],
                "intelliSenseMode": "clang-x86",
                "browse": {
                    "path": [
                        "${workspaceFolder}",
                        "${workspaceFolder}/include",
                        "../shared/include",
                        "/usr/include/c++/5",
                        "/usr/include/i386-linux-gnu/c++/5",
                        "/usr/include/c++/5/backward",
                        "/usr/lib/gcc/i686-linux-gnu/5/include",
                        "/usr/local/include",
                        "/usr/lib/gcc/i686-linux-gnu/5/include-fixed",
                        "/usr/include/i386-linux-gnu",
                        "/usr/include"
                    ],
                    "limitSymbolsToIncludedHeaders": true,
                    "databaseFilename": ""
                },
                "compilerPath": "/usr/bin/gcc",
                "cStandard": "c11",
                "cppStandard": "c++17"
            }
        ],
        "version": 3
    }

Con esto le hacemos saber a VSC que tiene que buscar en el /include/ del proyecto y en el /include/ de la shared. Los warnings en verde deberían irse.

## Parte 4.2: Configurando VSCode (Debug, Tareas, Shortcuts)

>*Todo bien, pero me dijiste que podía usar VSCode y tengo que usar exec, vexec y no sé qué. Al final sigo usando la terminal.*

Vamos a la parte de __Debug__ de VSC (bichito a la izquierda en el IDE), y arriba donde aparece "DEPURAR" vamos a abrir el combo box y seleccionar *Agregar Configuración (procesoN)*. Seleccionamos C++ (GDB/LLDB), y "Launch".

Se nos va a abrir launch.json en /.vscode/ . Vamos a cambiar *(gdb) Launch* en __name__ por *Proceso1*, y lo de __program__ por *${workspaceFolder}/proceso1*   

Ahora ya podemos Debuggear nuestro proyecto! Si vas al bichito nuevamente y seleccionas *Proceso1*, apretando en el triángulo verde o f5 vas a poder debuggear. Podés poner breakpoints para ver lo que se imprime por consola y demás. Tené en cuenta que debuggear **no compila tu aplicación, así que lo vas a tener que hacer manualmente**. Despues del proximo paso vamos a ver como arreglar eso

Ahora te toca repetir esto mismo con los cuatro procesos. De esta forma vas a poder debuggear cualquiera de los cuatro.

Pero eso no es todo...


Vamos a ir a Tareas-> Configurar Tareas->Crear archivo tasks.json desde plantilla (*proceso1*)->Others

Se nos abre un tasks.json donde vamos a poder decirle a VSCode qué queremos hacer. Vamos a agregar tres tareas: una para compilar, una para ejecutar normal y otra para valgrind (4 si querés hacer para helgrind). Reemplazamos los labels por el nombre de cada tarea, shell se queda igual, y en command vamos a poner ./exec o ./vexec respectivamente. Nos queda algo así:

    {
        "version": "2.0.0",
        "tasks": [
            {
                "label": "Make",
                "type": "shell",
                "command": "make",
                "problemMatcher": [
                    "$gcc"
                ]
            },
            {
                "label": "Proceso1 Normal",
                "type": "shell",
                "command": "./exec"
            },
            {
                "label": "Proceso1 Valgrind",
                "type": "shell",
                "command": "./vexec"
            }
        ]
    }

Ahora, podemos ir a **Tareas->Ejecutar Tarea->Proceso1 Normal->(gcc)** para ejecutarlo directo desde el IDE, o Proceso1 Valgrind para ejecutarlo con Valgrind! Ah y **se compila automáticamente cada vez que lo corrés** :smile:

>*Qué fiaca. son como 3 clicks.*

Vamos a ir a __Archivo->Preferencias->Métodos abreviados de teclado__ y apenas abajo del buscador hacemos click en *keybindings.json*.

Ahora vamos a linkear nuestras tareas creadas a keybinds. Copiamos lo siguiente:

    [
        {
            "key": "ctrl+h",
            "command": "workbench.action.tasks.runTask",
            "args": "Proceso1 Normal"
        },
        {
            "key": "ctrl+j",
            "command": "workbench.action.tasks.runTask",
            "args": "Proceso1 Valgrind"
        },      
    ]

En vez de ctrl+h y ctrl+j ponés tu combinación favorita. Command se queda como está, y **en args ponemos el nombre *exacto* de la tarea, tal como aparece en label dentro de tasks.json**.

Pumba! Ahora con una combinación de teclas podemos ejecutar el proceso las veces que queramos, desde el IDE. No más terminal, no más Eclipse, todos felices. 

Podés hacer esto mismo con el resto de los procesos y decirle a tus compañeros que se hagan sus propios keybinds (ya que es probable que una persona se dedique a un sólo proceso en el TP)

Ahora tambien podemos hacer que se ejecute la tarea de compilacion que creamos cuando empezamos a debuggear. Abrimos para todos los procesos el archivo launch.json y agregamos esta linea:

            "preLaunchTask": "Make",

## Parte 5: Testeo Unitario y el retorno de PdeP

>*Seré honesto contigo Lisa, jamás hice las pruebas.*

[![Lisa interactuando con el equipo de QA](https://i.ytimg.com/vi/c3MaSmykVC8/hqdefault.jpg)](https://www.youtube.com/watch?v=c3MaSmykVC8 "Seré honesto contigo Lisa, jamás hice las pruebas")

Nadie te lo va a pedir. Nadie se lo va a esperar cuando caigas a la entrega con tests unitarios. 

No lo hagas por la nota. Hacelo porque realmente te vas a ahorrar tiempo, frustración y noches de desvelo tratando de encontrar errores.

Si ya estás laburando te habrás dado cuenta de lo difícil que es tocar código de un programa que no tiene documentación ni tests, y de [la bronca que da no entender por qué las cosas no funcionan](https://www.youtube.com/watch?v=N9wsjroVlu8).

Habiendo dicho eso:

Nuestro makefile ya tiene para usar CUnit. Vamos a hacer las siguientes modificaciones:

##### proceso1.c
    #include "proceso1.h"
    int main(int argc, char ** argv){
        if(argc > 1 && strcmp(argv[1],"-test")==0)
            run_tests();
        else{  
            t_log* logger = log_create("./cfg/proceso1.log", "PROCESO1", true, LOG_LEVEL_INFO);
            log_info(logger, "Soy el proceso 1! %s", mi_funcion_compartida());
            log_destroy(logger);
        } 
    }
##### proceso1.h
    #ifndef PROCESO1_H
    #define PROCESO1_H
    #include <stdio.h>
    #include <commons/log.h>
    #include <stdbool.h>
    #include "shared_utils.h"
    #include "tests.h"
    #endif
Creamos un nuevo archivo "tests.c" y "tests.h" en src e include, que nos quedarían así:

##### tests.h
    #ifndef TESTS_H
    #define TESTS_H
    #include <CUnit/Basic.h>
    int run_tests();
    void suma_proceso1();
    #endif
##### tests.c
    #include "tests.h"
    int run_tests(){
        CU_initialize_registry();
        CU_pSuite tests = CU_add_suite("PROCESO1 Suite",NULL,NULL);
        CU_add_test(tests,"Probar Suma", suma_proceso1);

        CU_basic_set_mode(CU_BRM_VERBOSE);
        CU_basic_run_tests();
        CU_cleanup_registry();
        return CU_get_error();
    }
    void suma_proceso1(){
        CU_ASSERT_EQUAL(2+2, 4);
    }


De esta forma podemos correr **./proceso1 -test** para correr nuestros tests. [**Te recomiendo empezar testeando que se serialicen / deserialicen bien tus mensajes**](https://docs.google.com/document/d/190LLBTzHkFmD_yS8OWcUsJoIOQVKxJkUxIFtcnChr-g/edit) :smile:

De la misma forma, podemos ir a nuestro __launch.json__ y en __args__ agregar "-test" para correr nuestros tests en debug. También podemos crear tareas como en las ejecuciones normales y agregarle shortcuts. Lo que te haga feliz.

    >./proceso1 -test


    CUnit - A unit testing framework for C - Version 2.1-3
    http://cunit.sourceforge.net/


    Suite: PROCESO1 Suite
    Test: Probar Suma ...passed

    Run Summary:    Type  Total  Ran Passed Failed Inactive
                suites     1      1    n/a      0        0
                tests      1      1      1      0        0
                asserts    1      1      1      0      n/a

## Parte 6: Deployment para entrega final

>### Tu cara cuando ande en la VM pero no en la entrega
>![](https://pics.me.me/thumb_los-ping%C3%BCinos-de-madagascar-meme-58919278.png)

Seguro te dijeron que tenés 15 minutos para setear el proyecto en la entrega. Lo que probablemente no sepas (o te acuerdes) en la entrega es que tenés que **instalar la commons y todo lo que uses en la máquina donde entregás!** Y probablemente sean varias VMs conectadas por LAN, así que se te pueden ir los tiempos por este detalle. Te va a ser más fácil automatizar esto. Adapté un script de [acá](https://github.com/sisoputnfrba/so-deploy) para que sea más fácil de usar. Vamos a ir a la carpeta del TP y creamos:

##### ./deploy.sh

    #!/bin/bash
    length=$(($#-1))
    OPTIONS=${@:1:$length}
    REPONAME="${!#}"
    CWD=$PWD
    echo -e "\n\nInstalling commons libraries...\n\n"
    COMMONS="so-commons-library"
    git clone "https://github.com/sisoputnfrba/${COMMONS}.git" $COMMONS
    cd $COMMONS
    sudo make uninstall
    make all
    sudo make install
    cd $CWD
    echo -e "\n\nBuilding projects...\n\n"
    make -C ./proceso1
    make -C ./proceso2
    make -C ./proceso3
    echo -e "\n\nDeploy done!\n\n"

Reemplazamos en los 'make' por nuestras subcarpetas **exceptuando shared que no tiene makefile**.

Listo el pollo. Ejecutas esto en la VM nueva y ya estás listo para arrancar.