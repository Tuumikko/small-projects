/* Class Player
 * ----------
 * COMP.CS.110 SPRING 2021
 * ----------
 * Class for describing player's name, points and found pairs
 *
 * Ohjelman kirjoittaja
 * Nimi: Mikko Tuumanen
 * Opiskelijanumero: 284030
 * Käyttäjätunnus: tuumanen ( Git-repositorion hakemistonimi. )
 * E-Mail: mikko.tuumanen@tuni.fi
 **/


#ifndef PLAYER_HH
#define PLAYER_HH

#include <iostream>
#include <string>
#include <vector>

class Player
{
public:
    // Rakentaja: luo annetun nimisen pelaajan.
    //
    // Constructor: creates a player with the given name.
    Player(const std::string& name);

    // Palauttaa pelaajan nimen.
    //
    // Returns the name of the player.
    std::string get_name() const;

    // Palauttaa pelaajan tähän asti keräämien parien määrän.
    //
    // Returns the number of pairs collected by the player so far.
    unsigned int get_number_of_pairs() const;

    // Siirtää annetun kortin pelilaudalta pelaajalle,
    // eli lisää kortin pelaajan keräämiin kortteihin
    // ja poistaa sen pelilaudalta.
    //
    // Moves the given card from the game board for the player,
    // i.e. inserts it to the collected cards of the player and
    // removes it from the game board.
    void add_pair(std::string fruit);

    // Tulostaa pelaajan tilanteen: nimen ja tähän asti kerättyjen parien määrän.
    //
    // Prints the game status of the player: name and collected pairs so far.
    void print() const;

    // Returns vector of pairs that player has found so far
    std::vector<std::string> get_pairs_() const;

private:
    std::string name_;
    int pairs_total_;
    std::vector<std::string> found_pairs_;
};


#endif // PLAYER_HH
