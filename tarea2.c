#include "tdas/extra.h"
#include "tdas/list.h"
#include "tdas/map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char id[100];
  char artist[100];
  char album_name[100];
  char track_name[100];
  int tempo;
  char track_genre[100];
} Song;

// Menú principal
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

const char *obtenerCampo(char *linea, int numeroDeCampo) {
    const char *campoActual;

    // strtok va cortando la línea cada vez que encuentra una coma
    for (campoActual = strtok(linea, ","); campoActual && *campoActual; campoActual = strtok(NULL, ",\n\r")) {
        // Disminuimos el contador hasta llegar al campo que queremos
        numeroDeCampo--;
        if (numeroDeCampo == 0) {
            return campoActual; // Retornamos el campo encontrado
        }
    }

    return NULL; // Si no encuentra el campo, retorna nulo
}

void cargarCanciones(Map *songId, Map *songGenres, Map *songArtist, Map *songTempo) {
  FILE *archivo = fopen("song_dataset_.csv", "r");
  if (archivo == NULL) {
    printf("Error al abrir el archivo. Asegúrate de que song_dataset_.csv esté en la carpeta.\n");  
    return;
  }

  char linea[1024];
  fgets(linea, 1024, archivo); // Saltamos la primera linea

  int cargadas = 0;
  while (fgets(linea, 1024, archivo) != NULL) {
      Song *nuevaCancion = (Song *)malloc(sizeof(Song));
      if (nuevaCancion == NULL) continue;

      char lineaCopia[1024];

      // Extraemos los datos
      strcpy(lineaCopia, linea);
      strcpy(nuevaCancion->id, obtenerCampo(lineaCopia, 1));

      strcpy(lineaCopia, linea);
      strcpy(nuevaCancion->artist, obtenerCampo(lineaCopia, 2));

      strcpy(lineaCopia, linea);
      strcpy(nuevaCancion->album_name, obtenerCampo(lineaCopia, 3));

      strcpy(lineaCopia, linea);
      strcpy(nuevaCancion->track_name, obtenerCampo(lineaCopia, 4));

      strcpy(lineaCopia, linea);
      const char* tempoStr = obtenerCampo(lineaCopia, 5);
      nuevaCancion->tempo = tempoStr ? atoi(tempoStr) : 0;

      strcpy(lineaCopia, linea);
      const char* genreStr = obtenerCampo(lineaCopia, 6);
      if (genreStr) strcpy(nuevaCancion->track_genre, genreStr);

      // Insertar en Mapa ID
      map_insert(songId, nuevaCancion->id, nuevaCancion);

      // Insertar en Mapa Géneros
      MapPair *pairGenero = map_search(songGenres, nuevaCancion->track_genre);
      if (pairGenero == NULL) {
          List *listaGenero = list_create();
          list_pushBack(listaGenero, nuevaCancion);
          map_insert(songGenres, nuevaCancion->track_genre, listaGenero);
      } else {
          list_pushBack((List *)pairGenero->value, nuevaCancion);
      }

      // Insertar en Mapa Artistas
      MapPair *pairArtista = map_search(songArtist, nuevaCancion->artist);
      if (pairArtista == NULL) {
          List *listaArtista = list_create();
          list_pushBack(listaArtista, nuevaCancion);
          map_insert(songArtist, nuevaCancion->artist, listaArtista);
      } else {
          list_pushBack((List *)pairArtista->value, nuevaCancion);
      }

      // Insertar en Mapa Tempo
      char *categoriaTempo;
      if (nuevaCancion->tempo < 80) categoriaTempo = "Lentas";
      else if (nuevaCancion->tempo <= 120) categoriaTempo = "Moderadas";
      else categoriaTempo = "Rapidas";

      MapPair *pairTempo = map_search(songTempo, categoriaTempo);
      if (pairTempo == NULL) {
          List *listaTempo = list_create();
          list_pushBack(listaTempo, nuevaCancion);
          map_insert(songTempo, categoriaTempo, listaTempo);
      } else {
          list_pushBack((List *)pairTempo->value, nuevaCancion);
      }

      cargadas++;
  }

  fclose(archivo);
  printf("¡Se cargaron %d canciones exitosamente!\n", cargadas);
}

void mostrarListaCanciones(List *lista) {
    Song *cancion = (Song *) list_first(lista);
    if(cancion == NULL){
      printf("No se encontraron resultados.\n");
      return;
    }
    while (cancion != NULL) {
        printf("ID: %s | Cancion: %s | Artista: %s | Tempo: %d\n", cancion->id, cancion->track_name, cancion->artist, cancion->tempo);
        cancion = (Song *) list_next(lista);
    }
}

