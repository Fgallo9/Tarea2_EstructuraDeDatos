#include "tdas/extra.h"
#include "tdas/list.h"
#include "tdas/map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Declaramos la funcion del TDA para insercion directa.
// Esto nos permite saltar la validacion de duplicados al cargar los IDs masivamente.
void multimap_insert(Map *map, void *key, void *value);

typedef struct {
  char id[100];
  char artist[1024];
  char album_name[1024];
  char track_name[1024];
  int tempo;
  char track_genre[1024];
} Song;

void mostrarMenuPrincipal() {
  limpiarPantalla();
  puts("========================================");
  puts("      Base de Datos de Canciones        ");
  puts("========================================");
  puts("1) Cargar Canciones");
  puts("2) Buscar por género de la canción");
  puts("3) Buscar por artista");
  puts("4) Buscar por tempo");
  puts("5) Crear Lista de Reproduccion");
  puts("6) Agregar Cancion a Lista");
  puts("7) Mostrar Canciones de una Lista");
  puts("8) Salir");
}

/**
 * Compara dos claves de tipo string para determinar si son iguales.
 * Esta función se utiliza para inicializar mapas con claves de tipo string.
 *
 * @param key1 Primer puntero a la clave string.
 * @param key2 Segundo puntero a la clave string.
 * @return Retorna 1 si las claves son iguales, 0 de lo contrario.
 */
int is_equal_str(void *key1, void *key2) {
  return strcmp((char *)key1, (char *)key2) == 0;
}

/**
 * Compara dos claves de tipo entero para determinar si son iguales.
 * Esta función se utiliza para inicializar mapas con claves de tipo entero.
 *
 * @param key1 Primer puntero a la clave entera.
 * @param key2 Segundo puntero a la clave entera.
 * @return Retorna 1 si las claves son iguales, 0 de lo contrario.
 */
int is_equal_int(void *key1, void *key2) {
  return *(int *)key1 == *(int *)key2; // Compara valores enteros directamente
}

void cargarCanciones(Map *songId) {
  printf("Iniciando carga masiva de archivo...\n");
  fflush(stdout); 

  FILE *archivo = fopen("data/song_dataset_.csv", "r");
  if (archivo == NULL) {
    printf("Error al abrir el archivo. Verifique la carpeta data.\n");  
    return;
  }

  char linea[4096];
  fgets(linea, 4096, archivo); // Saltamos los encabezados

  int cargadas = 0;
  while (fgets(linea, 4096, archivo) != NULL) {
      Song *nuevaCancion = (Song *)malloc(sizeof(Song));
      if (nuevaCancion == NULL) continue;

      char *token = strtok(linea, ",\n\r");
      if (token != NULL) {
          strncpy(nuevaCancion->id, token, 99);
          nuevaCancion->id[99] = '\0'; // Aseguramos el fin de cadena
      }

      token = strtok(NULL, ",\n\r");
      if (token != NULL) {
          strncpy(nuevaCancion->artist, token, 1023);
          nuevaCancion->artist[1023] = '\0';
      }

      token = strtok(NULL, ",\n\r");
      if (token != NULL) {
          strncpy(nuevaCancion->album_name, token, 1023);
          nuevaCancion->album_name[1023] = '\0';
      }

      token = strtok(NULL, ",\n\r");
      if (token != NULL) {
          strncpy(nuevaCancion->track_name, token, 1023);
          nuevaCancion->track_name[1023] = '\0';
      }

      token = strtok(NULL, ",\n\r");
      nuevaCancion->tempo = token ? atoi(token) : 0;

      token = strtok(NULL, ",\n\r");
      if (token != NULL) {
          strncpy(nuevaCancion->track_genre, token, 1023);
          nuevaCancion->track_genre[1023] = '\0';
      }

      // Como los IDs en el dataset son unicos por defecto, usamos multimap_insert
      // para una insercion O(1) directa, reduciendo la carga de minutos a segundos.
      multimap_insert(songId, nuevaCancion->id, nuevaCancion);
      cargadas++;

      // Indicador de progreso visual
      if (cargadas % 20000 == 0) {
        printf("Procesando... %d canciones cargadas.\n", cargadas);
        fflush(stdout);
      }
  }

  fclose(archivo);
  printf("¡Se cargaron %d canciones exitosamente!\n", cargadas);
}

