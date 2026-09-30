# Spotifind - Base de Datos de Canciones

## Descripción

Esta aplicación en C permite gestionar una amplia base de datos de canciones cargadas desde un archivo CSV. Utilizando Tipos de Datos Abstractos (TDAs) como Mapas y Listas, Spotifind organiza la información en memoria para permitir búsquedas instantáneas y eficientes basadas en diferentes criterios musicales como el género, el artista y el tempo de la canción.

## Cómo compilar y ejecutar el programa

1. **Descarga y descomprime** el archivo `.zip` del proyecto en una carpeta de tu preferencia.

### Opción 1: En Visual Studio Code
1. Inicia **Visual Studio Code**.
2. Selecciona `Archivo > Abrir carpeta...` y elige la carpeta donde descomprimiste el proyecto.

### Opción 2: En Replit
1. Crea un nuevo repl o importa la carpeta del proyecto descomprimido.
2. Asegúrate de que el archivo song_dataset_.csv esté subido en la raíz del proyecto.
3. Abre la terminal de Replit (`Shell`).

### Forma de compilación del codigo

1. Compila el codigo escribiendo:
   ```bash
   gcc tdas/*.c tarea2.c -Wno-unused-result -o tarea2
   ```
2. Una vez compilado el programa escribe:
   ```bash
   ./tarea2
   ```

## Funcionalidades

El sistema cuenta con las siguientes opciones operativas:
1. Cargar Canciones: Carga en memoria todas las canciones desde el archivo song_dataset_.csv, indexándolas en mapas (por ID, género, artista y tempo) para búsquedas O(1).
2. Buscar por género de la canción: Recibe el nombre de un género musical y muestra todas las canciones asociadas a este.
3. Buscar por artista: Recibe el nombre de un artista o banda y despliega toda su discografía disponible en el sistema.
4. Buscar por tempo: Permite al usuario clasificar y listar las canciones según su velocidad:
    - Lentas (Menos de 80 BPM).
    - Moderadas (Entre 80 y 120 BPM).
    - Rápidas (Mayor a 120 BPM).
5. Salir: Finaliza la ejecución del sistema de manera segura, liberando toda la memoria utilizada por los TDAs.

## Ejemplo de uso

**Inicio:** Muestra el menú principal, donde se puede acceder a todas las funciones del codigo.

```
========================================
      Base de Datos de Canciones
========================================
1) Cargar Canciones
2) Buscar por género de la canción
3) Buscar por artista
4) Buscar por tempo
5) Salir
Ingrese su opción:
```

**Paso 1:** El sistema lee el archivo CSV y distribuye la información en los mapas. Es requisito hacer esto antes de buscar.

```
Ingrese su opción: 1
¡Se cargaron 1500 canciones exitosamente!
Presione una tecla para continuar...
```

**Paso 2:** El usuario ingresa un género y el sistema busca instantáneamente en el mapa correspondiente todas las coincidencias.

```
Ingrese su opción: 2
Por favor ingrese el genero a buscar: pop
ID: 12345 | Cancion: Shape of You | Artista: Ed Sheeran | Tempo: 95
ID: 67890 | Cancion: Blinding Lights | Artista: The Weeknd | Tempo: 171
...
Presione una tecla para continuar...
```

**Paso 3:** Busca un artista específico respetando los espacios en el nombre y muestra sus canciones.

```
Ingrese su opción: 3

Ingrese su opción: 3
Ingrese el nombre del artista: Queen
ID: 11111 | Cancion: Bohemian Rhapsody | Artista: Queen | Tempo: 71
ID: 22222 | Cancion: Don't Stop Me Now | Artista: Queen | Tempo: 156
...
Presione una tecla para continuar...
```

**Paso 4:** El usuario selecciona una categoría de velocidad y el programa filtra la música usando los rangos definidos.

```
Ingrese su opción: 4

Seleccione la velocidad:
1. Lentas (Menos de 80 BPM)
2. Moderadas (Entre 80 y 120 BPM)
3. Rapidas (Mayor a 120 BPM)
Opcion: 1

--- Mostrando Canciones Lentas ---
ID: 11111 | Cancion: Bohemian Rhapsody | Artista: Queen | Tempo: 71
...
Presione una tecla para continuar...
```

**Paso 5:** Finaliza la ejecución del sistema, limpiando los TDAs y liberando la memoria.

```
Ingrese su opción: 5
Saliendo de Spotifind...
```

## Contribuciones

- **Ignacio Aracena:**

- **Franco Gallo:**