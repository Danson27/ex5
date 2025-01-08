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
           watchPlaylists(*playlists, currentAmount);
           break;
       }
       default: {