// Busqueda secuencial en RAM: Iteramos el mapa maestro filtrando los resultados.
// Con los TDAs actuales, leer los 114k nodos en memoria toma fracciones de segundo.
void buscarPorGenero(Map *songId){
  char generoBuscado[50];
  printf("Por favor ingrese el genero a buscar: ");
  scanf(" %[^\n]", generoBuscado);

  int encontrados = 0;
  MapPair *pair = map_first(songId);
  while (pair != NULL) {
      Song *cancion = (Song *)pair->value;
      if (strcmp(cancion->track_genre, generoBuscado) == 0) {
          printf("ID: %s | Cancion: %s | Artista: %s | Tempo: %d\n", cancion->id, cancion->track_name, cancion->artist, cancion->tempo);
          encontrados++;
      }
      pair = map_next(songId);
  }

  if (encontrados == 0) printf("No se encontraron canciones.\n");
  else printf("-> Se encontraron %d canciones del genero %s.\n", encontrados, generoBuscado);
}

// Filtra las canciones del mapa principal verificando coincidencia exacta del artista
void buscarPorArtista(Map *songId){
  char artistaBuscado[100];
  printf("Ingrese el nombre del artista: ");
  scanf(" %[^\n]", artistaBuscado);

  int encontrados = 0;
  MapPair *pair = map_first(songId);
  while (pair != NULL) {
      Song *cancion = (Song *)pair->value;
      if (strcmp(cancion->artist, artistaBuscado) == 0) {
          printf("ID: %s | Cancion: %s | Artista: %s | Tempo: %d\n", cancion->id, cancion->track_name, cancion->artist, cancion->tempo);
          encontrados++;
      }
      pair = map_next(songId);
  }

  if (encontrados == 0) printf("No hay canciones registradas para '%s'.\n", artistaBuscado);
  else printf("-> Se encontraron %d canciones de %s.\n", encontrados, artistaBuscado);
}

// Clasifica e imprime canciones dependiendo del rango de BPM ingresado
void buscarPorTempo(Map *songId) {
  int opcion;
  printf("\nSeleccione la velocidad:\n");
  printf("1. Lentas (Menos de 80 BPM)\n");
  printf("2. Moderadas (Entre 80 y 120 BPM)\n");
  printf("3. Rapidas (Mayor a 120 BPM)\n");
  printf("Opcion: ");
  scanf("%d", &opcion);

  if (opcion < 1 || opcion > 3) {
      printf("Opcion no valida.\n");
      return;
  }

  int encontrados = 0;
  MapPair *pair = map_first(songId);
  while (pair != NULL) {
      Song *cancion = (Song *)pair->value;
      int match = 0;

      // Aplicamos la logica del rango dependiendo de la opcion elegida
      if (opcion == 1 && cancion->tempo < 80) match = 1;
      else if (opcion == 2 && cancion->tempo >= 80 && cancion->tempo <= 120) match = 1;
      else if (opcion == 3 && cancion->tempo > 120) match = 1;

      if (match) {
          printf("ID: %s | Cancion: %s | Artista: %s | Tempo: %d\n", cancion->id, cancion->track_name, cancion->artist, cancion->tempo);
          encontrados++;
      }
      pair = map_next(songId);
  }

  if (encontrados == 0) printf("No hay canciones en esta categoria.\n");
  else printf("-> Se listaron %d canciones.\n", encontrados);
}