void buscarPorGenero(Map *songGenres){
  char generoBuscado[50];
  printf("Por favor ingrese el genero a buscar");
  scanf(" %[^\n]s", generoBuscado);
  MapPair *resultado = map_search(songGenres, generoBuscado);
    if (resultado != NULL) {
        mostrarListaCanciones((List *)resultado->value); // Llamas a tu función auxiliar
    } else {
        printf("No se encontraron canciones.\n");
    }
}

void buscarPorArtista(Map *songArtist){
  char artistaBuscado[100];
    printf("Ingrese el nombre del artista: ");
    scanf(" %[^\n]s", artistaBuscado);

    MapPair *resultado = map_search(songArtist, artistaBuscado);

    if (resultado != NULL) {
        mostrarListaCanciones((List *) resultado->value);
    } else {
        printf("No hay canciones registradas para el artista '%s'.\n", artistaBuscado);
    }
}

void buscarPorTempo(Map *songTempo) {
  int opcion;
  printf("\nSeleccione la velocidad:\n");
  printf("1. Lentas (Menos de 80 BPM)\n");
  printf("2. Moderadas (Entre 80 y 120 BPM)\n");
  printf("3. Rapidas (Mayor a 120 BPM)\n");
  printf("Opcion: ");
  scanf("%d", &opcion);

  char *categoria;
    if (opcion == 1) categoria = "Lentas";
    else if (opcion == 2) categoria = "Moderadas";
    else if (opcion == 3) categoria = "Rapidas";
    else {
        printf("Opcion no valida.\n");
        return;
    }
    MapPair *resultado = map_search(songTempo, categoria);
    if(resultado != NULL){
      printf("\n--- Mostrando Canciones %s ---\n", categoria);
      mostrarListaCanciones((List*) resultado->value);
    }
    else{
      printf("No hay canciones en la categoria %s.\n", categoria);
    }
}

void crearListaReproduccion(Map *playlists) {
    char nombreLista[100];
    printf("Ingrese un nombre para la nueva lista de reproduccion: ");
    scanf(" %[^\n]s", nombreLista);

    if (map_search(playlists, nombreLista) != NULL) {
        printf("Ya existe una lista con el nombre '%s'.\n", nombreLista);
    } else {
        List *nuevaLista = list_create();

        char *claveNombre = (char *)malloc(strlen(nombreLista) + 1);
        strcpy(claveNombre, nombreLista);

        map_insert(playlists, claveNombre, nuevaLista);
        printf("Lista '%s' creada exitosamente.\n", nombreLista);
    }
}

void agregarCancionALista(Map *songId, Map *playlists) {
    char nombreLista[100];
    char idCancion[100];

    printf("Ingrese el nombre de la lista de reproduccion: ");
    scanf(" %[^\n]s", nombreLista);
  
    MapPair *pairLista = map_search(playlists, nombreLista);
    if (pairLista == NULL) {
        printf("La lista '%s' no existe.\n", nombreLista);
        return;
    }

    printf("Ingrese el ID de la cancion a agregar: ");
    scanf(" %[^\n]s", idCancion);

    MapPair *pairCancion = map_search(songId, idCancion);
    if (pairCancion == NULL) {
        printf("La cancion con ID '%s' no existe.\n", idCancion);
        return;
    }

    List *listaDestino = (List *)pairLista->value;
    Song *cancionAAgregar = (Song *)pairCancion->value;

    list_pushBack(listaDestino, cancionAAgregar);
    printf("Cancion '%s' agregada exitosamente a '%s'.\n", cancionAAgregar->track_name, nombreLista);
}

void mostrarCancionesDeLista(Map *playlists) {
    char nombreLista[100];
    printf("Nombre de la lista de reproduccion a mostrar: ");
    scanf(" %[^\n]s", nombreLista);

    MapPair *pairLista = map_search(playlists, nombreLista);
    if (pairLista == NULL) {
        printf("La lista '%s' no existe.\n", nombreLista);
        return;
    }

    printf("\nPlaylist: %s \n", nombreLista);
    List *listaAMostrar = (List *)pairLista->value;

    mostrarListaCanciones(listaAMostrar); 
}

int main() {
  char opcion; 
  Map *songId = map_create(is_equal_str);
  Map *songGenres = map_create(is_equal_str);
  Map *songArtist = map_create(is_equal_str);
  Map *songTempo = map_create(is_equal_str);
  Map *playlists = map_create(is_equal_str);

  do {
    mostrarMenuPrincipal();
    printf("Ingrese su opción: ");
    scanf(" %c", &opcion);

    switch (opcion) {
      case '1':
        cargarCanciones(songId, songGenres, songArtist, songTempo);
        break;
      case '2':
        buscarPorGenero(songGenres);
        break;
      case '3':
        buscarPorArtista(songArtist);
        break;
      case '4':
        buscarPorTempo(songTempo);
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

  // Limpiamos la memoria de todos los mapas
  map_clean(songId);
  map_clean(songGenres);
  map_clean(songArtist);
  map_clean(songTempo);
  map_clean(playlists);

  return 0;
}