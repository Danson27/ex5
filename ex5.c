/******************
Name: Dan Sonnenblick
ID: 345287882
Assignment: ex5
*******************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



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

//some recommendations for functions, you may implement however you want

void deleteSong() {

    printf("Song deleted successfully.\n");
}



void playSong() {
    
}



void freeSong() {

}

void freePlaylist(P) {
    
}

void printPlaylistsMenu() {
    printf("Please Choose:\n"); 
    printf("\t1. Watch playlists\n\t2. Add playlist\n\t3. Remove playlist\n\t4. exit\n");   
}



void sortPlaylist() {
    
    printf("sorted\n");
}



int main() {
    printf("Goodbye!\n");  
}
