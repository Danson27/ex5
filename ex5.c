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




void watchPlaylists(Playlist **playlists, int *currentAmount);
void addPlaylist(Playlist ***playlists, int *currentPlaylistAmount);
void removePlaylist(Playlist ***playlists, int *currentAmount);
void displayPlaylistMenu(Playlist*** playlists, Playlist* playlist, int *currentAmount);
void showPlaylist(Playlist** playlists, Playlist* playlist, int currentAmount);
void printPlaylistsMenu();
void sortPlaylist(Playlist *playlist);
void freePlaylist();
void addSong(Playlist** playlists, Playlist* playlist, int *currentAmount);
void deleteSong(Playlist* playlists, Playlist* playlist, int *currentAmount);
void playSong();
void freeSong();


// helper functions
char* readInput();
void clearBuffer();





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
                   watchPlaylists(playlists, &playlistAmount);
                   break;
               }
               // case 2 add playlist
               case 2: {
                   addPlaylist(&playlists, &playlistAmount);
                   break;
               }
               case 3: {
                   removePlaylist(&playlists, &playlistAmount);
                   break;
               }
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


void watchPlaylists(Playlist **playlists, int *currentAmount) {
   if (*currentAmount == 0) {
       printf("Choose a playlist:\n");
       printf("\t1. Back to main menu\n");
       int choice;
       scanf("%d", &choice); {
           while (choice != 1) {
               printf("Invalid input. Please try again.\n");
               printf("Choose a playlist:\n");
               printf("\t1. Back to main menu\n");
               scanf("%d", &choice);
           }
           return;
       }
   }
   printf("Choose a playlist: \n");
   for (int i = 0; i < *currentAmount; i++) {
       printf("\t%d. %s\n", i+1, playlists[i]->name);
   }
   printf("\t%d. Back to main menu\n", *currentAmount+1);


   int selection;
   scanf("%d", &selection);
   clearBuffer();
   while (selection < 1 || selection > *currentAmount + 1) {
       printf("Invalid option\n");


       printf("Choose a playlist: \n");
       for (int i = 0; i < *currentAmount; i++) {
           printf("\t%d. %s\n", i + 1, playlists[i]->name);
       }
       printf("\t%d. Back to main menu\n", *currentAmount + 1);
       scanf("%d", &selection);
       clearBuffer();
   }
   if (selection == *currentAmount+1) {
       return;
   }


   if (selection >= 1 && selection <= *currentAmount) {
       printf("playlist %s:\n", playlists[selection - 1]->name);
       displayPlaylistMenu(&playlists, playlists[selection - 1], currentAmount);
   }
}

void addPlaylist(Playlist ***playlists, int *currentPlaylistAmount) {
   Playlist **temp = realloc(*playlists, (*currentPlaylistAmount+1) * sizeof(Playlist*));
   if (temp == NULL) {
       printf("Memory reallocation failed\n");
       exit(1);
   }
   *playlists = temp;
   Playlist *newPlaylist = malloc(sizeof(Playlist));
   if (newPlaylist == NULL) {
       printf("Memory reallocation failed\n");
       free(*playlists);
       exit(1);
   }
   printf("Enter playlist's name: \n");
   char* name = readInput();


   newPlaylist->name = malloc(strlen(name)+1);
   if (newPlaylist->name == NULL) {
       free(name);
       free(newPlaylist);
       free(*playlists);
       exit(1);
   }
   strcpy(newPlaylist->name, name);
   free(name);
   newPlaylist->songs = NULL;
   newPlaylist->songsNum = 0;


   (*playlists)[*currentPlaylistAmount] = newPlaylist;
   (*currentPlaylistAmount)++;
}

