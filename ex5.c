/******************
Name: Dan Sonnenblick
ID: 345287882
Assignment: ex5
*******************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXIT 4
#define TRUE 1

typedef struct Song {
  char* title;
  char* artist;
  int year;
  char* lyrics;
  int streams;
} Song;


typedef struct Playlist {
  char* name;
  Song** songs;
  int songsNum;
} Playlist;


void watchPlaylists(Playlist ***playlists, int *currentAmount);
void addPlaylist(Playlist ***playlists, int *currentPlaylistAmount);
void removePlaylist(Playlist ***playlists, int *currentAmount);
void displayPlaylistMenu(Playlist*** playlists, Playlist* playlist, int *currentAmount);
void showPlaylist(Playlist* playlists, Playlist* playlist, int currentAmount);
void printPlaylistsMenu();
void sortPlaylist(Playlist *playlist);
void addSong(Playlist* playlists, Playlist* playlist, int *currentAmount);
void deleteSong(Playlist* playlists, Playlist* playlist, int *currentAmount);

// helper functions
char* readInput();
void clearBuffer();
char* duplicateString(const char* str);

int main() {
  int playlistAmount = 0;

  Playlist **playlists = malloc(sizeof(Playlist*)*1);
  if (playlists == NULL) {
      printf("Error allocating memory for playlists\n");
      exit(1);
  }
  int input;
  printPlaylistsMenu();
  scanf("%d", &input);

  while (input) {
      clearBuffer();
      switch (input) {
              // case 1 watch playlists: see all playlists
              case 1: {
                  watchPlaylists(&playlists, &playlistAmount);
                  break;
              }
              // case 2 add playlist
              case 2: {
                  addPlaylist(&playlists, &playlistAmount);
                  break;
              }
              // case 3 remove playlist
              case 3: {
                  removePlaylist(&playlists, &playlistAmount);
                  break;
              }
              //case 4 EXIT
              //here I do free on all the indiviual songs in each playlist
              // and then free the playlists
              case 4: {
                  for (int i = 0; i < playlistAmount; i++) {
                      if (playlists[i]->songs != NULL) {
                          for (int j = 0; j < playlists[i]->songsNum; j++) {
                              free(playlists[i]->songs[j]->title);
                              free(playlists[i]->songs[j]->artist);
                              free(playlists[i]->songs[j]->lyrics);
                              free(playlists[i]->songs[j]);
                          }
                          free(playlists[i]->songs);
                      }
                      free(playlists[i]->name);
                      free(playlists[i]);
                      }
                  free(playlists);
                  printf("Goodbye!\n");
                  return 0;
              }

              // Handle invalid menu inputs
              default:
                  printf("Invalid input\n");
          }
          // Display the menu again after an operation
          printPlaylistsMenu();
          // Get the user's next choice
          scanf("%d", &input);
      }
  return 0;
}

// Function to display available playlists and allow user to select one
void watchPlaylists(Playlist ***playlists, int *currentAmount) {
    // If there are no playlists available
    if (*currentAmount == 0) {
      printf("Choose a playlist:\n");
      printf("\t1. Back to main menu\n");
      int choice;
      scanf("%d", &choice); {
          // Ensure user input is valid (only option is to go back)
          while (choice != 1) {
              printf("Invalid input. Please try again.\n");
              printf("Choose a playlist:\n");
              printf("\t1. Back to main menu\n");
              scanf("%d", &choice);
          }
          return; // Exit function if user chooses to go back
      }
  }
  printf("Choose a playlist: \n");
    // Display list of playlists if there are any available

  for (int i = 0; i < *currentAmount; i++) {
      printf("\t%d. %s\n", i+1, (*playlists)[i]->name);
  }
  printf("\t%d. Back to main menu\n", *currentAmount+1);

  int selection;
  scanf("%d", &selection);
  clearBuffer(); // Clear input buffer to handle leftover input characters

  // Validate user input: Ensure selection is within valid range
  while (selection < 1 || selection > *currentAmount + 1) {
      printf("Invalid option\n");

      // Re-display playlist options
      printf("Choose a playlist: \n");
      for (int i = 0; i < *currentAmount; i++) {
          printf("\t%d. %s\n", i + 1, (*playlists)[i]->name);
      }
      printf("\t%d. Back to main menu\n", *currentAmount + 1);
      scanf("%d", &selection);
      clearBuffer();
  }
    // If user selects "Back to main menu"
  if (selection == *currentAmount+1) {
      return;
  }

    // If user selects a valid playlist
  if (selection >= 1 && selection <= *currentAmount) {
      // Call a function to display the playlist menu and handle further operations
      printf("playlist %s:\n", (*playlists)[selection - 1]->name);
      displayPlaylistMenu(playlists, (*playlists)[selection - 1], currentAmount);
  }
}

// Function to add a new playlist
void addPlaylist(Playlist ***playlists, int *currentPlaylistAmount) {
    // Reallocate memory for the playlists array to accommodate the new playlist
    Playlist **temp = realloc(*playlists, (*currentPlaylistAmount+1) * sizeof(Playlist*));
  if (temp == NULL) {
      printf("Memory reallocation failed\n");
      exit(1);
  }
  *playlists = temp; // Update playlists pointer to the new memory location

    // Allocate memory for the new playlist
  Playlist *newPlaylist = malloc(sizeof(Playlist));
  if (newPlaylist == NULL) {
      printf("Memory reallocation failed\n");
      free(*playlists);
      exit(1);
  }
  printf("Enter playlist's name: \n");
  char* name = readInput();

    // Duplicate the entered name and assign it to the new playlist
  newPlaylist->name = duplicateString(name);
  if (newPlaylist->name == NULL) {
      free(name);
      free(newPlaylist);
      free(*playlists);
      exit(1);
  }
  free(name);
    // Initialize the playlist's songs and song count

  newPlaylist->songs = NULL;
  newPlaylist->songsNum = 0;
    // Add the new playlist to the playlists array

  (*playlists)[*currentPlaylistAmount] = newPlaylist;
  (*currentPlaylistAmount)++; // Increment the playlist count
}

// Function to remove an existing playlist
void removePlaylist(Playlist *** playlists, int *currentPlaylistAmount) {
    // If no playlists exist, provide the option to return to the main menu
    if (*currentPlaylistAmount == 0) {
        printf("Choose a playlist:\n");
        printf("\t1. Back to main menu\n");
        int choice;
        scanf("%d", &choice);
        {
            // Validate user input for returning to the main menu
            while (choice != 1) {
                printf("Invalid input. Please try again.\n");
                printf("Choose a playlist:\n");
                printf("\t1. Back to main menu\n");
                scanf("%d", &choice);
            }
            return;
        }
    }
    // Display available playlists for selection

    printf("Choose a playlist: \n");

    for (int i = 0; i < *currentPlaylistAmount; i++) {
        printf("\t%d. %s\n", i+1, (*playlists)[i]->name);
    }
    printf("\t%d. Back to main menu\n", *currentPlaylistAmount+1);

    int selection;
    scanf("%d", &selection);
    clearBuffer();

    while (selection < 1 || selection > *currentPlaylistAmount + 1) {
        printf("Invalid option\n");
        printf("Choose a playlist: \n");
        for (int i = 0; i < *currentPlaylistAmount; i++) {
            printf("\t%d. %s\n", i + 1, (*playlists)[i]->name);
        }
        printf("\t%d. Back to main menu\n", *currentPlaylistAmount + 1);
        scanf("%d", &selection);
        clearBuffer();
    }
    if (selection == *currentPlaylistAmount+1) {
        return;
    }
    // If a valid playlist is selected, delete it
    if (selection >= 1 && selection <= *currentPlaylistAmount) {
        Playlist *toDelete = (*playlists)[selection-1];
        // Free all songs in the selected playlist
        for (int i = 0; i < toDelete->songsNum; i++) {
            free(toDelete->songs[i]->title);
            free(toDelete->songs[i]->artist);
            free(toDelete->songs[i]->lyrics);
            free(toDelete->songs[i]);
        }
        free(toDelete->name);
        free(toDelete->songs);
        free(toDelete);

        // Shift the remaining playlists in the array
        for (int i = selection-1; i < (*currentPlaylistAmount-1); i++) {
            (*playlists)[i] = (*playlists)[i+1];
        }

        (*currentPlaylistAmount)--;
        // Reallocate memory for the reduced playlists array
        if (*currentPlaylistAmount > 0) {
            Playlist **temp = realloc(*playlists, (*currentPlaylistAmount) * sizeof(Playlist *));
            if (temp == NULL && *currentPlaylistAmount > 0) {
                printf("Memory reallocation failed.\n");
                exit(1);
            }
            *playlists = temp;
        }
        printf("Playlist deleted.\n");
    }
    else {
        printf("Invalid option\n");
    }
}

// Function to display the menu for a specific playlist and handle user actions
void displayPlaylistMenu(Playlist*** playlists, Playlist* playlist, int *currentAmount) {
  int input;
    // Display menu options to the user
  printf("\t1. Show Playlist\n"
               "\t2. Add Song\n"
               "\t3. Delete Song\n"
               "\t4. Sort\n"
               "\t5. Play\n"
               "\t6. exit\n");
  scanf("%d", &input);
  clearBuffer();
  switch (input) {
      //case 1 Show the playlist
      case 1: {
          showPlaylist(**playlists, playlist, *currentAmount);
          displayPlaylistMenu(playlists, playlist, currentAmount);
          return;
      }
      // case 2 add a song to the okaylist
      case 2: {
          addSong(**playlists, playlist, currentAmount);
          displayPlaylistMenu(playlists, playlist, currentAmount);
          return;
      }
      // case 3  Delete a song from the playlist
      case 3: {
          deleteSong(**playlists, playlist, currentAmount);
          displayPlaylistMenu(playlists, playlist, currentAmount);
          return;
      }
      // case 4 Sort the songs in the playlist
      case 4: {
          sortPlaylist(playlist);
          displayPlaylistMenu(playlists, playlist, currentAmount);
          return;
      }
      case 5: {
          // Play the songs in the playlist
          for (int i = 0; i < playlist->songsNum; i++) {
              playlist->songs[i]->streams++;
              printf("Now playing %s:\n", playlist->songs[i]->title);
              printf("$ %s $\n ", playlist->songs[i]->lyrics);
          }
          displayPlaylistMenu(playlists, playlist, currentAmount);
          return;
      }
      case 6: {
          // Exit to the main menu
          watchPlaylists(playlists, currentAmount);
          return;
      }
      default: {
          printf("Invalid option\n");
      }
  }
}
// Function to display all songs in a playlist and allow the user to play them
void showPlaylist(Playlist* playlists, Playlist* playlist, int currentAmount) {
    (void)playlists;
    (void)currentAmount;

    // Display each song in the playlist

  for (int i = 0; i < playlist->songsNum; i++) {
      printf("%d. Title: %s \n", i+1, playlist->songs[i]->title);
      printf("   Artist: %s\n", playlist->songs[i]->artist);
      printf("   Released: %d\n", playlist->songs[i]->year);
      printf("   Streams: %d\n", playlist->songs[i]->streams);
  }
  int input;
  printf("choose a song to play, or 0 to quit:\n");
  scanf("%d", &input);
    // Allow the user to play songs until they choose to quit (input = 0)
  if (playlist->songsNum != 0) {
      while (input != 0){
          printf("Now playing %s:\n", playlist->songs[input-1]->title);
          printf("$ %s $\n", playlist->songs[input-1]->lyrics);
          playlist->songs[input-1]->streams++;
          printf("choose a song to play, or 0 to quit:\n");
          scanf("%d", &input);
      }
  }
}

// Function to add a new song to a playlist
void addSong(Playlist* playlists, Playlist* playlist, int *currentAmount) {
    (void)playlists;  // Suppress unused parameter warning
    (void)currentAmount;
    int yearOfRelease;

    // Allocate memory for the new song
  Song* newSong = (Song*)malloc(sizeof(Song));
  if (newSong == NULL) {
      printf("Memory allocation failed\n");
      exit(1);
  }

  printf("Enter song's details\n");

  printf("Title: \n");
  newSong->title = readInput();

  printf("Artist: \n");
  newSong->artist = readInput();

  printf("Year of release: \n");
  scanf("%d", &yearOfRelease);
  newSong->year = yearOfRelease;
  clearBuffer();

  printf("Lyrics: \n");
  newSong->lyrics = readInput();

  newSong->streams = 0;

    // Reallocate memory for the songs array in the playlist to add the new song
  playlist->songs = (Song**)realloc(playlist->songs, (playlist->songsNum + 1) * sizeof(Song*));
  if (playlist->songs == NULL) {
      printf("Memory allocation failed\n");
      free(newSong->title);
      free(newSong->artist);
      free(newSong->lyrics);
      free(newSong);
      exit(1);
  }
    // Add the new song to the playlist and update the song count
  playlist->songs[playlist->songsNum] = newSong;
  playlist->songsNum++;
}

void deleteSong(Playlist* playlists, Playlist* playlist, int *currentAmount) {
    (void)playlists;  // Suppress unused parameter warning
    (void)currentAmount;
    // Display all songs in the playlist
    for (int i = 0; i < playlist->songsNum; i++) {
      printf("%d. Title: %s \n", i+1, playlist->songs[i]->title);
      printf("   Artist: %s\n", playlist->songs[i]->artist);
      printf("   Released: %d\n", playlist->songs[i]->year);
      printf("   Streams: %d\n", playlist->songs[i]->streams);
  }
  int input;
  printf("choose a song to delete, or 0 to quit:\n");
  scanf("%d", &input);

  free(playlist->songs[input-1]->title);
  free(playlist->songs[input-1]->artist);
  free(playlist->songs[input-1]->lyrics);
  free(playlist->songs[input-1]);

  for (int i = input-1; i < playlist->songsNum-1; i++) {
      playlist->songs[i] = playlist->songs[i+1];
  }
  playlist->songsNum--;
    // Reallocate memory for the reduced songs array
  playlist->songs = realloc (playlist->songs, playlist->songsNum * sizeof(Song*));
  if (playlist->songs == NULL) {
      printf("Memory allocation failed\n");
      exit(1);
  }
  printf("Song deleted successfully.\n");
}

// Function to sort the songs in a playlist based on user-selected criteria
void sortPlaylist(Playlist *playlist) {
  int selection;
  printf("choose:\n");
  printf("1. sort by year\n");
  printf("2. sort by streams - ascending order\n");
  printf("3. sort by streams - descending order\n");
  printf("4. sort alphabetically\n");
  scanf("%d", &selection);
  if (selection == 4 || selection < 1 || selection > 4) {
    // Handle invalid input or alphabetical sort
      for (int i = 0; i < playlist->songsNum - 1; i++) {
          for (int j = 0; j < playlist->songsNum - i - 1; j++) {
              if (strcmp(playlist->songs[j]->title, playlist->songs[j + 1]->title) > 0) {
                  Song *temp = playlist->songs[j];
                  playlist->songs[j] = playlist->songs[j + 1];
                  playlist->songs[j + 1] = temp;
              }
          }
      }
      printf("sorted\n");
      return;
  }
  switch (selection) {
      case 1: {
            // Sort songs by release year in ascending order using bubble sort
          for (int i = 0; i < playlist->songsNum - 1; i++) {
              for (int j = 0; j < playlist->songsNum - i - 1; j++) {
                  if (playlist->songs[j]->year > playlist->songs[j + 1]->year) {
                      Song *temp = playlist->songs[j];
                      playlist->songs[j] = playlist->songs[j + 1];
                      playlist->songs[j + 1] = temp;
                  }
              }
          }
          printf("sorted\n");
          break;
      }
      case 2: {
          //sort by ascending streams
          for (int i = 0; i < playlist->songsNum - 1; i++) {
              for (int j = 0; j < playlist->songsNum - i - 1; j++) {
                  if (playlist->songs[j]->streams > playlist->songs[j + 1]->streams) {
                      Song *temp = playlist->songs[j];
                      playlist->songs[j] = playlist->songs[j + 1];
                      playlist->songs[j + 1] = temp;
                  }
              }
          }
          printf("sorted\n");
          break;
      }
      case 3: {
          // sort by descending streams
          for (int i = 0; i < playlist->songsNum - 1; i++) {
              for (int j = 0; j < playlist->songsNum - i - 1; j++) {
                  if (playlist->songs[j]->streams < playlist->songs[j + 1]->streams) {
                      Song *temp = playlist->songs[j];
                      playlist->songs[j] = playlist->songs[j + 1];
                      playlist->songs[j + 1] = temp;
                  }
              }
          }
          printf("sorted\n");
          break;
      }
  }
}

// Function to print the main menu for playlists
void printPlaylistsMenu() {
  printf("Please Choose:\n");
  printf("\t1. Watch playlists\n"
                "\t2. Add playlist\n"
                "\t3. Remove playlist\n"
                "\t4. exit\n");
}

// Function to read a dynamically allocated string input from the user
char *readInput() {
    char *input = NULL; // Pointer to hold the dynamically allocated string
    size_t length = 0;  // Current length of the input
    char buffer[128];   // Temporary buffer for reading chunks of input

    // Loop to read input in chunks using fgets
    while (fgets(buffer, sizeof(buffer), stdin)) {
        size_t bufferLength = strlen(buffer);

        // Check if the input ends with a newline and remove it
        if (buffer[bufferLength - 1] == '\n') {
            buffer[bufferLength - 1] = '\0';
            bufferLength--;
        }
        // Reallocate memory to append the new chunk of input
        char *temp = realloc(input, length + bufferLength + 1);
        if (!temp) {
            free(input);
            fprintf(stderr, "Memory allocation error.\n");
            exit(EXIT_FAILURE);
        }

        input = temp;
        memcpy(input + length, buffer, bufferLength);
        length += bufferLength;
        input[length] = '\0';

        // Break the loop if the entire input was read in this chunk
        if (bufferLength < sizeof(buffer) - 1) {
            break;
        }
    }

    return input; // Return the dynamically allocated string
}

// Function to clear the input buffer after reading input
void clearBuffer() {
  scanf("%*[^\n]");
  scanf("%*c");
}


// Function to duplicate a string by dynamically allocating memory for it
char* duplicateString(const char* str) {
    if (str == NULL) {
        exit(1); // Handle NULL input
    }

    // Allocate memory for the duplicate string
    size_t length = strlen(str) + 1; // Include space for the null terminator
    char* duplicate = malloc(length);
    if (duplicate == NULL) {
        printf("Memory allocation failed\n");
        exit(1); // Exit on allocation failure
    }

    // Copy the original string into the allocated memory
    strcpy(duplicate, str);
    return duplicate; // Return the pointer to the duplicated string
}
