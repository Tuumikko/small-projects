/* Class: Player / file: player.cpp
 *
 * Ohjelman kirjoittaja
 * Nimi: Mikko Tuumanen
 * Opiskelijanumero: 284030
 * Käyttäjätunnus: tuumanen ( Git-repositorion hakemistonimi. )
 * E-Mail: mikko.tuumanen@tuni.fi
 */


#include "player.hh"

//luo pelaaja olion
Player::Player(const std::string& name):
    name_(name)
{
    pairs_total_ =0;
}


//palauttaa pelaajan nimen
std::string Player::get_name() const
{
    return name_;
}

//palauttaa pelaajan löydettyjen korttien määrän
unsigned int Player::get_number_of_pairs() const
{
    return pairs_total_;
}

//lisää parin pelaajan löydettyihin pareihin ja poistaa parin kortit pelilaudalta.
void Player::add_pair(std::string fruit)
{
    pairs_total_++;
    found_pairs_.push_back(fruit);
}

//returns vector of found pairs
std::vector<std::string> Player::get_pairs_() const
{
    return found_pairs_;
}