void removePlaylist(Playlist *** playlists, int *currentPlaylistAmount) {

  if (*currentPlaylistAmount == 0) {
      printf("Choose a playlist:\n");
      printf("\t1. Back to main menu\n");
      int choice;
      scanf("%d", &choice);
      {
          while (choice != 1) {
              printf("Invalid input. Please try again.\n");
              printf("Choose a playlist:\n");
              printf("\t1. Back to main menu\n");
              scanf("%d", &choice);
          }
          return;
      }
  }

   printf("Choose a playlist: \n");


   for (int i = 0; i < *currentPlaylistAmount; i++) {
       printf("\t%d. %s\n", i+1, (*playlists[i])->name);
   }
   printf("\t%d. Back to main menu\n", *currentPlaylistAmount+1);


   int selection;
   scanf("%d", &selection);
   clearBuffer();


   while (selection < 1 || selection > *currentPlaylistAmount + 1) {
       printf("Invalid option\n");
       printf("Choose a playlist: \n");
       for (int i = 0; i < *currentPlaylistAmount; i++) {
           printf("\t%d. %s\n", i + 1, (*playlists[i])->name);
       }
       printf("\t%d. Back to main menu\n", *currentPlaylistAmount + 1);
       scanf("%d", &selection);
       clearBuffer();
   }
   if (selection == *currentPlaylistAmount+1) {
       return;
   }


  if (selection >= 1 && selection <= *currentPlaylistAmount) {
      Playlist *toDelete =(*playlists)[selection-1];
          for (int i = 0; i < toDelete->songsNum; i++) {
              free(toDelete->songs[i]->title);
              free(toDelete->songs[i]->artist);
              free(toDelete->songs[i]->lyrics);
              free(toDelete->songs[i]);
          }
              free(toDelete->name);
              free(toDelete->songs);
              free(toDelete);


      for (int i = selection-1; i < (*currentPlaylistAmount-1); i++) {
          (playlists)[i] = (playlists)[i+1];
      }


      (*currentPlaylistAmount)--;
      if (*currentPlaylistAmount > 0) {
          *playlists = (Playlist**)realloc(playlists, (*currentPlaylistAmount) * sizeof(Playlist *));
          if (*playlists == NULL) {
              printf("Memory reallocation failed.\n");
              exit(1);
          }
      }
      printf("Playlist deleted.\n");
  }
  else {
      printf("Invalid option\n");
  }
}

void displayPlaylistMenu(Playlist*** playlists, Playlist* playlist, int *currentAmount) {
   int input;
   printf("\t1. Show Playlist\n"
                "\t2. Add Song\n"
                "\t3. Delete Song\n"
                "\t4. Sort\n"
                "\t5. Play\n"
                "\t6. exit\n");
   scanf("%d", &input);
   clearBuffer();
   switch (input) {
       case 1: {
           showPlaylist(*playlists, playlist, *currentAmount);
           return;
       }
       case 2: {
           addSong(*playlists, playlist, currentAmount);
           return;
       }
       case 3: {
           deleteSong(**playlists, playlist, currentAmount);
           displayPlaylistMenu(playlists, playlist, currentAmount);
           break;
       }
       case 4: {
           sortPlaylist(playlist);
           displayPlaylistMenu(playlists, playlist, currentAmount);
           break;
       }
       case 5: {
           //play Playlist
           for (int i = 0; i < playlist->songsNum; i++) {
               playlist->songs[i]->streams++;
               printf("Now playing %s:\n", playlist->songs[i]->title);
               printf("$ %s $\n ", playlist->songs[i]->lyrics);
           }
           displayPlaylistMenu(playlists, playlist, currentAmount);
           return;
       }
       case 6: {
           //watchPlaylists(*playlists, currentAmount);
           if (*currentAmount == 0) {
               printf("Choose a playlist:\n");
               printf("\t1. Back to main menu\n");
               int choice;
               scanf("%d", &choice); {
                   while (choice != 1) {
                       printf("Invalid input. Please try again.\n");
                       printf("Choose a playlist:\n");
                       printf("\t1. Back to main menu\n");
                       scanf("%d", &choice);
                   }
                   return;
               }
           }
           printf("Choose a playlist: \n");
           for (int i = 0; i < *currentAmount; i++) {
               printf("\t%d. %s\n", i+1, playlist->name);
           }
           printf("\t%d. Back to main menu\n", *currentAmount+1);


           int selection;
           scanf("%d", &selection);
           clearBuffer();
           while (selection < 1 || selection > *currentAmount + 1) {
               printf("Invalid option\n");


               printf("Choose a playlist: \n");
               for (int i = 0; i < *currentAmount; i++) {
                   printf("\t%d. %s\n", i + 1, playlist->name);
               }
               printf("\t%d. Back to main menu\n", *currentAmount + 1);
               scanf("%d", &selection);
               clearBuffer();
           }
           if (selection == *currentAmount+1) {
               return;
           }


           if (selection >= 1 && selection <= *currentAmount) {
               printf("playlist %s:\n", playlist->name);
               displayPlaylistMenu(playlists, *playlists[selection - 1], currentAmount);
           }
           break;
       }
       default: {
           printf("Invalid option\n");
       }
   }
}