// Crea una lista de reproduccion vacia y la guarda en el mapa de playlists
void crearListaReproduccion(Map *playlists) {
    char nombreLista[100];
    printf("Ingrese un nombre para la nueva lista de reproduccion: ");
    scanf(" %[^\n]", nombreLista);

    if (map_search(playlists, nombreLista) != NULL) {
        printf("Ya existe una lista con el nombre '%s'.\n", nombreLista);
    } else {
        List *nuevaLista = list_create();

        // Memoria dinamica para la clave del mapa para que no se pierda el string
        char *claveNombre = (char *)malloc(strlen(nombreLista) + 1);
        strcpy(claveNombre, nombreLista);

        map_insert(playlists, claveNombre, nuevaLista);
        printf("Lista '%s' creada exitosamente.\n", nombreLista);
    }
}

// Vincula un puntero del mapa principal hacia la lista de una playlist
void agregarCancionALista(Map *songId, Map *playlists) {
    char nombreLista[100];
    char idCancion[100];

    printf("Ingrese el nombre de la lista de reproduccion: ");
    scanf(" %[^\n]", nombreLista);

    MapPair *pairLista = map_search(playlists, nombreLista);
    if (pairLista == NULL) {
        printf("La lista '%s' no existe.\n", nombreLista);
        return;
    }

    printf("Ingrese el ID de la cancion a agregar: ");
    scanf(" %[^\n]", idCancion);

    // Como los IDs son unicos y exactos, map_search es ideal y rapido aqui
    MapPair *pairCancion = map_search(songId, idCancion);
    if (pairCancion == NULL) {
        printf("La cancion con ID '%s' no existe.\n", idCancion);
        return;
    }

    // Obtenemos los valores y los enlazamos
    List *listaDestino = (List *)pairLista->value;
    Song *cancionAAgregar = (Song *)pairCancion->value;

    list_pushBack(listaDestino, cancionAAgregar);
    printf("Cancion '%s' agregada exitosamente a '%s'.\n", cancionAAgregar->track_name, nombreLista);
}

// Recorre e imprime los datos almacenados dentro de una playlist
void mostrarCancionesDeLista(Map *playlists) {
    char nombreLista[100];
    printf("Nombre de la lista de reproduccion a mostrar: ");
    scanf(" %[^\n]", nombreLista);

    MapPair *pairLista = map_search(playlists, nombreLista);
    if (pairLista == NULL) {
        printf("La lista '%s' no existe.\n", nombreLista);
        return;
    }

    printf("\nPlaylist: %s \n", nombreLista);
    List *listaAMostrar = (List *)pairLista->value;

    Song *cancion = (Song *) list_first(listaAMostrar);
    if(cancion == NULL){
      printf("La lista esta vacia.\n");
      return;
    }

    while (cancion != NULL) {
        printf("ID: %s | Cancion: %s | Artista: %s | Tempo: %d\n", cancion->id, cancion->track_name, cancion->artist, cancion->tempo);
        cancion = (Song *) list_next(listaAMostrar);
    }
}

int main() {
  char opcion; 

  // Optimizacion arquitectonica: Solo instanciamos el mapa maestro y el de listas.
  // Las busquedas (artista, genero, tempo) se hacen recorriendo iterativamente el mapa.
  Map *songId = map_create(is_equal_str);
  Map *playlists = map_create(is_equal_str);

  do {
    mostrarMenuPrincipal();
    printf("Ingrese su opción: ");
    scanf(" %c", &opcion);

    switch (opcion) {
      case '1':
        cargarCanciones(songId);
        break;
      case '2':
        buscarPorGenero(songId);
        break;
      case '3':
        buscarPorArtista(songId);
        break;
      case '4':
        buscarPorTempo(songId);
        break;
      case '5':
        crearListaReproduccion(playlists);
        break;
      case '6':
        agregarCancionALista(songId, playlists);
        break;
      case '7':
        mostrarCancionesDeLista(playlists);
        break;
      case '8':
        printf("Saliendo de Spotifind...\n");
        break;
      default:
        printf("Opcion no valida. Por favor intente de nuevo.\n");
    }

    if (opcion != '8') presioneTeclaParaContinuar();

  } while (opcion != '8');

  // Limpieza final de memoria
  map_clean(songId);
  map_clean(playlists);

  return 0;
}