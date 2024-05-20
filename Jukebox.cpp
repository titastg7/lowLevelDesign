#include <iostream>
#include <vector>

// Forward declarations
class Song;
class Playlist;
class User;

// Song class representing a song in the jukebox
class Song {
private:
    std::string title;
    std::string artist;
    // Other song attributes...

public:
    Song(const std::string& title, const std::string& artist)
        : title(title), artist(artist) {}

    std::string getTitle() const {
        return title;
    }

    std::string getArtist() const {
        return artist;
    }
};

// Playlist class representing a playlist in the jukebox
class Playlist {
private:
    std::string name;
    std::vector<Song*> songs;

public:
    Playlist(const std::string& name)
        : name(name) {}

    std::string getName() const {
        return name;
    }

    void addSong(Song* song) {
        songs.push_back(song);
    }

    void play() const {
        std::cout << "Playing playlist: " << name << std::endl;
        for (const Song* song : songs) {
            std::cout << "Title: " << song->getTitle() << ", Artist: " << song->getArtist() << std::endl;
        }
    }
};

// User class representing a user of the jukebox
class User {
private:
    std::string name;
    // Other user attributes...

public:
    User(const std::string& name)
        : name(name) {}

    std::string getName() const {
        return name;
    }
};

// Jukebox class representing the musical jukebox
class Jukebox {
private:
    std::vector<Song*> songs;
    std::vector<Playlist*> playlists;
    User* currentUser;

public:
    void addSong(Song* song) {
        songs.push_back(song);
    }

    void addPlaylist(Playlist* playlist) {
        playlists.push_back(playlist);
    }

    void setCurrentUser(User* user) {
        currentUser = user;
    }

    void playPlaylist(const std::string& playlistName) const {
        Playlist* playlist = findPlaylist(playlistName);
        if (playlist != nullptr) {
            playlist->play();
        } else {
            std::cout << "Playlist not found." << std::endl;
        }
    }

    std::vector<Song*> searchSongsByArtist(const std::string& artist) const {
        std::vector<Song*> foundSongs;
        for (const Song* song : songs) {
            if (song->getArtist() == artist) {
                foundSongs.push_back(song);
            }
        }
        return foundSongs;
    }

    std::vector<Song*> searchSongsByTitle(const std::string& title) const {
        std::vector<Song*> foundSongs;
        for (const Song* song : songs) {
            if (song->getTitle() == title) {
                foundSongs.push_back(song);
            }
        }
        return foundSongs;
    }

private:
    Playlist* findPlaylist(const std::string& playlistName) const {
        for (Playlist* playlist : playlists) {
            if (playlist->getName() == playlistName) {
                return playlist;
            }
        }
        return nullptr;
    }
};

int main() {
    // Create songs
    Song song1("Song 1", "Artist 1");
    Song song2("Song 2", "Artist 2");
    Song song3("Song 3", "Artist 3");

    // Create playlists
    Playlist playlist1("Playlist 1");
    playlist1.addSong(&song1);
    playlist1.addSong(&song2);

    Playlist playlist2("Playlist 2");
    playlist2.addSong(&song2);
    playlist2.addSong(&song3);

    // Create user
    User user("John");

    // Create jukebox
    Jukebox jukebox;
    jukebox.addSong(&song1);
    jukebox.addSong(&song2);
    jukebox.addSong(&song3);
    jukebox.addPlaylist(&playlist1);
    jukebox.addPlaylist(&playlist2);
    jukebox.setCurrentUser(&user);

    // Play a playlist
    jukebox.playPlaylist("Playlist 1");

    // Search songs by artist
    std::string searchArtist = "Artist 2";
    std::vector<Song*> songsByArtist = jukebox.searchSongsByArtist(searchArtist);
    std::cout << "Songs by Artist: " << searchArtist << std::endl;
    for (const Song* song : songsByArtist) {
        std::cout << "Title: " << song->getTitle() << ", Artist: " << song->getArtist() << std::endl;
    }

    // Search songs by title
    std::string searchTitle = "Song 3";
    std::vector<Song*> songsByTitle = jukebox.searchSongsByTitle(searchTitle);
    std::cout << "Songs by Title: " << searchTitle << std::endl;
    for (const Song* song : songsByTitle) {
        std::cout << "Title: " << song->getTitle() << ", Artist: " << song->getArtist() << std::endl;
    }

    return 0;
}