void showPlaylist(Playlist** playlists, Playlist* playlist, int currentAmount) {
   for (int i = 0; i < playlist->songsNum; i++) {
       printf("%d. Title: %s \n", i+1, playlist->songs[i]->title);
       printf("   Artist: %s\n", playlist->songs[i]->artist);
       printf("   Released: %d\n", playlist->songs[i]->year);
       printf("   Streams: %d\n", playlist->songs[i]->streams);
   }
   int input;
   printf("choose a song to play, or 0 to quit:\n");
   scanf("%d", &input);
   if (playlist->songsNum != 0) {
       while (input != 0){
           printf("Now playing %s:\n", playlist->songs[input-1]->title);
           printf("$ %s $\n", playlist->songs[input-1]->lyrics);
           playlist->songs[input-1]->streams++;
           printf("choose a song to play, or 0 to quit:\n");
           scanf("%d", &input);
       }
   }
   displayPlaylistMenu(&playlists, playlist, &currentAmount);
}


void addSong(Playlist** playlists, Playlist* playlist, int *currentAmount) {
   int yearOfRelease;
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


   playlist->songs = (Song**)realloc(playlist->songs, (playlist->songsNum + 1) * sizeof(Song*));
   if (playlist->songs == NULL) {
       printf("Memory allocation failed\n");
       free(newSong->title);
       free(newSong->artist);
       free(newSong->lyrics);
       free(newSong);
       exit(1);
   }


   playlist->songs[playlist->songsNum] = newSong;
   playlist->songsNum++;


   displayPlaylistMenu(&playlists, playlist, currentAmount);
}




void deleteSong(Playlist* playlists, Playlist* playlist, int *currentAmount) {
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
   playlist->songs = realloc (playlist->songs, playlist->songsNum * sizeof(Song*));
   if (playlist->songs == NULL) {
       printf("Memory allocation failed\n");
       exit(1);
   }
   printf("Song deleted successfully.\n");
}


void sortPlaylist(Playlist *playlist) {
   int selection;
   printf("choose:\n");
   printf("1. sort by year\n");
   printf("2. sort by streams - ascending order\n");
   printf("3. sort by streams - descending order\n");
   printf("4. sort alphabetically\n");
   scanf("%d", &selection);
   if (selection == 4 || selection < 1 || selection > 4) {
       //alphabetical sort
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
           //sort by year
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


void printPlaylistsMenu() {
   printf("Please Choose:\n");
   printf("\t1. Watch playlists\n\t2. Add playlist\n\t3. Remove playlist\n\t4. exit\n");
}


char* readInput() {
   int size = 2;
   char ch;
   char* input = malloc(size * sizeof(char));
   if (input == NULL) {
       printf("Memory allocation failed\n");
       free (input);
       exit(1);
   }


   // Skip any leading newline characters
   while (scanf("%c", &ch) && (ch == '\n' || ch == '\r')) {}


   int index = 0;
   while (ch != '\n' && ch != '\r') {
       input[index] = ch;
       index++;
       if (index >= size) {
           size *= 2;
           char* temp = realloc(input, size * sizeof(char));
           if (temp == NULL) {
               printf("Memory allocation failed\n");
               exit(1);
           }
           input = temp;
       }
       scanf ("%c", &ch);
   }


   input[index] = '\0';
   return input;
}


void clearBuffer() {
   scanf("%*[^\n]");
   scanf("%*c");
}



